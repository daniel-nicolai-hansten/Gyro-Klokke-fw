/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/gpio.h>

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   2000
#define STEP_DELAY   1000


#define MOT1A1 0
#define MOT1A2 1
#define MOT1B1 2
#define MOT1B2 3

#define MOT2A1 6
#define MOT2A2 7
#define MOT2B1 13
#define MOT2B2 14

#ifndef FLAGS
#define FLAGS	0
#endif
#define STEPSN 8
int steps[STEPSN]  = {0b0101,0b0001,0b1001,0b1000,0b01010,0b0010,0b0110,0b0100};
#define STEPS 9
//#define STEPSN 4
//unsigned int steps[STEPSN]  = {0b0100,0b0010,0b1000,0b0001};


void stepper_int(void){
	struct device *dev;
	dev = device_get_binding("GPIOA");
	if (dev == NULL) {
		return;
	}

	gpio_pin_configure(dev, MOT1A1, GPIO_OUTPUT_ACTIVE | FLAGS);
	gpio_pin_configure(dev, MOT1A2, GPIO_OUTPUT_ACTIVE | FLAGS);
	gpio_pin_configure(dev, MOT1B1, GPIO_OUTPUT_ACTIVE | FLAGS);
	gpio_pin_configure(dev, MOT1B2, GPIO_OUTPUT_ACTIVE | FLAGS);
	
	gpio_pin_configure(dev, MOT2A1, GPIO_OUTPUT_ACTIVE | FLAGS);
	gpio_pin_configure(dev, MOT2A2, GPIO_OUTPUT_ACTIVE | FLAGS);
	gpio_pin_configure(dev, MOT2B1, GPIO_OUTPUT_ACTIVE | FLAGS);
	gpio_pin_configure(dev, MOT2B2, GPIO_OUTPUT_ACTIVE | FLAGS);
}
void stepper1_move(uint8_t motor, unsigned int step){
	struct device *dev;
	dev = device_get_binding("GPIOA");
	if (dev == NULL) {
		return;
	}
	if (motor == 1){
		int mot1a1 = (0b0001 & step);
		int mot1a2 = (0b0010 & step) >> 1;
		int mot1b1 = (0b0100 & step) >> 2;
		int mot1b2 = (0b1000 & step) >> 3;
		gpio_pin_set(dev, MOT1A1, mot1a1);
		gpio_pin_set(dev, MOT1A2, mot1a2);
		gpio_pin_set(dev, MOT1B1, mot1b1);
		gpio_pin_set(dev, MOT1B2, mot1b2);
	}
	if (motor == 2){
		int mot2a1 = (0b0001 & step);
		int mot2a2 = (0b0010 & step) >> 1;
		int mot2b1 = (0b0100 & step) >> 2;
		int mot2b2 = (0b1000 & step) >> 3;
		gpio_port_set(dev, MOT1A1, mot1a1);
		gpio_pin_set(dev, MOT1A2, mot1a2);
		gpio_pin_set(dev, MOT1B1, mot1b1);
		gpio_pin_set(dev, MOT1B2, mot1b2);
	}
}

void main(void)
{

	unsigned int step;
	int ret;
printk("hello");
	stepper_int();
	if (ret < 0) {
		return;
	}

	while (1) {
		for ( int y = 0; y < STEPS; y++ ){
		for ( int x = 0; x < STEPSN; x++ ){
		//for ( int x = (STEPSN -1); x < 0; x-- ){
		step =	steps[x];
		stepper1_move(step);
		printk("%u   %u \r",step, x);
		k_msleep(STEP_DELAY);
		}
	}
		//k_msleep(SLEEP_TIME_MS);
		for ( int y = 0; y < STEPS; y++ ){
		//for ( int x = 0; x < STEPSN; x++ ){
		for ( int x = (STEPSN - 1); x >= 0; x-- ){
		step =	steps[x];
		stepper1_move(step);
		printk("%u   %u \r",step, x);
		k_msleep(STEP_DELAY);
		}
	}
		//k_msleep(SLEEP_TIME_MS);
	}
}
