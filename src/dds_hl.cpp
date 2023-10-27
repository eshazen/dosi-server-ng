#include "dds_hl.hh"

/*Constructor
 *
 *Opens communication with the DDS Boards
*/
DDS_hl::DDS_hl(int UIO0, int UIO1, int UIO2):
	DDS0(UIO0),
	DDS1(UIO1),
	DDS2(UIO2){
	DDS0.IOReset();
}

/*
*PURPOSE: Writes the 3 configuration registers to all the DDS Boards
*/
void DDS_hl::configureBoards(){
	//Write CFR1 to register 0 for each board
	setChipSelect(false);
	DDS0.Write32(0x00, DDS0.CFR1);
	DDS1.Write32(0x00, DDS1.CFR1);
	DDS2.Write32(0x00, DDS2.CFR1);
	serialGo();
	//Write CFR2 to register 1 for each board
	DDS0.Write32(0x01, DDS0.CFR2);
	DDS1.Write32(0x01, DDS1.CFR2);
	DDS2.Write32(0x01, DDS2.CFR2);
	serialGo();
	//Write CFR3 to register 2 for each board
	DDS0.Write32(0x02, DDS0.CFR3);
	DDS1.Write32(0x02, DDS1.CFR3);
	DDS2.Write32(0x02, DDS2.CFR3);
	serialGo();

	setChipSelect(true);
	//usleep(1000);
	DDS0.Write32(0x00, DDS0.CFR1);
	DDS1.Write32(0x00, DDS1.CFR1);
	DDS2.Write32(0x00, DDS2.CFR1);
	serialGo();
	//Write CFR2 to register 1 for each board
	DDS0.Write32(0x01, DDS0.CFR2);
	DDS1.Write32(0x01, DDS1.CFR2);
	DDS2.Write32(0x01, DDS2.CFR2);
	serialGo();
	//Write CFR3 to register 2 for each board
	DDS0.Write32(0x02, DDS0.CFR3);
	DDS1.Write32(0x02, DDS1.CFR3);
	DDS2.Write32(0x02, DDS2.CFR3);
	serialGo();
}


/*
*PURPOSE: Change the frequency of the DDSes. Amplitude is fixed at max, phase is fixed at 0
*/
void DDS_hl::updateFrequencies(float f1, float f2, float f3,float f4,float f5, float f6) {
	setChipSelect(false);
	//Write new frequencies to the DDS boards
	DDS0.frequency[0] = f2;
	DDS1.frequency[0] = f4;
	DDS2.frequency[0] = f6;
	//Get the profile command associated with each frequency
	//getProfileCMD calculates the 64 bit data word to write to the profile register 0x0E
	//It includes the frequency tuning word, phase offset word, and amplitude word
	uint64_t profData0 = DDS0.getProfileCMD(false);
	uint64_t profData1 = DDS1.getProfileCMD(false);
	uint64_t profData2 = DDS2.getProfileCMD(false);
	//Write the data to the boards

	DDS0.Write64(0x0E,profData0);
	DDS1.Write64(0x0E,profData1);
	DDS2.Write64(0x0E,profData2);
	serialGo();
	setChipSelect(true);

	//Write new frequencies to the DDS boards
	DDS0.frequency[1] = f1;
	DDS1.frequency[1] = f3;
	DDS2.frequency[1] = f5;
	//Get the profile command associated with each frequency
	//getProfileCMD calculates the 64 bit data word to write to the profile register 0x0E
	//It includes the frequency tuning word, phase offset word, and amplitude word
	profData0 = DDS0.getProfileCMD(true);
	profData1 = DDS1.getProfileCMD(true);
	profData2 = DDS2.getProfileCMD(true);
	//Write the data to the boards
	DDS0.Write64(0x0E,profData0);
	DDS1.Write64(0x0E,profData1);
	DDS2.Write64(0x0E,profData2);
	serialGo();

}



