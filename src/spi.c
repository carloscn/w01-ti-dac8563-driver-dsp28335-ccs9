/*
 * spi.c
 *
 *  Created on: 2019Äê5ÔÂ5ÈÕ
 *      Author: lifim
 */

#include "spi.h"


inline void _spi_pin_init()
{
	// please set up a function in main function that is init your gpio;
	EALLOW;
	GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 0;
	GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 0;
	GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 0;
	GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 0;
	GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 0;

	GpioCtrlRegs.GPADIR.bit.GPIO0 = 1;
	GpioCtrlRegs.GPADIR.bit.GPIO1 = 1;
	GpioCtrlRegs.GPADIR.bit.GPIO2 = 1;
	GpioCtrlRegs.GPADIR.bit.GPIO3 = 1;
	GpioCtrlRegs.GPADIR.bit.GPIO4 = 0;
	EDIS;
}

static bool spi_read_n_bytes(SPI_BUS* bus, uint8_t* ret_bytes, uint32_t n)
{
	bool ret;


	return ret;
}

static bool spi_read_byte(SPI_BUS* bus, uint8_t* ret_byte)
{
	bool ret;
	uint8_t i;
	uint8_t byte = 0;

	CLR_CS(bus);
	switch( bus->info.mode ) {
	case SPI_MODE_CPOL_H_CPHA_FIRST:
		CLR_CLK(bus);
		for (i = 0; i < 8; i++) {
			byte<<=1;
			CLR_CLK(bus);
			byte += RD_MISO(bus);
			SET_CLK(bus);
		}
		CLR_CLK(bus);
		break;
	case SPI_MODE_CPOL_H_CPHA_SECOND:
		CLR_CLK(bus);
		for (i = 0; i < 8; i++) {
			byte<<=1;
			SET_CLK(bus);
			CLR_CLK(bus);
			byte += RD_MISO(bus);

		}
		break;
	case SPI_MODE_CPOL_L_CPHA_FIRST:
		SET_CLK(bus);
		for (i = 0; i < 8; i++) {
			byte<<=1;
			CLR_CLK(bus);
			byte += RD_MISO(bus);
			SET_CLK(bus);
		}
		SET_CLK(bus);
		break;
	case SPI_MODE_CPOL_L_CPHA_SECOND:
		SET_CLK(bus);
		for (i = 0; i < 8; i++) {
			byte<<=1;
			CLR_CLK(bus);
			SET_CLK(bus);
			byte += RD_MISO(bus);
		}
		SET_CLK(bus);
		break;
	}
	SET_CS(bus);
	*ret_byte = byte;
	ret = true;

	return ret;

}

static void spi_write_n_bytes(SPI_BUS* bus, uint8_t* bytes, uint32_t n)
{

}

static void spi_write_byte(SPI_BUS* bus, uint8_t byte)
{
	uint8_t i;

	CLR_CS(bus);
	switch( bus->info.mode ) {
	case SPI_MODE_CPOL_H_CPHA_FIRST:
		CLR_CLK(bus);
		for (i = 0; i < 8; i++) {
			CLR_CLK(bus);
			if ( byte&0x80 ) {
				SET_MOSI(bus);
			}
			else {
				CLR_MOSI(bus);
			}
			SET_CLK(bus);
			byte<<=1;
		}
		CLR_CLK(bus);
		break;
	case SPI_MODE_CPOL_H_CPHA_SECOND:
		CLR_CLK(bus);
		for (i = 0; i < 8; i++) {
			SET_CLK(bus);
			CLR_CLK(bus);
			if ( byte&0x80 ) {
				SET_MOSI(bus);
			}
			else
				CLR_MOSI(bus);
			byte<<=1;
		}
		break;
	case SPI_MODE_CPOL_L_CPHA_FIRST:
		SET_CLK(bus);
		for (i = 0; i < 8; i++) {
			CLR_CLK(bus);
			if ( byte&0x80 ) {
				SET_MOSI(bus);
			}
			else {
				CLR_MOSI(bus);
			}
			SET_CLK(bus);
			byte<<=1;
		}
		SET_CLK(bus);
		break;
	case SPI_MODE_CPOL_L_CPHA_SECOND:
		SET_CLK(bus);
		for (i = 0; i < 8; i++) {
			CLR_CLK(bus);
			SET_CLK(bus);
			if ( byte&0x80 ) {
				SET_MOSI(bus);
			}
			else{
				CLR_MOSI(bus);
			}
			byte<<=1;
		}
		SET_CLK(bus);
		break;
	}
	SET_CS(bus);
}

static void spi_reset(SPI_BUS* bus, bool state, uint32_t n_us)
{
	if (state == true) {
		CLR_RST(bus);
		SET_RST(bus);
		DELAY_US(n_us);
		CLR_RST(bus);
	}
	else {
		SET_RST(bus);
		CLR_RST(bus);
		DELAY_US(n_us);
		SET_RST(bus);
	}
}

