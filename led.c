#include <linux/init.h>
#include <linux/module.h>
#include <asm/io.h>
#include <linux/timer.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/mm.h>

MODULE_LICENSE("GPL");

struct GpioRegisters {
    uint32_t GPFSEL[6];
    uint32_t Reserved1;
    uint32_t GPSET[2];
    uint32_t Reserved2;
    uint32_t GPCLR[2];
};

struct GpioRegisters *s_pGpioRegisters;

static void SetGPIOFunction(int GPIO, int functionCode)
{
    int registerIndex = GPIO / 10;
    int bit = (GPIO % 10) * 3;

    unsigned oldValue = s_pGpioRegisters->GPFSEL[registerIndex];
    unsigned mask = 0b111 << bit;
    //printk("Changing function of GPIO%d from %x to %x\n", GPIO, (oldValue >> bit) & 0b111, functionCode);
    s_pGpioRegisters->GPFSEL[registerIndex] = (oldValue & ~mask) | ((functionCode << bit) & mask);
}

static void SetGPIOOutputValue(int GPIO, bool outputValue)
{
    if (outputValue)
        s_pGpioRegisters->GPSET[GPIO / 32] = (1 << (GPIO % 32));
    else
        s_pGpioRegisters->GPCLR[GPIO / 32] = (1 << (GPIO % 32));
}

static const int LedGpioPin = 21;
#define PERIPH_BASE 0x20000000
#define GPIO_BASE (PERIPH_BASE + 0x200000)

static int __init LedBlinkModule_init(void) {
    s_pGpioRegisters = (struct GpioRegisters *)ioremap(GPIO_BASE, sizeof(struct GpioRegisters));
    SetGPIOFunction(LedGpioPin, 0b001);  	//Configure the pin as output
    return 0;
}

static void __exit LedBlinkModule_exit(void) {
    iounmap(s_pGpioRegisters);
    SetGPIOFunction(LedGpioPin, 0);  	//Configure the pin as input
}

module_init(LedBlinkModule_init);
module_exit(LedBlinkModule_exit);
