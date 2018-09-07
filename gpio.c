#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/mman.h>

#define PERI_BASE_ADDR  0x20000000
#define GPIO_START_ADDR 0x00200000
#define GPIO_END_ADDR   0x002000B0
#define GPIO_SIZE       0x01000000
#define GPIO_OFFSET     0

#define GPSET_BASE    0x1C        // Set output
#define GPCLR_BASE    0x28        // Clear output 
#define GPLEV_BASE    0x34        // Level read

#define GPFSEL_MASK 0x07  // b111
#define GPFSEL_INPUT 0x00 // b000
#define GPFSEL_OUTPUT 0x01 // b001

size_t gpio_size = 0x01000000;

int fd;

volatile void *gpio_addr;
volatile uint32_t *fsel_addr;
volatile uint32_t *set_out;
volatile uint32_t *clear_out;
volatile uint32_t *read_lev;

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
   gpio_addr = mmap(NULL, GPIO_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, GPIO_OFFSET);
   
   return true;
}

void gpio_deinit()
{
   // Unmap gpio pointer
   munmap(&gpio_addr, GPIO_SIZE);
   
   // Nullify pointers
   gpio_addr = NULL;
   fsel_addr = NULL;
   set_out   = NULL;
   read_lev  = NULL;

   // Unmap /dev/gpiomem
   close(fd);
}

uint32_t gpio_fSel(uint8_t pin)
{
   // Base + offset mapped by f_sel 
   // register[0-5] which hold 10 pins each
   fsel_addr = (uint32_t *)gpio_addr + (pin/10);
   
   // first bit number of 3-bit mask
   uint32_t fsel_baseBit = (pin % 10) * 3;  

   // fsel mask (b111) << baseBit
   uint32_t fsel_mask = GPFSEL_MASK << fsel_baseBit;  

   // fsel pin is output mask (b001) << baseBit
   uint32_t outputMask = GPFSEL_OUTPUT << fsel_mask;  

   // sets pin as output
   uint32_t fselValue = (*fsel_addr & (~fsel_mask)) | outputMask;  
   *fsel_addr = fselValue;
   
   return 0;
}

uint32_t gpio_setOut(uint8_t pin)
{
   set_out = (uint32_t *)(gpio_addr + GPSET_BASE + pin/32);

   __sync_synchronize();
   *set_out = (1 << pin); // PIN 5 set high
   __sync_synchronize();

   return 0;
}

uint32_t gpio_clearOut(uint8_t pin)
{
   clear_out = (uint32_t *)(gpio_addr + GPCLR_BASE + pin/32);

   __sync_synchronize();
   *clear_out = (1 << pin); // PIN 5 set high
   __sync_synchronize();

   return 0;
}

uint32_t gpio_readLev(uint8_t pin)
{
   // Read pin from level register
   read_lev = (uint32_t *)(gpio_addr + GPLEV_BASE);

   return ((*read_lev & (1 << pin)) >> pin);
}
