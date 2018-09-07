#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

void gpio_init();
void gpio_deinit();
uint32_t gpio_funcSel();
uint32_t gpio_setOut();
uint32_t gpio_clearOut();
uint32_t gpio_readLev();

#endif /* GPIO_H */