void DDS_hl::updateAmplitudes(uint16_t a1, uint16_t a2, uint16_t a3,uint16_t a4, uint16_t a5, uint16_t a6) {
	setChipSelect(false);
	DDS0.amplitude[0] = a2;
	DDS1.amplitude[0] = a4;
	DDS2.amplitude[0] = a6;
	uint64_t profData0 = DDS0.getProfileCMD(false);
	uint64_t profData1 = DDS1.getProfileCMD(false);
	uint64_t profData2 = DDS2.getProfileCMD(false);
	//Write the data to the boards
	DDS0.Write64(0x0E,profData0);
	DDS1.Write64(0x0E,profData1);
	DDS2.Write64(0x0E,profData2);
	serialGo();

	setChipSelect(true);

	DDS0.amplitude[1] = a1;
	DDS1.amplitude[1] = a3;
	DDS2.amplitude[1] = a5;
	
	profData0 = DDS0.getProfileCMD(true);
	profData1 = DDS1.getProfileCMD(true);
	profData2 = DDS2.getProfileCMD(true);
	//Write the data to the boards
	//fprintf(stderr,"profile0b: 0x%016Lx\n",profData0);
	DDS0.Write64(0x0E,profData0);
	DDS1.Write64(0x0E,profData1);
	DDS2.Write64(0x0E,profData2);
	serialGo();
}


void DDS_hl::updateFrequencies(float f1, float f2, float f3, float f4, float f5, float f6, uint32_t numSamps) {
	setChipSelect(false);
	float sampRate = 250;
	uint32_t bin1 = (uint32_t)(0.5 + (numSamps * f2)/sampRate);
	uint32_t bin2 = (uint32_t)(0.5 + (numSamps * f4)/sampRate);
	uint32_t bin3 = (uint32_t)(0.5 + (numSamps * f6)/sampRate);

	float binFreq1 = (bin1 * sampRate) / numSamps;
	float binFreq2 = (bin2 * sampRate) / numSamps;
	float binFreq3 = (bin3 * sampRate) / numSamps;

	DDS0.frequency[0] = binFreq1;
	DDS1.frequency[0] = binFreq2;
	DDS2.frequency[0] = binFreq3;
	
	uint64_t profData0 = DDS0.getProfileCMD(false);
	uint64_t profData1 = DDS1.getProfileCMD(false);
	uint64_t profData2 = DDS2.getProfileCMD(false);
	//Write the data to the boards
	DDS0.Write64(0x0E,profData0);
	DDS1.Write64(0x0E,profData1);
	DDS2.Write64(0x0E,profData2);
	serialGo();

	setChipSelect(true);
	bin1 = (uint32_t)(0.5 + (numSamps * f1)/sampRate);
	bin2 = (uint32_t)(0.5 + (numSamps * f3)/sampRate);
	bin3 = (uint32_t)(0.5 + (numSamps * f5)/sampRate);

	binFreq1 = (bin1 * sampRate) / numSamps;
	binFreq2 = (bin2 * sampRate) / numSamps;
	binFreq3 = (bin3 * sampRate) / numSamps;

	DDS0.frequency[1] = binFreq1;
	DDS1.frequency[1] = binFreq2;
	DDS2.frequency[1] = binFreq3;

	profData0 = DDS0.getProfileCMD(true);
	profData1 = DDS1.getProfileCMD(true);
	profData2 = DDS2.getProfileCMD(true);
	//Write the data to the boards
	DDS0.Write64(0x0E,profData0);
	DDS1.Write64(0x0E,profData1);
	DDS2.Write64(0x0E,profData2);
	serialGo();
}

/*
*PURPOSE: Puts the DDS Boards into powersave mode. There is still power going to the PLL. Waking up should be quite fast
*/
void DDS_hl::sleepAll() {
	//Set the sleep signal high for each board
	DDS0.sleep = 1;
	DDS1.sleep = 1;
	DDS2.sleep = 1;
	//Update the CFR registers with the new sleep state
	DDS0.getConfigData();
	DDS1.getConfigData();
	DDS2.getConfigData();

	setChipSelect(false);
	//Write register CFR1;
	DDS0.Write32(0x00, DDS0.CFR1);
	DDS1.Write32(0x00, DDS1.CFR1);
	DDS2.Write32(0x00, DDS2.CFR1);
	//Tell FPGA to clock out serial commands
	serialGo();

	setChipSelect(true);
	//usleep(100);
	//Write register CFR1;
	DDS0.Write32(0x00, DDS0.CFR1);
	DDS1.Write32(0x00, DDS1.CFR1);
	DDS2.Write32(0x00, DDS2.CFR1);
	//Tell FPGA to clock out serial commands
	serialGo();
}

