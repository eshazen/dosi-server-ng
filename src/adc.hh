#ifndef ADC_HH
#define ADC_HH

#include <sys/mman.h>
#include <sys/stat.h>
#include<fcntl.h>
#include<stdlib.h>
#include<stdio.h>

#define ADC_PORT 2

//part number of ADC: ADS42LB69
class ADC{
	public:
		ADC();
		void Reset(); //Hard Reset of ADC
		void StartCapture(); //Start saving data to RAM
		void StopCapture(); //Stop saving data to RAM
		void StartProcessing(); //Starts processing data
		void StopProcessing(); //Stops processing data
		//Read Ram at an address
		uint32_t getData(uint16_t RAMAddrA, uint16_t RAMAddrB);
		//Read Ram at a range of addresses
		void readRange(uint16_t startAddr, uint16_t stopAddr, uint32_t *data);
		//Read Serial Register
		uint8_t readRegister(uint8_t Address);
		void writeRegister(uint8_t Address, uint8_t Data);
		//Set the Test Patterns
		void setTestPattern(uint8_t chA, uint8_t chB);
		//Toggle standby status
		void toggleStandby();
		void standbyOn();
		void standbyOff();
		//Set internal gain
		void setInternalGain(uint8_t iGainA, uint8_t iGainB);
		//Set External Gain
		void setExternalGain(uint8_t eGainA, uint8_t eGainB);
		//Set Capture Length
		void setCaptureLength(uint32_t inputLength);
		//Set Filter Frequencies
		void setFilterFrequencies();
		//Read 1 Goertzel Result
		void getGoertzelResult(uint8_t filtNum);
		//Read all Goertzel Results
		void getAllGoertzelResults();
		//Checks if results are ready
		bool resultsReady();
		bool captureDone();
		//read AXI register
		uint32_t readAXIRegister(uint8_t reg);
		//write AXI register
		void writeAXIRegister(uint8_t reg,uint32_t data);
		bool gainSerialDone();


	private:
		uint8_t extGainA;		//PGA870 gain for channel A
		uint8_t extGainB;		//PGA870 gain for channel B
		bool standbyMode; 		//Turn ADC to standby
		uint8_t intGainA; 		//ADC internal Gain, 0 or 3-19
		uint8_t intGainB;
		uint32_t captureLength;
		bool resultMode; 		//0 -- read waveform from register and 1 -- process waveform

		bool gainSerialStart;
		bool startProcessing;
		bool capENA;
		bool capENB;
		bool reset;

		uint8_t ChATestPattern; //Test patterns for each channel
		uint8_t ChBTestPattern;

		uint8_t filterFrequencies[6];
		int32_t ChAReal[6];
		int32_t ChAImag[6];
		int32_t ChBReal[6];
		int32_t ChBImag[6];
		uint32_t chAReal_fixed[6];
		uint32_t chAImag_fixed[6];
		uint32_t chBReal_fixed[6];
		uint32_t chBImag_fixed[6];
		struct HW {
		uint32_t reg[30];
		};
		volatile HW *AXI;
		uint8_t capLenCode;
		uint32_t getReg0();							//setup the ADC register
};
#endif
