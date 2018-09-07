#include <stdio.h>
#include <stdbool.h>

#define TXPIN 0
#define RXPIN 1
#define DEBOUNCE_MAX 60  // Number of clock ticks for debounce
#define SENSOR_TRIPPED 1
#define CAMERA_TIMEOUT 6000 // FIX

int cameraCounter = 0;

// dl v4l2 driver, use libv4l2

void activateSensor()
{
   
}

void deactivateSensor()
{
   
}

int readSensor()
{
   return 0;
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
   for (int i = 0; i < 1000; i++){}  // Wait for pin to set

   // Read lev reg after pin 5 is set
   printf("Pin 5 old: %d\n\r", ((*read_lev & (1 << pin)) >> pin));

   for (int i = 0; i < 1000; i++){}  // Wait for pin to set

   // Read lev reg after pin 5 is set
   printf("Pin 5 old: %d\n\r", ((*read_lev & (1 << pin)) >> pin));
}

int main()
{
   runTest();

   while(true)
   {
      sensorControl_tick();
   }

   return 0;
}

