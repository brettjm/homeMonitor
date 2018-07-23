#include <stdio.h>

int main()
{
   enum clockControl_st_t
{
  init_st,
  second_timer_st,
  minute_st
} currentState = init_st;

void clockControl_tick()
{
  // perform state action first
  switch (currentState)
  {
    case init_st:
      break;
    case second_timer_st:
      break;
    case minute_st:
      break;
    default:
      Serial.print("tick state default error");
      break;
  }

  // perform state update next
  switch (currentState)
  {
    case init_st:
      break;
    case second_timer_st:
      break;
    case minute_st:
      break;
    default:
      Serial.print("tick state default error");
      break;
  }
}

   return 0;
}