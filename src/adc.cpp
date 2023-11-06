#include "adc.hh"

ADC::ADC(){
	char UIDev[15];
	Debug::log( LOG_DEBUG, "%s\n", UIDev);
	fprintf(stderr,"%s\n", UIDev);
	int fdUIO = open(UIDev, O_RDWR|O_SYNC);

	//setup memory mapping
	AXI = (ADC::HW*) mmap(NULL,
			sizeof(ADC::HW),
			PROT_READ|PROT_WRITE,
			MAP_SHARED,
			fdUIO,
			0x0);
	extGainA = 0;           //External gain from PGA870
	extGainB = 0;
	standbyMode = 0;       //standby on or off
	intGainA = 0;          //ChA internal gain
	intGainB = 0;          //ChB internal gain
	ChATestPattern = 0;    //ChA Test pattern off
	ChBTestPattern = 0;    //ChB Test pattern off
	captureLength = 512;   //Capture length (integer)
	capLenCode = 0;        //Coded capture length
	resultMode = 0;        //0 --read waveform from register and 1 -- process waveform with Goertzel
	gainSerialStart = false;
	startProcessing = false;
	capENA = false;
	capENB = false;
	reset = false;
	
	for (int i = 0; i<6; i++) {
		filterFrequencies[i] = 0; //Initialize all filters to 1 MHz
		ChAReal[i] = 0;
		ChAImag[i]=0;
		ChBReal[i]=0;
		ChBImag[i]=0;
	}
	
	//Register 15 has to be written to 1 or else it won't work
	ADC::writeRegister(0x15,0x01); //!!SETS ADC TO DDR MODE!!!!
}

uint32_t ADC::getReg0() {
	uint32_t r0 = 0;
	r0 = (r0 |
			(extGainA << 26) |
			(extGainB << 20) |
			(gainSerialStart << 19) |
			(startProcessing << 17) |
			(capLenCode << 13) |
			(resultMode << 12) |
			(capENA << 5) |
			(capENB << 4) |
			(reset << 2) |
			(0x01 << 1) //Leave Serial enable high
	);
	return r0;
}

//Reset() strobes the ADC reset pin. Must be done after power up
void ADC::Reset(){
	reset = true;
	AXI->reg[0] = getReg0();
	reset = false;
	AXI->reg[0] = getReg0();
	ADC::writeRegister(0x15,0x01); //DDR mode
}

//start saving ADC data to RAM
void ADC::StartCapture(){
	capENA = true;
	capENB = true;
	AXI->reg[0] = getReg0();
}

//Stop saving ADC data to RAM to read it back
void ADC::StopCapture(){
	capENA = false;
	capENB = false;
	AXI->reg[0] = getReg0();
}

//start process waveform
void ADC::StartProcessing(){
	resultMode = 1;
	startProcessing = true;
	AXI->reg[0] = getReg0();
}

//stop data processing
void ADC::StopProcessing() {
	resultMode = 0;
	startProcessing = false;
	AXI->reg[0] = getReg0();
}

//get ADC data from RAM. Returns a single value from FPGA RAM
uint32_t ADC::getData(uint16_t RAMAddrA, uint16_t RAMAddrB){
	//Set the addresses
	AXI->reg[1] = ((uint32_t)RAMAddrB<<16) | ((uint32_t)RAMAddrA);
	return AXI->reg[2];
}

//read data stored in specified addresses
void ADC::readRange(uint16_t startAddr, uint16_t stopAddr, uint32_t *data){
	uint16_t numSamps = stopAddr - startAddr;
	if (numSamps <= 0) {
		fprintf(stderr, "stop address must be greater than start address\n");
	}
	for (uint16_t i=0; i<=numSamps; i++) {
		data[i]=ADC::getData(startAddr+i, startAddr+i);
	}
}

/*
 *Purpose: Sets the ADC into a test pattern mode for debugging
 *
 *NOTES: Values taken from Table 20 of the ADS42LB69 Datasheet
 *0 = Normal Operation
 *1 = All 0s
 *2 = All 1s
 *3 = Toggle pattern (ie 1010101010...)
 *4 = Ramp 1 increment per clock cycle
 *8 = Deskew pattern (0xAAAA)
 *10 = Random numbers
 *11 = 8 point sinewave
 */
