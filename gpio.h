#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

void gpio_init();
void gpio_deinit();
void gpio_funcSel(uint8_t pin);
void gpio_setOut(uint8_t pin);
void gpio_clearOut(uint8_t pin);
uint32_t gpio_readLev(uint8_t pin);

#endif /* GPIO_H */
