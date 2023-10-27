#include "dds_config.hh"

DDS_Config::DDS_Config(int uio){
	Board = UIO;
	if (UIO > 4 || UIO < 0) {
		fprintf(stderr,"UIO must be between 0 and 4");
	}
	char UIDev[15];
	sprintf(UIDev, "/dev/uio%d",UIO);
	fprintf(stderr,"%s\n", UIDev);
	int fdUIO = open(UIDev, O_RDWR|O_SYNC);
	//setup memory mappings
	AXI = (DDS::HW*) mmap(NULL,
			sizeof(DDS::HW),
			PROT_READ|PROT_WRITE,
			MAP_SHARED,
			fdUIO,
			0x0);  //This may need some page size rounding for safety, but for now the AXI devices are easily on page boundaries. 
	frequency[0] = 50.0;
	frequency[1] = 50.0;
	phase[0] = 0;
	phase[1] = 0;
	amplitude[0] = 0x3FFF;
	amplitude[1] = 0x3FFF;
	sleep = 0;
	powerdown = 0;
	sincFilt = 0;
	refClkOut = 0;
	PLLCurrent = 0x02;
	refClkOut = 0;
	refClkDivider = 1;
	refClkMultiplier = 40;
	sysClk = (uint16_t)(refClkMultiplier * DDS_OSC);
	VCO = 0x05;
	ftw[0] = frequency2ftw(frequency[0],sysClk);
	pow[0] = phase2pow(phase[0]);
	ftw[1] = frequency2ftw(frequency[1],sysClk);
	pow[1] = phase2pow(phase[1]);
	actualFreq[0] = ftw2frequency(ftw[0],sysClk);
	actualFreq[1] = ftw2frequency(ftw[1],sysClk);
	chipSelect = false;
	getConfigData();
	profileCMD = getProfileCMD(false);	
}

bool DDS::serialDone() {
	return ((AXI->reg[0] >> 12) & 0x01);
}

bool DDS::getChipSelect() {
	chipSelect = (AXI->reg[0] >> 24) & 0x01;
	return chipSelect;
}

void DDS::setChipSelect(bool cs) {
	uint32_t axiCopy = AXI->reg[0] & 0xFEFFFFFF; //Clear the chip select bit
	AXI->reg[0] = axiCopy | (cs << 24);
	chipSelect = cs;
}
/*
 *PURPOSE: sets Configuration register words to write to DDS chips prepare for a call to IOUpdate
 */
void DDS::getConfigData() {
	sysClk = (uint16_t)(refClkMultiplier * DDS_OSC);
	//Set the VCO for PLL based on the system clock
	if (sysClk > 850 ) {VCO = 0x05;}
	else if (sysClk >  750) {VCO = 0x04;}
	else if (sysClk >650) {VCO = 0x03;}
	else if (sysClk >550) {VCO = 0x02;}
	else if (sysClk >450) {VCO = 0x01;}
	else {VCO = 0x00;}
	uint32_t powerBits = 0;
	if (sleep) {powerBits = ((0x01 << 7) | (0x01 << 5) | (0x01 << 4));}
	if (powerdown) {powerBits = (0x0F << 4);}
	CFR1 = ((0x01 << 1) |  //Set to 3 wire serial
		((sincFilt &0x01) << 22)| //toggles sinc filt
		powerBits); //Toggles powerdown status
	CFR2 = ((0x01 << 24) | //Enable amplitude scaling
		(0x00 << 22) | //Disable Sync clock
		(0x01 << 11) | //Enable PD clock
		(0x01 << 5));  //Sync timing validation disable
	CFR3 = (((refClkOut & 0x01) << 28) | //Disable RefClock Out
		((VCO & 0x07) << 24) | //Set VCO for PLL
		((PLLCurrent&0x07) << 19) | //Set PLL feedback current
		((refClkDivider & 0x01) << 14) | //Enable Refclock divider
		(0x01 << 8 ) | //Enable PLL
		(refClkMultiplier   << 1));  //Set the Refclock multiplier

}