/*
*PURPOSE: Wake all DDS chips up from power save or power down mode
*/
void DDS_hl::wakeAll() {
	//Turn off all power saving options
	DDS0.powerdown = 0;
	DDS1.powerdown = 0;
	DDS2.powerdown = 0;
	DDS0.sleep = 0;
	DDS1.sleep = 0;
	DDS2.sleep = 0;
	//Update the CFR registers
	DDS0.getConfigData();
	DDS1.getConfigData();
	DDS2.getConfigData();
	//Writ CFR1 with the new sleep state
	setChipSelect(false);
	DDS0.Write32(0x00, DDS0.CFR1);
	DDS1.Write32(0x00, DDS1.CFR1);
	DDS2.Write32(0x00, DDS2.CFR1);
	//Clock out the serial commands
	serialGo();

	setChipSelect(true);
	//usleep(100);
	DDS0.Write32(0x00, DDS0.CFR1);
	DDS1.Write32(0x00, DDS1.CFR1);
	DDS2.Write32(0x00, DDS2.CFR1);
	//Clock out the serial commands
	serialGo();
}

/*
*PURPOSE: Power down all DDS Boards to save power and limit heating of the boards/box
*/
void DDS_hl::powerDownAll() {
	//Update power down state
	DDS0.powerdown = 1;
	DDS1.powerdown = 1;
	DDS2.powerdown = 1;
	//Update CFR Registers
	DDS0.getConfigData();
	DDS1.getConfigData();
	DDS2.getConfigData();
	setChipSelect(false);
	//Write CFR1
	DDS0.Write32(0x00, DDS0.CFR1);
	DDS1.Write32(0x00, DDS1.CFR1);
	DDS2.Write32(0x00, DDS2.CFR1);
	//Clock out the Data
	serialGo();
	setChipSelect(true);
	//usleep(100);
	DDS0.Write32(0x00, DDS0.CFR1);
	DDS1.Write32(0x00, DDS1.CFR1);
	DDS2.Write32(0x00, DDS2.CFR1);
	//Clock out the Data
	serialGo();
}

/*
*PURPOSE: Signal FPGA to start serial communication with the DDS Boards. This is accomplished with a rising edge on bit 8 of register[0]
*NOTE: Any rising edge of bit 8 on ANY DDS board will initiate serial communication
*/
void DDS_hl::serialGo(){
	//Flip bit 8 to 1
	DDS2.AXI -> reg[0] = ((DDS2.AXI->reg[0]) | (0x01 << 8));
	//Immediately clear bit 8
	DDS2.AXI -> reg[0] = ((DDS2.AXI -> reg[0]) & (0xFFFFFEFF));
	//Wait for serial command to finish
	waitForSerialDone();
}

void DDS_hl::waitForSerialDone() {
	while(!DDS2.serialDone()) {
		continue;
	}
}

/*
*PURPOSE: Read a 32 bit register from the DDS Boards
*INPUTS: Address // the register address to read
*        Dat0    // the Data in that register from DDS0
*        Dat1    // Same for DDS1
*        Dat2    // Same for DDS2 
*/
void DDS_hl::Read32(uint8_t Address, uint32_t* Dat0, uint32_t* Dat1, uint32_t* Dat2, bool cs){
	setChipSelect(cs);
	DDS0.Read32(Address);
	DDS1.Read32(Address);
	DDS2.Read32(Address);
	serialGo();
	*Dat0 = DDS0.AXI -> reg[3];
	*Dat1 = DDS1.AXI -> reg[3];
	*Dat2 = DDS2.AXI -> reg[3];
}

void DDS_hl::setChipSelect(bool cs) {
	DDS0.setChipSelect(cs);
	DDS1.setChipSelect(cs);
	DDS2.setChipSelect(cs);
}

DDS_hl::~DDS_hl(){
	fprintf(stderr, "destructing DDS_hl class");
}