void ADC::setTestPattern(uint8_t chA, uint8_t chB){
	ChATestPattern = chA & 0x0F;
	ChBTestPattern = chB & 0x0F;
	if (ChATestPattern > 11 || ChATestPattern == 5 || ChATestPattern ==6 || ChATestPattern == 7 || ChATestPattern == 9) {
		fprintf(stderr, "Channel A Test pattern must be 0-4, 8, 10, or 11\n");
	}
	if (ChBTestPattern > 11 || ChBTestPattern == 5 || ChBTestPattern ==6 || ChBTestPattern == 7 || ChBTestPattern == 9) {
		fprintf(stderr, "Channel B Test pattern must be 0-4, 8, 10, or 11");
	}
	ADC::writeRegister(0x0F,((ChATestPattern<<4) | ChATestPattern));
}

//Puts the ADC into standby mode if it's awake or pulls it out of standby mode if it's asleep
void ADC::toggleStandby(){
	if (standbyMode > 0) {
		standbyMode = 0;
	} else {
		standbyMode = 1;
	}
	ADC::writeRegister(0x08, ((standbyMode << 5) | //set standby mode
							(0x01 << 3))); //Disable floating control pins
}

void ADC::standbyOn(){
	standbyMode = 1;
	ADC::writeRegister(0x08, ((1 << 5) | (1 << 3)));
}

void ADC::standbyOff(){
	standbyMode = 0;
	ADC::writeRegister(0x08, ((0 << 5) | (1 << 3)));
}

void ADC::setInternalGain(uint8_t igainA, uint8_t igainB){
	if (igainA < 3 || igainA > 19) {
		intGainA = 0;
		ADC::writeRegister(0x0B,0x00);
	} else {
		intGainA = igainA;
		ADC::writeRegister(0x0B, (((intGainA & 0x1F) << 3) | //Set Gain
			(0x01 << 2))); //Enable Gain
	}
	if (igainB < 3 || igainB > 19) {
		intGainB = 0;
		ADC::writeRegister(0x0C, 0x00);
	} else {
		intGainB = igainB
		ADC::writeRegister(0x0C, (((intGainB & 0x1F) << 3) | //Set Gain
			(0x01 << 2))); //enable channel B gain
	}
}

/*
 *Inputs:
 *eGain   //6 bit PGA870 Gain control pins
 *Purpose: Program the PGA870 Amplifier
 */
void ADC::setExternalGain(uint8_t eGainA, uint8_t eGainB) {
	extGainA = eGainA & 0x3F;
	extGainB = eGainB & 0x3F;
	gainSerialStart = true;
	AXI->reg[0] = getReg0();
	gainSerialStart = false;
	AXI->reg[0] = getReg0();
	while(!gainSerialDone()) {
		continue;
	}
}

bool ADC::gainSerialDone() {
	return ((AXI->reg[0] >> 6) & 0x01);
}

/*
 *INPUTS:
 *capLen //Capture length 0 = 512
 *       //               1 = 1024
 *       //               2 = 2048
 *       //               .
 *       //               .
 *       //               7 = 65536
 */
void ADC::setCaptureLength(uint32_t inputLength) {
	captureLength = inputLength
	if (captureLength <= 512) {capLenCode = 0;}
	else if (captureLength <= 1024) {capLenCode = 1;}
	else if (captureLength <= 2048) {capLenCode = 2;}
	else if (captureLength <= 4096) {capLenCode = 3;}
	else if (captureLength <= 8192) {capLenCode = 4;}
	else if (captureLength <= 16384) {capLenCode = 5;}
	else if (captureLength <= 32768) {capLenCode = 6;}
	else {capLenCode = 7;}

	AXI->reg[0] = getReg0();
}

/* 
 * Writes the filter frequencies to the correct registers
 * Frequencies are in MHz. First they get translated to a frequency index which is a 7-bit number.
 */

void ADC::setFilterFrequencies() {
	uint8_t freqIdx[6];
	//Set the frequency index
	//1 MHz is index 0, 2 MHz is index 1, etc.
	for (uint8_t i = 0; i<6; i++) {
		freqIdx[i] = (filterFrequencies[i]-1) & 0x7F;
	}

	AXI -> reg[4] = ((freqIdx[2] << 16) |
			(freqIdx[1] << 8) |
			(freqIdx[0]));
	AXI -> reg[5] = ((freqIdx[5] << 16) |
			(freqIdx[4] << 8) |
			(freqIdx[3]));
}

