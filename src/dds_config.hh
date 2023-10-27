#ifndef DDS_CONFIG_HH
#define DDS_CONFIG_HH

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define DDS_P0RT0 0
#define DDS_P0RT1 1	
#define DDS_P0RT2 3

class DDS_Config{
	public:
		DDS_Config(int uio);
		double GetActualFreq(uint32_t fword);
		void getConfigData();
		uint64_t getProfileCMD(bool cs);
		void ResetDDS();					//Resets DDS Boards
		void IOReset(); 					//Resets IO register
		void Write32(uint8_t address, uint32_t data);
		void Write64(uint8_t address, uint64_t data);
		void Read32(uint8_t data);
		void Read64(uint8_t data);
		bool getChipSelect();
		void setChipSelect(bool cs);
		bool serialDone();

		virtual ~DDS_Config();

	private:
		struct HW {               //AXI Registers
			uint32_t reg[4];
		};
		volatile HW *AXI;         //Location of AXI registers
		float frequency[2];       //desired frequency
		double actualFreq[2];     //Actual frequency output
		float phase[2];           //Desired phase
		uint16_t amplitude[2];    //Amplitude
		bool sincFilt;            //Use the sinc filter?
		bool refClkOut;           //Output the reference clock?
		uint8_t PLLCurrent;       //How much current to use in the PLL detector
		bool refClkDivider;       //I'm not sure what this does
		uint8_t refClkMultiplier; //Multiplier for system clock
		uint8_t Board;            //Board select
		uint32_t CFR1, CFR2, CFR3;
		uint64_t profileCMD;
		bool sleep;               //If set high and written to CFR0, puts DDS to sleep
		bool powerdown;           //If set high and written to CFR0, powers down DDS
		bool chipSelect;
		
		uint16_t sysClk;
		uint32_t ftw[2];
		uint16_t pow[2];
		uint8_t VCO;
		uint32_t frequency2ftw(float ftw, uint16_t sysClk);
		double ftw2frequency(uint32_t ftw, uint16_t sysClk);
		uint16_t phase2pow(float p);

};
#endif
