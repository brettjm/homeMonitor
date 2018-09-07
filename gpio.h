#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>
#include <stdbool.h>

bool gpio_init();
void gpio_deinit();
uint32_t gpio_fSel(uint8_t pin);
uint32_t gpio_setOut(uint8_t pin);
uint32_t gpio_clearOut(uint8_t pin);
uint32_t gpio_readLev(uint8_t pin);

#endif /* GPIO_H */
