#ifndef GPIO_HH
#define GPIO_HH

#include <stdint.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

#define GPIO_PORT 4

class GPIO {
	public:
		GPIO(); 						//constructor with default UIO
		void writeGPIO(int, uint32_t); 	//write to GPIO register
		uint32_t readGPIO(int);        	//read from GPIO register
		
		virtual ~GPIO(); 				//destructor

	private:
		struct HW{
			uint32_t reg[3];
		};
		volatile HW *AXI;
};
#endif
