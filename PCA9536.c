// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// https://www.controleverything.com/content/Digital-IO?sku=PCA9536_I2CBZ#tabs-0-product_tabset-2

#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>

void main() 
{
	// Create I2C bus
	int file;
	char *bus = "/dev/i2c-1";
	if ((file = open(bus, O_RDWR)) < 0) 
	{
		printf("Failed to open the bus. \n");
		exit(1);
	}
	// Get I2C device, PCA9536_BZ I2C address is 0x41(65)
	ioctl(file, I2C_SLAVE, 0x41);

	// Select configuration register(0x03)
	// All pins configured as inputs(0xFF)
	char config[2] = {0};
	config[0] = 0x03;
	config[1] = 0xFF;
	if(write(file, config, 2) != 2)
	{
		printf("Error : Input/Output error \n");
		exit(1);
	}

	int i;
	for(i = 0; i < 4; i++)
	{
		// Select output port register(0x01)
		// Logic HIGH on each pin
		config[0]= 0x01;
		config[1]= 0xF0 | (1 << i);
		write(file, config, 2);

	// Checking status of all GPIO pins of port
	char reg[1] = {0x00};
	write(file, reg, 1);
	char data[1] = {0};
	read(file, data, 1);
	char status = data[0];
	char data1 = 0x01;
	int i;
	for(i = 0; i < 4; i++)
	{
		char state = (status & data1);
		if(state == data1)
		{
			printf("GPIO pin %d of port is HIGH \n", i);
		}
		else
		{
			printf("GPIO pin %d of port is LOW \n", i);
		}
		data1 = (data1 << 1);
		sleep(1);
	}
}
