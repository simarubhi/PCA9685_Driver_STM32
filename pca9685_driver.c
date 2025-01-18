/**
 * @file PCA9685 Driver
 * @date Created: Jan 18, 2025
 * @author Author: Simar Singh Ubhi
 * @version 1.0.0
 */

#include "pca9685_driver.h"

//**********************Private Variables**********************//
static I2C_HandleTypeDef *phi2c1 = NULL;

//**********************Public API Functions**********************//
void pca9685_init(I2C_HandleTypeDef *hi2c, uint16_t frequency)
{
	phi2c1 = hi2c; // Assigning handle

	pca9685_setfrequency(frequency);
	pca9685_setbit(0x00, 5, 1); // Auto increment bit
}

void pca9685_setbit(uint8_t register_addr, uint8_t bit_pos, uint8_t bit_value)
{
	uint8_t register_value;

	HAL_I2C_Mem_Read(phi2c1, PCA9685_ADDRESS, register_addr, 1, &register_value, 1, HAL_MAX_DELAY);

	if (bit_value == 1)
	{
		register_value |= (1 << bit_pos);
	}
	else
	{
		register_value &= ~(1 << bit_pos);
	}

	HAL_I2C_Mem_Write(phi2c1, PCA9685_ADDRESS, register_addr, 1, &register_value, 1, HAL_MAX_DELAY);
	HAL_Delay(2); // Needs at least 500us according to data sheet
}

void pca9685_setfrequency(uint16_t frequency)
{
	uint8_t prescaler; // 8 bit register, can have values between [3, 255]

	// Plugging frequency into formula provided in data sheet, frequency can only be between [24, 1526]
	if (frequency >= 1526)
	{
		prescaler = 3;
	}
	else if (frequency <= 24)
	{
		prescaler = 255;
	}
	else
	{
		prescaler =  round(25000000 / (4096 * frequency)) - 1; // Formula given in data sheet
//		prescaler = 25000000 / (4096 * frequency);
	}

	// MAKE SLEEPING ETC. INTO SEPERATRE FUNCTIONS ALL MODE1 *****************************************************************

	pca9685_setbit(0x00, 4, 1); // Put to sleep
	HAL_I2C_Mem_Write(phi2c1, PCA9685_ADDRESS, 0xfe, 1, &prescaler, 1, HAL_MAX_DELAY);
	pca9685_setbit(0x00, 4, 0); // Wake up
	pca9685_setbit(0x00, 7, 1); // Restart
}

void pca9685_setpwm(uint8_t channel, uint16_t on_time, uint16_t off_time)
{
	uint8_t inital_led_register;
	uint8_t transfer_data[4];

	inital_led_register = 0x06 + (4 * channel);

	transfer_data[0] = on_time & 0x00ff;
	transfer_data[1] = on_time >> 8;
	transfer_data[2] = off_time & 0x00ff;
	transfer_data[3] = off_time >> 8;

	HAL_I2C_Mem_Write(phi2c1, PCA9685_ADDRESS, inital_led_register, 1, transfer_data, 4, HAL_MAX_DELAY);
}

void pca9685_setservo_angle(uint8_t servo_channel, float angle)
{
	float pwm_value;
	// based on data sheet, 0 degrees (.5ms) => pwm_value = (102.4) | 180 degrees (2.5ms) => pwm_value = (511.9)
	pwm_value = (angle * (511.9 - 102.4) / 180.0) + 102.4;
	pca9685_setpwm(servo_channel, 0, (uint16_t)pwm_value);
}
