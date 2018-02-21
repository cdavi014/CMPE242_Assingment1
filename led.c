#include <linux/init.h>
#include <linux/module.h>
#include <linux/mm.h>
#include <asm/io.h>
#include <linux/gpio.h>

#define LED_PIN 	21			/* GPIO pin to toggle. */

/* INIT */
static int __init led_init(void) {
  printk("hello led!\n");
  
  gpio_request(LED_PIN, "sysfs");
  gpio_direction_output(LED_PIN, false);
  gpio_export(LED_PIN, false);
  gpio_set_value(LED_PIN, 1);

  return 0;
}

/* SYSFS */
// TODO

/* CLEAN UP */
static void __exit led_exit(void) {
  printk(KERN_ALERT "bye led!\n");
  
  gpio_set_value(LED_PIN, 0);
  gpio_unexport(LED_PIN);
  gpio_free(LED_PIN);
  // Get the correct GPSETn values and CLEARn
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Carlos R. Davila");
MODULE_DESCRIPTION("Assingment 1 LED GPIO module driver.");

module_init(led_init);
module_exit(led_exit);
