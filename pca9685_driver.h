/**
 * @file PCA9685 Driver
 * @date Created: Jan 18, 2025
 * @author Author: Simar Singh Ubhi
 * @version 1.0.0
 */

#ifndef INC_PCA9685_DRIVER_H_
#define INC_PCA9685_DRIVER_H_

#define PCA9685_ADDRESS (0x40 << 1)

#include "stm32f4xx_hal.h"  // Include the HAL I2C header to define I2C_HandleTypeDef
#include <math.h>

#endif /* INC_PCA9685_DRIVER_H_ */

void pca9685_init(I2C_HandleTypeDef *hi2c, uint16_t frequency);

void pca9685_setbit(uint8_t register_addr, uint8_t bit_pos, uint8_t bit_value);

void pca9685_setfrequency(uint16_t frequency);

void pca9685_setpwm(uint8_t channel, uint16_t on_time, uint16_t off_time);

void pca9685_setservo_angle(uint8_t servo_channel, float angle);

