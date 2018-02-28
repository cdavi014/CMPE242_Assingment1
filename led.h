/**
 * Author: Carlos R. Davila
 * SJSU ID: 010779067
 * Last Modified: Feb-22-2-18
 */
#include <asm/io.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/mm.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>

#define PERIPH_BASE 0x3F000000              // Base address of peripheral address range
#define GPIO_BASE (PERIPH_BASE + 0x200000)  // Base address of GPIO pins

static const int LedGpioPin = 21;           // Pin LED is connected to
static int led_status = 0;					// 0-off, 1-on
static struct kobject *led_kobj;

/**
* Set a contiguous range of memory for the GPIO registers.
* Space for 6 GPFSELn registers (0-5).
* Space for reserved address 0x7E20 0018 (Bus Address) or 0x3F20 0018 (Physical Address).
* Space for 2 GPSET registers.
* Space for reserved address 0x7E20 0024 (Bus Address) or 0x3F20 0024 (Physical Address).
* Space for 2 GPCLR registers.
*/
struct GpioRegisters {
  volatile uint32_t GPFSEL[6];
  volatile uint32_t Reserved1;
  volatile uint32_t GPSET[2];
  volatile uint32_t Reserved2;
  volatile uint32_t GPCLR[2];
};

/*
 * Function callback for sysfs interface that will take care of powering 
 * on or off the LED
 */
static ssize_t led_show(struct kobject *kobj, 
						struct kobj_attribute *attr, char *buf);
static ssize_t led_store(struct kobject *kobj, 
						 struct kobj_attribute *attr, 
						 const char *buf, size_t count);

/* 
 * Function to set the function of an individual pin. (input, output...etc).
 * int GPIO:         GPIO pin to set the funtion for.
 * int functionCode: The code to set for the GPIO pin.
 */
static void SetGPIOFunction(int GPIO, int functionCode);

/* 
 * Function to set the output value of an individual pin. (0 or 1, low or high).
 * int GPIO:         GPIO pin to set the funtion for.
 * int outputValue:  The output to set the pin to.
 */
static void SetGPIOOutputValue(int GPIO, bool outputValue);