/*
*PURPOSE: Gets 64 bit control word to send to DDS Board
*/
uint64_t DDS::getProfileCMD(bool cs){
	uint64_t profData;
	uint16_t truncASF = amplitude[cs] & 0x3FFF;
	ftw[cs] = frequency2ftw(frequency[cs],sysClk);
	pow[cs] = phase2pow(phase[cs]);
	profData = (((uint64_t)truncASF << 48)|
			((uint64_t)pow[cs] << 32) |
			((uint64_t)ftw[cs]));
	return profData;
}


/*
*PURPOSE: When called for one DDS Resets all DDS's
*/
void DDS::ResetDDS(){
	AXI->reg[0] = 0x00100000; //Flips the reset bit high
	AXI->reg[0] = 0x00000000; //Returns it to low
}

/*
*PURPOSE: When called for one DDS, Resets the IOBuffer for all DDS's
*/
void DDS::IOReset(){
	AXI->reg[0] = 0x00010000; //Flip the IOreset bit high
	AXI->reg[0] = 0x00000000; //Return it to low
}


void DDS::Write32(uint8_t Address, uint32_t Data){
	uint32_t upperBits = AXI->reg[0] & 0xFFFFFD00; //Clear bit 9, and 0-7
	AXI->reg[0] = upperBits | (Address & 0x7F) | (0x01 << 9); //Add 1 to bit 9 b/c it's a 32 bit register
	AXI -> reg[1] = Data;
}

void DDS::Write64(uint8_t Address, uint64_t Data){
	uint32_t upperBits = AXI->reg[0] & 0xFFFFFD00; //Clear bit 9, and 0-7
	AXI->reg[0] = upperBits | (Address & 0x7F); //Replace bits 0-7

	uint32_t MSB = (uint32_t)((Data & 0xFFFFFFFF00000000)>>32);
	uint32_t LSB = (uint32_t)(Data & 0x00000000FFFFFFFF);
	AXI->reg[1] = MSB;
	AXI->reg[2] = LSB;
}


/*
*PURPOSE: Builds command to read a 32 bit register from DDS
*NOTE: When serial start is active (bit 8) this function will populate reg[3] with the requested read. That's why it doesn't return anything
*/
void DDS::Read32(uint8_t Address){
	uint32_t upperBits = AXI->reg[0] & 0xFFFFFD00;
	uint8_t truncAddress = (Address & 0x7F) | (0x01 << 7);
	AXI->reg[0] = upperBits | truncAddress | (0x01 << 9);
	
}

/*
*
*PURPOSE: Same as above, but with 64 bits. reg[3] (MSB) and reg[4] (LSB) are populated.
*/
void DDS::Read64(uint8_t Address){
	uint32_t upperBits = AXI->reg[0] & 0xFFFFFD00;
	uint8_t truncAddress = (Address & 0x7F) | (0x01 << 7);
	AXI->reg[0] = upperBits | truncAddress;
}

/*
*PURPOSE: Recalculates the system clock and outputs the actual output frequency with the given ftw
*/
double DDS::GetActualFreq(uint32_t fword){
	sysClk = (uint16_t)(refClkMultiplier * DDS_OSC);
	return(ftw2frequency(fword,sysClk));
}


uint32_t DDS::frequency2ftw(float f, uint16_t sysClk) {
	return (uint32_t)((double)(4294967296.0) * ((double)f/(double)sysClk));
}

/*
*uint32_t ftw //Frequency tuning word
*OUTPUT: The actual frequency output by DDS chip
*NOTES: Formula from Page 22 of AD9910 Datasheet
*/
double DDS::ftw2frequency(uint32_t ftw, uint16_t sysClk){
	return ((double)ftw/(double)(4294967296.0)) * (double)sysClk;
}

uint16_t DDS::phase2pow(float p){
	uint16_t po = (uint16_t)(((double)(65536.0)*(double)(p))/(double)(6.2831853));
	return po;
}

DDS_Config::~DDS_Config(){
	fprintf(stderr, "destructing DDS_config class");
}
