/**
 * Author: Carlos R. Davila
 * SJSU ID: 010779067
 * Original Code: https://sysprogs.com/VisualKernel/tutorials/raspberry/leddriver/
 * Last Modified: Feb-23-2-18
 */
#include "led.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Carlos R. Davila");

struct GpioRegisters *s_pGpioRegisters;

static void SetGPIOFunction(int GPIO, int functionCode) {
  int registerIndex = GPIO / 10;    // Select the correct GPFSEL to change.
  int bit = (GPIO % 10) * 3;	      // Set the bit offset of the correct GPIO.

  unsigned oldValue = s_pGpioRegisters->GPFSEL[registerIndex];
  unsigned mask = 0b111 << bit;
  s_pGpioRegisters->GPFSEL[registerIndex] = (oldValue & ~mask) | ((functionCode << bit) & mask);
}

static void SetGPIOOutputValue(int GPIO, bool outputValue) {
  if (outputValue)
    s_pGpioRegisters->GPSET[GPIO / 32] = (1 << (GPIO % 32));  // Set pin to high.
  else
    s_pGpioRegisters->GPCLR[GPIO / 32] = (1 << (GPIO % 32));  // Set pin to low.
}

static int __init led_init(void) {
  s_pGpioRegisters = (struct GpioRegisters *)ioremap( 
    GPIO_BASE, sizeof(struct GpioRegisters));         // Map physical address to virtual address space.
  SetGPIOFunction(LedGpioPin, 0b001);  	              // Configure the pin as output.
  SetGPIOOutputValue(LedGpioPin, true);               // Turn LED on.
  return 0;
}

static void __exit led_exit(void) {
  SetGPIOFunction(LedGpioPin, 0);  		                // Configure the pin as input.
  iounmap(s_pGpioRegisters);                          // Unmap physical to virtual addresses.
}

module_init(led_init);
module_exit(led_exit);
