/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/zephyr.h>
#include <drivers/gpio.h>

#define USING_POLLING    0
#define USING_INTERRUPT  1

#define DEMO_METHOD USING_INTERRUPT

#if (DEMO_METHOD == USING_POLLING)
#define DEMO_NAME "polling"
#else
#define DEMO_NAME "interrupt"
#endif

#define SLEEP_TIME_MS 1000

struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_NODELABEL(led0), gpios); // Get the information of defined led pin
struct gpio_dt_spec button = GPIO_DT_SPEC_GET(DT_NODELABEL(button0), gpios); // Get the information of defined button pin

#if (DEMO_METHOD == USING_INTERRUPT)
static struct gpio_callback pin_cb_data;

void pin_isr(const struct device *dev, struct gpio_callback *cb, gpio_port_pins_t pins)
{
	/* The callback function is called when an interrupt is triggered */
	gpio_pin_toggle_dt(&led);
}
#endif

void main(void)
{
	uint8_t val = 0;

	printk("Demo using button to toggle led state with %s on %s\n", DEMO_NAME, CONFIG_BOARD);

	/* Init gpio */
	if (!device_is_ready(led.port)) { // Check if the device port is ready or not
		return;
	}

	gpio_pin_configure_dt(&led, GPIO_OUTPUT); // Configure the io pin as output
	gpio_pin_configure_dt(&led, GPIO_OUTPUT | GPIO_ACTIVE_LOW); // Configure the io pin as output what is active low

	gpio_pin_configure_dt(&button, GPIO_INPUT); // Configure the io pin as input
#if (DEMO_METHOD == USING_INTERRUPT)
	gpio_pin_interrupt_configure_dt(&button,GPIO_INT_EDGE_TO_ACTIVE); // Configure interrupt on the pin
	gpio_init_callback(&pin_cb_data, pin_isr, BIT(button.pin));
	gpio_add_callback(button.port, &pin_cb_data);
#endif

	/* Write to the output pin */ 
	gpio_pin_set_dt(&led, 1); 

	/* Read from the input pin */
	val = gpio_pin_get_dt(&button); // for polling method
	while(1) {

#if (DEMO_METHOD == USING_POLLING)	
		if(gpio_pin_get_dt(&button) ==  1) { // for polling method
			gpio_pin_toggle_dt(&led);
		}
#endif

        k_msleep(SLEEP_TIME_MS);
	}	
}

