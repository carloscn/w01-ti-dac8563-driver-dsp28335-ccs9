/*
 * spi.h
 *
 *  Created on: 2019Äê5ÔÂ5ÈÕ
 *      Author: lifim
 */

#ifndef INC_SPI_H_
#define INC_SPI_H_

#include <DSP28x_Project.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#ifndef UINT8_DEF
#define UINT8_DEF
typedef unsigned char uint8_t;
#endif

#ifndef INT8_DEF
#define INT8_DEF
typedef char int8_t;
#endif

#define				SET_CS(self)			GpioDataRegs.GPADAT.bit.GPIO0 = 1;DELAY_US(self->info.speed)
#define				CLR_CS(self)			GpioDataRegs.GPADAT.bit.GPIO0 = 0;DELAY_US(self->info.speed)
#define				SET_MOSI(self)			GpioDataRegs.GPADAT.bit.GPIO1 = 1;DELAY_US(self->info.speed)
#define				CLR_MOSI(self)			GpioDataRegs.GPADAT.bit.GPIO1 = 0;DELAY_US(self->info.speed)
#define				SET_CLK(self)			GpioDataRegs.GPADAT.bit.GPIO2 = 1;DELAY_US(self->info.speed)
#define				CLR_CLK(self)			GpioDataRegs.GPADAT.bit.GPIO2 = 0;DELAY_US(self->info.speed)
#define				SET_RST(self)			GpioDataRegs.GPADAT.bit.GPIO3 = 1;DELAY_US(self->info.speed)
#define				CLR_RST(self)			GpioDataRegs.GPADAT.bit.GPIO3 = 0;DELAY_US(self->info.speed)
#define				RD_MISO(self)			GpioDataRegs.GPADAT.bit.GPIO4
#define				SPI_CLK_FULL_SPEED		0

typedef struct spi_t SPI_BUS;

typedef enum spi_mode_t {

	SPI_MODE_CPOL_H_CPHA_FIRST = 0,
	SPI_MODE_CPOL_H_CPHA_SECOND,
	SPI_MODE_CPOL_L_CPHA_FIRST,
	SPI_MODE_CPOL_L_CPHA_SECOND

 } SPI_MODE;


struct spi_info_t
{
	uint8_t* name;
	uint32_t clk;
	uint8_t id;
	SPI_MODE mode;
	uint32_t speed;

};
/* if the pin number is 0, the spi bus use microcontroller spi interface,
 * else use gpio simulate spi time-sequence.
 * 4-wire spi.
 * */
struct spi_hw_t
{
	uint32_t cs_io_num;
	uint32_t miso_io_num;
	uint32_t clk_io_num;
	uint32_t mosi_io_num;
	uint32_t reset_io_num;
};

struct spi_t
{
	struct spi_info_t info;
	struct spi_hw_t hw;
	SPI_BUS *self;
	uint32_t mem[10];

	/* Init spi bus include gpio, microcontroller spi interface.
	 * #para:
	 * @1, SPI_BUS handler point;
	 * */
	void (*init)(SPI_BUS* bus);

	/* Reset spi bus include gpio, microcontroller spi interface.
	 * #para:
	 * @1, SPI_BUS handler point;
	 * @2, state: true: raising-edge reset; false: fall-edge reset.
	 * @3, n_us: hold level n_us.
	 * */
	void (*reset)(SPI_BUS* bus, bool state, uint32_t n_us);
	/* Reset spi bus.
	 * #para:
	 * @1, SPI_BUS handler point;
	 * */

	void (*write_byte)(SPI_BUS* bus, uint8_t byte);

	/* Writen bytes to spi bus.
	 * #para:
	 * @1, SPI_BUS handler point;
	 * @2, Write 8 bits bytes data array.
	 * @3, transmit bytes number.
	 * */
	void (*write_n_bytes)(SPI_BUS* bus, uint8_t* bytes, uint32_t n);

	/* Read a byte from spi bus.
	 * #para:
	 * @1, SPI_BUS handler point;
	 * @2, a point, spi return byte;
	 * #return:
	 * @1, read byte succuss? yes, return true; no, return false;
	 * */
	bool (*read_byte)(SPI_BUS* bus, uint8_t* ret_byte);

	/* Read n byte from spi bus.
	 * #para:
	 * @1, SPI_BUS handler point;
	 * @2, a point, spi return n bits;
	 * @3, spi return n bits;
	 * #return:
	 * @1, read bits all succuss? yes, return true; no, return false;
	 * */
	bool (*read_n_bytes)(SPI_BUS* bus, uint8_t* ret_byte, uint8_t n);

	/* Read n bits from spi bus.
	 * #para:
	 * @1, SPI_BUS handler point;
	 * @2, a point, spi return n bits;
	 * @3, spi return n bits;
	 * #return:
	 * @1, read bits all succuss? yes, return true; no, return false;
	 * */
	bool (*read_n_bits)(SPI_BUS* bus, uint64_t* ret_data, uint8_t n);

	/* write n bits from spi bus.
	 * #para:
	 * @1, SPI_BUS handler point;
	 * @2, write data top 64bits
	 * @3, spi return n bits;
	 * #return:
	 * @1, read bits all succuss? yes, return true; no, return false;
	 * */
	bool (*write_n_bits)(SPI_BUS* bus, uint64_t ret_data, uint8_t n);



};

extern SPI_BUS* spi_new_bus(uint32_t clk, SPI_MODE mode, uint8_t* name, uint8_t id);


#endif /* INC_SPI_H_ */
