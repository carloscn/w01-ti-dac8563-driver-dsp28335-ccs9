/*
 * dac8563.h
 *
 *  Created on: 2019Äê5ÔÂ5ÈÕ
 *      Author: lifim
 */

#ifndef INC_DAC8563_H_
#define INC_DAC8563_H_


#include <DSP28x_Project.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "spi.h"

#ifndef UINT8_DEF
#define UINT8_DEF
typedef unsigned char uint8_t;
#endif

#ifndef INT8_DEF
#define INT8_DEF
typedef char int8_t;
#endif

typedef struct dac8563_t DAC8563;

struct dac8563_info_t
{

};

struct dac8563_hw_t
{
	SPI_BUS *spi;
};

struct dac8563_t
{
	struct dac8563_t *self;
	struct dac8563_info_t *info;
	struct dac8563_hw_t *hw;

	void (*init)(DAC8563* dev);
	void (*reset)(DAC8563* dev);
	void (*write_cmd)(DAC8563 *dev, uint32_t cmd);
	void (*write_data)(DAC8563 *dev, uint8_t channel, uint16_t data);

};

extern DAC8563* dac8563_new_dev();

#endif /* INC_DAC8563_H_ */
