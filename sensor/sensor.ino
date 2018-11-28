#define TXPIN 2
#define RXPIN 3
#define SENSOR_PIN 5
#define TRIP_THRESHOLD 600  // Voltage level from photoresistor
#define DEBOUNCE_MAX 60  // Number of clock ticks for debounce

int debounceTimer = 0;

void setup() 
{
  Serial.begin(9600);  //  setup serial
  
  pinMode(TXPIN, OUTPUT);
  pinMode(RXPIN, INPUT);

  digitalWrite(TXPIN, LOW);
}

enum sensorControl_st_t
{
  init_st,
  wait_activate_st,
  readSensor_st,
  debounceSensor_st,
  waitReset_st
} currentState = init_st;

void sensorControl_tick()
{
  // perform state action first
  switch (currentState)
  {
    case init_st:
      break;
    case wait_activate_st:
      break;
    case readSensor_st:
      break;
    case debounceSensor_st:
      debounceTimer++;
      break;
    case waitReset_st:
      break;
    default:
      Serial.println("tick state default error");
      break;
  }

  // perform state update next
  switch (currentState)
  {
    case init_st:
      Serial.println("init state");
      digitalWrite(TXPIN, LOW);  // Reset transmit signal
      debounceTimer = 0;  // Reset debounce timer
      currentState = wait_activate_st;
      break;
    case wait_activate_st:
      if (digitalRead(RXPIN) == HIGH)
      {
        currentState = readSensor_st;
        Serial.println("read sensor");
      }
      break;
    case readSensor_st:
      // read sensor input
      if (analogRead(SENSOR_PIN) <= TRIP_THRESHOLD)
      { 
        currentState = debounceSensor_st;
      }
      break;
    case debounceSensor_st:
      // Debounce sensor
      if (debounceTimer >= DEBOUNCE_MAX)
      {
        // Check sensor after debounce
        if (analogRead(SENSOR_PIN) <= TRIP_THRESHOLD)
        {
          Serial.println("line tripped");
          digitalWrite(TXPIN, HIGH);
          currentState = waitReset_st;
        }
      }
      break;
    case waitReset_st:
      if (digitalRead(RXPIN) == LOW)
      {
        Serial.println("Resetting");
        currentState = init_st;        
      }
      break;
    default:
      Serial.println("tick state default error");
      break;
  }
}

void loop() 
{
//  Serial.print(analogRead(SENSOR_PIN));
//  Serial.println(digitalRead(RXPIN));
//  delay(500);
  sensorControl_tick();
}
