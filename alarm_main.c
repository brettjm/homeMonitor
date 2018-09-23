#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include "gpio.h"

#define TXPIN 5
#define RXPIN 1
#define DEBOUNCE_MAX 60  // Number of clock ticks for debounce
#define SENSOR_TRIPPED 1
#define CAMERA_TIMEOUT 6000 // FIX

#define DEBUG

int cameraCounter = 0;

// dl v4l2 driver, use libv4l2
// https://www.raspberrypi.org/forums/viewtopic.php?t=174098

void activateSensor()
{
   gpio_setOut(TXPIN);
}

void deactivateSensor()
{
   gpio_clearOut(TXPIN);   
}

int readSensor()
{
   return gpio_readLev(RXPIN);
}

void activateCamera()
{
   
}

void deactivateCamera()
{
   
}

int readCamera()
{
   return 0;
}

void handleFootage()
{
   // int cameraRead = readCamera();
}
  
enum sensorControl_st_t
{
   init_st,
   activate_sensor_st,
   wait_sensor_st,
   deactivate_sensor_st,
   activate_camera_st,
   hold_camera_st,
   deactivate_camera_st,
   handle_footage_st
} currentState = init_st;

void sensorControl_tick()
{
   // perform state action first
   switch (currentState)
   {
   case init_st:
      break;
   case activate_sensor_st:
      break;
   case wait_sensor_st:
      break;
   case deactivate_sensor_st:
      break;
   case activate_camera_st:
      break;
   case hold_camera_st:
      cameraCounter++;
      break;
   case deactivate_camera_st:
      break;
   case handle_footage_st:
      break;
   default:
      printf("tick state default error");
      break;
  }

  // perform state update next
  switch (currentState)
  {
   case init_st:
      cameraCounter = 0;
      currentState = activate_sensor_st;
      break;
   case activate_sensor_st:
      activateSensor();
      currentState = wait_sensor_st;
      break;
   case wait_sensor_st:
      if (readSensor() == SENSOR_TRIPPED)
         currentState = activate_camera_st;
      break;
   case deactivate_sensor_st:
      deactivateSensor();
      currentState = activate_camera_st;
      break;
   case activate_camera_st:
      activateCamera();
      currentState = hold_camera_st;
      break;
   case hold_camera_st:
      if (cameraCounter >= CAMERA_TIMEOUT)
         currentState = deactivate_camera_st;
      break;
   case deactivate_camera_st:
      deactivateCamera();
      currentState = handle_footage_st;
      break;
   case handle_footage_st:
      handleFootage();
      currentState = init_st;
      break;
   default:
      printf("tick state default error");
      break;
   }
}

void runTest()
{
   // Set pin as output
   gpio_fSel(TXPIN, GPIO_OUTPUT);

   // Read pin
   for (int i = 0; i < 1000; i++){}  // Wait for pin to set
   printf("Pin %d init: %d\n\r", TXPIN, gpio_readLev(TXPIN));

   // Set pin high
   gpio_setOut(TXPIN);

   // Read pin
   for (int i = 0; i < 1000; i++){}  // Wait for pin to set
   printf("Pin %d set: %d\n\r", TXPIN, gpio_readLev(TXPIN));

   // Set pin low
   gpio_clearOut(TXPIN);

   // Read pin
   for (int i = 0; i < 1000; i++){}  // Wait for pin to set
   printf("Pin %d clear: %d\n\r", TXPIN, gpio_readLev(TXPIN));

   gpio_deinit();
}

int main()
{
   // Init gpio
   if (!gpio_init())
      return 0;

#ifndef DEBUG

   // Set pin functions
   gpio_fSel(TXPIN, GPIO_OUTPUT);
   gpio_fSel(RXPIN, GPIO_INPUT);

   // Run Control state machine
   while(true)
   {
      sensorControl_tick();
   }

#else
   runTest();
   
#endif

   gpio_deinit();  // De-init gpio

   return 0;
}

