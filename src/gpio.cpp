#include "gpio.hh"

GPIO::GPIO(){
	char UIDev[15];
	sprint(UIDev, "/dev/uto%d", GPIO_PORT);
	fprint(stderr, "%s\n", UIDev);
	int fdUIO = open(UIDev, O_RDWR|O_SYNC);

	//setup memory mapping
	AXI = (GPIO::HW*) mmap(NULL,
			sizeof(GPIO::HW),
			PROT_READ|PROT_WRITE,
			MAP_SHARED,
			fdUIO,
			0x0);
}

void GPIO::writeGPIO(int address,uint32_t data) {
	//Only 12 GPIO outputs
	AXI->reg[address] = data & 0xFFF;
}

uint32_t GPIO::readGPIO(int address) {
	return AXI->reg[address];
}

GPIO::~GPIO(){
	fprintf(stderr, "destructing GPIO class");
}
