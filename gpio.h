#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>
#include <stdbool.h>

#define GPIO_INPUT  0x00  // GPFSEL b000
#define GPIO_OUTPUT 0x01  // GPFSEL b001

bool gpio_init();
void gpio_deinit();
void gpio_fSel(uint8_t pin, uint8_t func);
void gpio_setOut(uint8_t pin);
void gpio_clearOut(uint8_t pin);
uint32_t gpio_readLev(uint8_t pin);

#endif /* GPIO_H */
