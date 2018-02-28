/**
 * Author: Carlos R. Davila
 * SJSU ID: 010779067
 * Original Code: https://sysprogs.com/VisualKernel/tutorials/raspberry/leddriver/
 * Last Modified: Feb-23-2-18
 */
#include "led.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Carlos R. Davila");

/* --- SYSFS --- */
static ssize_t led_show(struct kobject *kobj, 
						struct kobj_attribute *attr, char *buf){
  return sprintf(buf, "%d\n", led_status);
}

static ssize_t led_store(struct kobject *kobj, 
						 struct kobj_attribute *attr, 
						 const char *buf, size_t count) {
  sscanf(buf, "%du", &led_status);
  
  if(led_status){
	  SetGPIOOutputValue(LedGpioPin, true); // Turn LED on.
  } else {
	  SetGPIOOutputValue(LedGpioPin, false); // Turn LED off.
  }
  return count;
}

// Create device_attribute struct
static struct kobj_attribute led_attr = __ATTR(led_pwr, 0660, 
											   led_show, led_store);
  
/* --- GPIO FUNCTIONS --- */

static struct GpioRegisters *s_pGpioRegisters;

static void SetGPIOFunction(int GPIO, int functionCode) {
  int registerIndex = GPIO / 10;    // Select the correct GPFSEL to change.
  int bit = (GPIO % 10) * 3;		// Set the bit offset of the correct GPIO.

  unsigned oldValue = s_pGpioRegisters->GPFSEL[registerIndex];
  unsigned mask = 0b111 << bit;
  s_pGpioRegisters->GPFSEL[registerIndex] = (oldValue & ~mask) | ((functionCode << bit) & mask);
}

static void SetGPIOOutputValue(int GPIO, bool outputValue) {
  if (outputValue) {
    s_pGpioRegisters->GPSET[GPIO / 32] = (1 << (GPIO % 32));  // Set pin to high.
  } else {
    s_pGpioRegisters->GPCLR[GPIO / 32] = (1 << (GPIO % 32));  // Set pin to low.
  }
}

static int __init led_init(void) {
  int error = 0;
  s_pGpioRegisters = (struct GpioRegisters *)ioremap( 
    GPIO_BASE, sizeof(struct GpioRegisters));         // Map physical address to virtual address space.
  SetGPIOFunction(LedGpioPin, 0b001);  	              // Configure the pin as output.
  
  led_kobj = kobject_create_and_add("led_ctrl_pwr", kernel_kobj);
  if(!led_kobj) {
	  return -ENOMEM;
  }
  
  error = sysfs_create_file(led_kobj, &led_attr.attr);
  if(error) {
	  printk(KERN_INFO "Failed to register sysfs for LED");
  }
  return error;
}

static void __exit led_exit(void) {
  SetGPIOFunction(LedGpioPin, 0);                     // Configure the pin as input.
  iounmap(s_pGpioRegisters);                          // Unmap physical to virtual addresses.
  kobject_put(led_kobj);
}

module_init(led_init);
module_exit(led_exit);
