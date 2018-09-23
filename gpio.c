#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/mman.h>

/* Defines                                                                   */
#define PERI_BASE_ADDR  0x20000000
#define GPIO_START_ADDR 0x00200000
#define GPIO_END_ADDR   0x002000B0
#define GPIO_SIZE       0x01000000
#define GPIO_OFFSET     0

#define GPSET_BASE    0x1C         // Set output
#define GPCLR_BASE    0x28         // Clear output 
#define GPLEV_BASE    0x34         // Level read

#define GPFSEL_MASK 0x07           // b111

/* Global Variables                                                          */
size_t gpio_size = 0x01000000;

int fd;

volatile void *gpio_addr;

bool gpio_init()
{
   if ((fd = open("/dev/gpiomem", O_RDWR | O_SYNC)) < 0)
   {
      printf("Error reading gpiomem\n\r");
      return false;
   }

   /*
      mmap functionality:

      void *mmap(void *addr, size_t length, int prot, 
                 int flags, int fd, off_t offset);
   */
   gpio_addr = mmap(NULL, GPIO_SIZE, PROT_READ | PROT_WRITE, 
      MAP_SHARED, fd, GPIO_OFFSET);
   
   return true;
}

void gpio_deinit()
{
   munmap(&gpio_addr, GPIO_SIZE);  // Unmap gpio pointer
   gpio_addr = NULL;  // Nullify pointer
   close(fd);  // Unmap /dev/gpiomem
}

// Define the operation of the gpio pins
void gpio_fSel(uint8_t pin, uint8_t func)
{
   /* 
      Base + offset mapped by f_sel register[0-5] which hold 10 pins each
      Addresses are a contiguous set of 32 bits each
   */
   volatile uint32_t fsel_addr = (uint32_t *)gpio_addr + pin/10;
   
   // first bit number of 3-bit mask
   uint32_t fsel_baseBit = (pin % 10) * 3;  

   // clear bits << baseBit
   uint32_t fsel_mask = GPFSEL_MASK << fsel_baseBit;  

   // fsel pin mask << baseBit
   uint32_t func_mask = func << fsel_baseBit;  

   // sets pin as output
   uint32_t fsel_value = (*fsel_addr & (~fsel_mask)) | func_mask;  
   *fsel_addr = fsel_value;
}

void gpio_setOut(uint8_t pin)
{
   volatile uint32_t *set_out = (uint32_t *)(gpio_addr + GPSET_BASE + pin/32);
   uint8_t set_pin = pin % 32;
   
   __sync_synchronize();
   *set_out = (1 << set_pin); // set bit high
   __sync_synchronize();
}

void gpio_clearOut(uint8_t pin)
{
   volatile uint32_t *clear_out = (uint32_t *)(gpio_addr + GPCLR_BASE + pin/32);
   uint8_t clear_pin = pin % 32;

   __sync_synchronize();
   *clear_out = (1 << clear_pin); // set bit high
   __sync_synchronize();
}

uint32_t gpio_readLev(uint8_t pin)
{
   // Read pin from level register
   volatile uint32_t *read_lev = (uint32_t *)(gpio_addr + GPLEV_BASE + pin/32);
   uint32_t read_pin = pin % 32;

   return ((*read_lev & (1 << read_pin)) >> read_pin);
}
