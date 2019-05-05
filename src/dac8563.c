/*
 * dac8563.c
 *
 *  Created on: 2019年5月5日
 *      Author: lifim
 */


#include "dac8563.h"

void dac8563_write_cmd(DAC8563* dev, uint32_t cmd)
{
	dev->hw->spi->write_n_bits(dev->hw->spi, (uint64_t)cmd, 24);
}

void dac8563_write_data(DAC8563* dev, uint8_t ch, uint16_t data)
{
	if (ch == 0) {
		/* Write to DAC-A input register and update DAC-A; */
		dev->hw->spi->write_n_bits(dev->hw->spi, (uint64_t)(3 << 19) | (uint64_t)(0 << 16) | (uint64_t)(data << 0), 24);
	}
	else if (ch == 1) {
		/* Write to DAC-B input register and update DAC-A; */
		dev->hw->spi->write_n_bits(dev->hw->spi, (uint64_t)(3 << 19) | (uint64_t)(1 << 16) | (uint64_t)(data << 0), 24);
	}
}

void dac8563_init(DAC8563* dev)
{
	/* Power up DAC-A and DAC-B */
	dev->write_cmd(dev, (uint64_t)((uint64_t)4 << 19) | (uint64_t)(0 << 16) | (uint64_t)(3 << 0));
	/* Power up DAC-A and DAC-B */
	dev->write_cmd(dev, (uint64_t)((uint64_t)4 << 19) | (uint64_t)(0 << 16) | (uint64_t)(3 << 0));
	/* LDAC pin inactive for DAC-B and DAC-A  不使用LDAC引脚更新数据 */
	dev->write_cmd(dev, (uint64_t)((uint64_t)6 << 19) | (uint64_t)(0 << 16) | (uint64_t)(3 << 0));
	/* 复位2个DAC到中间值, 输出2.5V */
	dev->write_data(dev, 0, 32767);
	dev->write_data(dev, 1, 32767);
	/* 选择内部参考并复位2个DAC的增益=2 （复位时，内部参考是禁止的) */
	dev->write_cmd(dev, (uint64_t)((uint64_t)7 << 19) | (uint64_t)(0 << 16) | (uint64_t)(1 << 0));
}

void dac8563_reset(DAC8563* dev)
{
	dev->hw->spi->reset(dev->hw->spi, 0, 50);
}

DAC8563* dac8563_new_dev()
{
	DAC8563 *dev = (DAC8563*)malloc(sizeof(DAC8563));

	dev->hw->spi = spi_new_bus(SPI_CLK_FULL_SPEED, SPI_MODE_CPOL_H_CPHA_SECOND, "DAC8563 DEV", 0);
	dev->init = &dac8563_init;
	dev->reset = &dac8563_reset;
	dev->write_cmd  = &dac8563_write_cmd;
	dev->write_data = &dac8563_write_data;
	dev->init(dev);

	return dev;
}