/*
 *INPUTS: uint8_t filtNum  //The filter number to return
 *Gets the result from Goertzel algorithm processing from the specified filter number and saves it to the result variable arrays
 */
void ADC::getGoertzelResult(uint8_t filtNum) {
	if (filtNum < 0 || filtNum > 5) {
		fprintf(stderr, "Filter number must be between 0 and 5.");
	} else {
		ChAReal[filtNum] = AXI->reg[4*filtNum+6];
		ChAImag[filtNum] = AXI->reg[4*filtNum+7];
		ChBReal[filtNum] = AXI->reg[4*filtNum+8];
		ChBImag[filtNum] = AXI->reg[4*filtNum+9];
	}
}

/*
 *Reads all of the Goertzel results and saves them to the results variables
 */
void ADC::getAllGoertzelResults() {
	for (uint8_t i = 0; i<6; i++) {
		ADC::getGoertzelResult(i);
	}
}

/*
 *Designed to be polled repeatedly. Will return 1 when the Goertzel algorithm is finished
 */
bool ADC::resultsReady() {
	uint32_t regDat = AXI->reg[0];
	return((regDat >> 16) & 0x01);
}

bool ADC::captureDone() {
	uint32_t regDat = AXI->reg[0];
	return((regDat >> 18) & 0x01);
}

uint32_t ADC::readAXIRegister(uint8_t reg) {
	return AXI->reg[reg];
}

void ADC::writeAXIRegister(uint8_t reg, uint32_t data) {
	AXI->reg[reg] = data;
}

/* 
 * PURPOSE: Reads a 8 bit data word from an Address
 */
uint8_t ADC::readRegister(uint8_t Address) {
	//Register 0, bit 0 is the clock
	//Register 0, bit 1 is Enable (Active Low)
	//Register 0, bit 3 is MOSI
	//Register 3, bit 0 is MISO

	uint8_t value = 0x00;
	//Build instructionByte
	uint8_t instructionByte = 0x80 | (Address & 0x3F);
	//Bang out the instructionByte
	for (uint8_t p = 0; p<8; p++) {
		if (instructionByte << p & 0x80) {
			AXI->reg[0] = 0x08;
			AXI->reg[0] = 0x09;
		} else {
			AXI->reg[0] = 0x00;
			AXI->reg[0] = 0x01;
		}
	}

	//Read the data
	for (uint8_t p = 0; p<8; p++) {
		AXI->reg[0] =  0x00;
		AXI->reg[0] =  0x01;
		value = value | (AXI->reg[3] << (7-p));

	}
	AXI->reg[0] = 0x00;
	//Set SEN to HIGH, Clock to Low and return External Gain setting
	AXI->reg[0] = getReg0();
	return value;
}

/*
 * uint8_t Address (Address of the register where the data will be written)
 * uint8_t Data   (Data to write to the specified Address)
 * TestHW *HW      (Pointer to the UIO device. Essentially a "Board select")
 * PURPOSE: Writes a 8 bit data word to an Address
 */

void ADC::writeRegister(uint8_t Address, uint8_t Data) {
	//Register 0, bit 0 is the clock
	//Register 0, bit 1 is Enable (Active Low)
	//Register 0, bit 3 is MOSI
	//Register 3, bit 0 is MISO

	//Pull SEN Low
	AXI->reg[0] = 0x00;

	//Build the instruction byte 0b0XAAAAAA
	uint8_t instructionByte = 0x00 | (Address & 0x3F);
	for (uint8_t p = 0; p<8; p++) {
		if (instructionByte << p & 0x80) {
			AXI->reg[0] = 0x08;
			AXI->reg[0] = 0x09;
		} else {
			AXI->reg[0] = 0x00;
			AXI->reg[0] = 0x01;
		}
	}
	//Write the data
	for (uint8_t p = 0; p<8; p++) {
		if (Data << p & 0x80) {
			AXI->reg[0] = 0x08;
			AXI->reg[0] = 0x09;
		} else {
			AXI->reg[0] = 0x00;
			AXI->reg[0] = 0x01;
		}
	}
	AXI->reg[0] = 0x00;
	//Set SEN High and return external gain value
	AXI->reg[0] = getReg0();
}

ADC::~ADC(){
	DebugLog::log( LOG_DEBUG, "destructing, ADC class");
}
