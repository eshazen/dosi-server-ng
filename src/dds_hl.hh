#ifndef DDS_HL_HH
#define DDS_HL_HH

#include "DDS_buildCommands.h"

class DDS_hl{
	public:
		DDS DDS0;
		DDS DDS1;
		DDS DDS2;
		DDDS_hl(int, int, int);
		void configureBoards();
		void updateFrequencies(float, float, float, float, float, float);
		void updateFrequencies(float, float, float,float, float, float, uint32_t);
		void updateAmplitudes(uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t);
		void sleepAll();
		void wakeAll();
		void powerDownAll();
		void serialGo();
		void waitForSerialDone();
		void Read32(uint8_t, uint32_t*, uint32_t*, uint32_t*,bool);
		void setChipSelect(bool);

		virtual ~DDS_hl();

	private:
};
#endif

