#ifndef DDS_HL_HH
#define DDS_HL_HH

#include "dds_config.hh"

class DDS_hl {
	public:
		DDS_hl();
		void configureBoards();
		void updateFrequencies(float f1, float f2, float f3, float f4, float f5, float f6);
		void updateFrequencies(float f1, float f2, float f3, float f4, float f5, float f6, uint32_t numSamps);
		void updateAmplitudes(uint16_t a1, uint16_t a2, uint16_t a3, uint16_t a4, uint16_t a5, uint16_t a6);
		void sleepAll();
		void wakeAll();
		void powerDownAll();
		void serialGo();
		void waitForSerialDone();
		void Read32(uint8_t Address, uint32_t *Dat0, uint32_t *Dat1, uint32_t *Dat2, bool cs);
		void setChipSelect(bool cs);

		virtual ~DDS_hl();

	private:
		DDS_Config DDS0;
		DDS_Config DDS1;
		DDS_Config DDS2;

};
#endif