static bool spi_read_n_bits(SPI_BUS* bus, uint64_t* ret_data, uint8_t n)
{
	bool ret;
	uint8_t i;
	uint64_t data = 0;

	/* over the range. */
	if (n > 64 && n == 0) {
		ret = false;
		return ret;
	}
	CLR_CS(bus);
	switch( bus->info.mode ) {
	case SPI_MODE_CPOL_H_CPHA_FIRST:
		CLR_CLK(bus);
		for (i = 0; i < n; i++) {
			data<<=1;
			CLR_CLK(bus);
			data += RD_MISO(bus);
			SET_CLK(bus);
		}
		CLR_CLK(bus);
		break;
	case SPI_MODE_CPOL_H_CPHA_SECOND:
		CLR_CLK(bus);
		for (i = 0; i < n; i++) {
			data<<=1;
			SET_CLK(bus);
			CLR_CLK(bus);
			data += RD_MISO(bus);

		}
		break;
	case SPI_MODE_CPOL_L_CPHA_FIRST:
		SET_CLK(bus);
		for (i = 0; i < n; i++) {
			data<<=1;
			CLR_CLK(bus);
			data += RD_MISO(bus);
			SET_CLK(bus);
		}
		SET_CLK(bus);
		break;
	case SPI_MODE_CPOL_L_CPHA_SECOND:
		SET_CLK(bus);
		for (i = 0; i < n; i++) {
			data<<=1;
			CLR_CLK(bus);
			SET_CLK(bus);
			data += RD_MISO(bus);
		}
		SET_CLK(bus);
		break;
	}
	SET_CS(bus);
	*ret_data = data;
	ret = true;

	return ret;
}

static bool spi_write_n_bits(SPI_BUS* bus, uint64_t ret_data, uint8_t n)
{
	uint8_t i;

	CLR_CS(bus);
	switch( bus->info.mode ) {
	case SPI_MODE_CPOL_H_CPHA_FIRST:
		CLR_CLK(bus);
		for (i = 0; i < n; i++) {
			CLR_CLK(bus);
			if ( ret_data&(1<<24) ) {
				SET_MOSI(bus);
			}
			else {
				CLR_MOSI(bus);
			}
			SET_CLK(bus);
			ret_data<<=1;
		}
		CLR_CLK(bus);
		break;
	case SPI_MODE_CPOL_H_CPHA_SECOND:
		CLR_CLK(bus);
		for (i = 0; i < n; i++) {
			SET_CLK(bus);
			CLR_CLK(bus);
			if ( ret_data&(1<<24) ) {
				SET_MOSI(bus);
			}
			else
				CLR_MOSI(bus);
			ret_data<<=1;
		}
		break;
	case SPI_MODE_CPOL_L_CPHA_FIRST:
		SET_CLK(bus);
		for (i = 0; i < n; i++) {
			CLR_CLK(bus);
			if ( ret_data&(1<<24) ) {
				SET_MOSI(bus);
			}
			else {
				CLR_MOSI(bus);
			}
			SET_CLK(bus);
			ret_data<<=1;
		}
		SET_CLK(bus);
		break;
	case SPI_MODE_CPOL_L_CPHA_SECOND:
		SET_CLK(bus);
		for (i = 0; i < n; i++) {
			CLR_CLK(bus);
			SET_CLK(bus);
			if ( ret_data&(1<<24) ) {
				SET_MOSI(bus);
			}
			else{
				CLR_MOSI(bus);
			}
			ret_data<<=1;
		}
		SET_CLK(bus);
		break;
	}
	SET_CS(bus);
}

static void spi_init(SPI_BUS* bus)
{
	_spi_pin_init();
}
/* 10kHz < clk < 1MHz
 * 0 is full io switch speed no delay.
 * */
SPI_BUS* spi_new_bus(uint32_t clk, SPI_MODE mode, uint8_t* name, uint8_t id)
{
	SPI_BUS *dev = (SPI_BUS*)malloc(sizeof(SPI_BUS));
	if ( clk != 0&&clk < 10000) {
		clk = 10000;
		dev->info.clk = clk;
		dev->info.speed = 1000000/clk;
	}else if (clk > 1000000) {
		clk = 1000000;
		dev->info.clk = clk;
		dev->info.speed = 1000000/clk;
	}else if( clk == 0) {
		clk = 0;
		dev->info.clk = clk;
		dev->info.speed = 0;
	}
	dev->info.name = (uint8_t*)malloc(sizeof(uint8_t) * 20);
	strncpy(dev->info.name, name, 10);
	dev->info.id = id;
	dev->info.mode = mode;
	dev->init = &spi_init;
	dev->reset = &spi_reset;
	dev->read_n_bytes = &spi_read_n_bytes;
	dev->write_n_bytes = &spi_write_n_bytes;
	dev->read_byte  = &spi_read_byte;
	dev->write_byte = &spi_write_byte;
	dev->read_n_bits = &spi_read_n_bits;
	dev->write_n_bits = &spi_write_n_bits;
	dev->init(dev);

	return dev;
}
