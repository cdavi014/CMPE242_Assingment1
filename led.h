/**
 * Author: Carlos R. Davila
 * SJSU ID: 010779067
 * Last Modified: Feb-22-2-18
 */
#include <asm/io.h>
#include <linux/timer.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/mm.h>
#include <linux/init.h>
#include <linux/module.h>

#define PERIPH_BASE 0x3F000000
#define GPIO_BASE (PERIPH_BASE + 0x200000)

static const int LedGpioPin = 21;

struct GpioRegisters {
  volatile uint32_t GPFSEL[6];
  volatile uint32_t Reserved1;
  volatile uint32_t GPSET[2];
  volatile uint32_t Reserved2;
  volatile uint32_t GPCLR[2];
};

static void SetGPIOFunction(int GPIO, int functionCode);
static void SetGPIOOutputValue(int GPIO, bool outputValue);
