/* BLDC Hall Sensor read and calculation program for Teensy 3.5 in the Arduino IDE (Ver.1). Digi-Key Electronics*/

/***************************** Variables *********************************/


//INPUT/////////////////////
#define CW             1      // Assign a value to represent clock wise rotation
#define CCW           -1      // Assign a value to represent counter-clock wise rotation

bool HSU_Val = digitalRead(2);    // Set the U sensor value as boolean and read initial state
bool HSV_Val = digitalRead(3);    // Set the V sensor value as boolean and read initial state 
bool HSW_Val = digitalRead(4);    // Set the W sensor value as boolean and read initial state 


bool HSU_Val2 = digitalRead(5);    // Set the U sensor value as boolean and read initial state
bool HSV_Val2 = digitalRead(6);    // Set the V sensor value as boolean and read initial state 
bool HSW_Val2 = digitalRead(7);    // Set the W sensor value as boolean and read initial state 

int direct = 1;       // Integer variable to store BLDC rotation direction
int pulseCount;       // Integer variable to store the pulse count

int direct2 = 1;       // Integer variable to store BLDC rotation direction
int pulseCount2;       // Integer variable to store the pulse count

float startTime;        // Float variable to store the start time of the current interrupt 
float prevTime;         // Float variable to store the start time of the previous interrupt 
float pulseTimeW;       // Float variable to store the elapsed time between interrupts for hall sensor W 
float pulseTimeU;       // Float variable to store the elapsed time between interrupts for hall sensor U 
float pulseTimeV;       // Float variable to store the elapsed time between interrupts for hall sensor V 
float AvPulseTime;      // Float variable to store the average elapsed time between all interrupts 

float startTime2;        // Float variable to store the start time of the current interrupt 
float prevTime2;         // Float variable to store the start time of the previous interrupt 
float pulseTimeW2;       // Float variable to store the elapsed time between interrupts for hall sensor W 
float pulseTimeU2;       // Float variable to store the elapsed time between interrupts for hall sensor U 
float pulseTimeV2;       // Float variable to store the elapsed time between interrupts for hall sensor V 
float AvPulseTime2;      // Float variable to store the average elapsed time between all interrupts 

float PPM;        // Float variable to store calculated pulses per minute
float RPM;        // Float variable to store calculated revolutions per minute

float PPM2;        // Float variable to store calculated pulses per minute
float RPM2;        // Float variable to store calculated revolutions per minute

//OUTPUT//////////////
const byte QuadratureA_DOPin = 8;
const byte QuadratureB_DOPin = 9;

const byte QuadratureA_DOPin2 = 10;
const byte QuadratureB_DOPin2 = 11;


const byte A = 1;
const byte B = 2;
const byte QuadratureStates[4] = {0, A, A + B, B};
byte QuadratureState = 0;

const byte QuadratureStates2[4] = {0, A, A + B, B};
byte QuadratureState2 = 0;
/***************************** Setup *********************************/

void setup()
{
// Set digital pins 2, 3 and 4 as inputs
  pinMode(2, INPUT);      
  pinMode(3, INPUT);      
  pinMode(4, INPUT);

  pinMode(5, INPUT);      
  pinMode(6, INPUT);      
  pinMode(7, INPUT);

  pinMode(QuadratureA_DOPin, OUTPUT);
  pinMode(QuadratureB_DOPin, OUTPUT);

  pinMode(QuadratureA_DOPin2, OUTPUT);
  pinMode(QuadratureB_DOPin2, OUTPUT);

// Set digital pins 2, 3 and 4 as interrupts that trigger on rising and falling edge changes. Call a function (i.e. HallSensorU) on change
  attachInterrupt(digitalPinToInterrupt(2), HallSensorU, CHANGE);      
  attachInterrupt(digitalPinToInterrupt(3), HallSensorV, CHANGE);
  attachInterrupt(digitalPinToInterrupt(4), HallSensorW, CHANGE);

  attachInterrupt(digitalPinToInterrupt(5), HallSensorU2, CHANGE);      
  attachInterrupt(digitalPinToInterrupt(6), HallSensorV2, CHANGE);
  attachInterrupt(digitalPinToInterrupt(7), HallSensorW2, CHANGE);


// Initialize the print monitor and set baud rate to 9600 
  Serial.begin(9600);
}

/*************************** Main Loop ******************************/

void loop()
{
  if ((millis() - prevTime) > 600) RPM = 0;                                           // Zero out RPM variable if wheel is stopped

 if (pulseCount !=pulseCount) (direct = -1);
  {
    QuadratureState = (QuadratureState - 1) % 4;
  }
  if (pulseCount !=pulseCount)(direct = 1);
  {
    QuadratureState = (QuadratureState + 1) % 4;
  }
 digitalWrite(QuadratureA_DOPin, QuadratureStates[QuadratureState] & A);
 digitalWrite(QuadratureB_DOPin, QuadratureStates[QuadratureState] & B);


if ((millis() - prevTime) > 600) RPM = 0;                                           // Zero out RPM variable if wheel is stopped

 if (pulseCount2 !=pulseCount2) (direct2 = -1);
  {
    QuadratureState2 = (QuadratureState2 - 1) % 4;
  }
  if (pulseCount2 !=pulseCount2)(direct2 = 1);
  {
    QuadratureState2 = (QuadratureState2 + 1) % 4;
  }
 digitalWrite(QuadratureA_DOPin2, QuadratureStates2[QuadratureState2] & A);
 digitalWrite(QuadratureB_DOPin2, QuadratureStates2[QuadratureState2] & B);

 
}

/************************ Interrupt Functions ***************************/

void HallSensorW()
{
  startTime = millis();           // Set startTime to current microcontroller elapsed time value
  HSW_Val = digitalRead(4);         // Read the current W hall sensor value
  HSV_Val = digitalRead(3);           // Read the current V (or U) hall sensor value 
  direct = (HSW_Val == HSV_Val) ? CW : CCW;     // Determine rotation direction (ternary if statement)
  pulseCount = pulseCount + (1 * direct);       // Add 1 to the pulse count
  pulseTimeW = startTime - prevTime;        // Calculate the current time between pulses
  AvPulseTime = ((pulseTimeW + pulseTimeU + pulseTimeV)/3); // Calculate the average time time between pulses
  PPM = (1000 / AvPulseTime) * 60;          // Calculate the pulses per min (1000 millis in 1 second)
  RPM = PPM / 90;           // Calculate revs per minute based on 90 pulses per rev
  prevTime = startTime;           // Remember the start time for the next interrupt
}

void HallSensorV()
{
  startTime = millis();
  HSV_Val = digitalRead(3);
  HSU_Val = digitalRead(2);         // Read the current U (or W) hall sensor value 
  direct = (HSV_Val == HSU_Val) ? CW : CCW;
  pulseCount = pulseCount + (1 * direct);
  pulseTimeV = startTime - prevTime;        
  AvPulseTime = ((pulseTimeW + pulseTimeU + pulseTimeV)/3);   
  PPM = (1000 / AvPulseTime) * 60;          
  RPM = PPM / 90;
  prevTime = startTime;
}

void HallSensorU()
{
  startTime = millis();
  HSU_Val = digitalRead(2);
  HSW_Val = digitalRead(4);         // Read the current W (or V) hall sensor value    
  direct = (HSU_Val == HSW_Val) ? CW : CCW;
  pulseCount = pulseCount + (1 * direct);
  pulseTimeU = startTime - prevTime;        
  AvPulseTime = ((pulseTimeW + pulseTimeU + pulseTimeV)/3);   
  PPM = (1000 / AvPulseTime) * 60;          
  RPM = PPM / 90;
  prevTime = startTime;
}


void HallSensorW2()
{
  startTime2 = millis();           // Set startTime to current microcontroller elapsed time value
  HSW_Val2 = digitalRead(4);         // Read the current W hall sensor value
  HSV_Val2 = digitalRead(3);           // Read the current V (or U) hall sensor value 
  direct2 = (HSW_Val2 == HSV_Val2) ? CW : CCW;     // Determine rotation direction (ternary if statement)
  pulseCount2 = pulseCount2 + (1 * direct2);       // Add 1 to the pulse count
  pulseTimeW2 = startTime2 - prevTime2;        // Calculate the current time between pulses
  AvPulseTime2 = ((pulseTimeW2 + pulseTimeU2 + pulseTimeV2)/3); // Calculate the average time time between pulses
  PPM2 = (1000 / AvPulseTime2) * 60;          // Calculate the pulses per min (1000 millis in 1 second)
  RPM2 = PPM2 / 90;           // Calculate revs per minute based on 90 pulses per rev
  prevTime2 = startTime2;           // Remember the start time for the next interrupt
}

void HallSensorV2()
{
  startTime2 = millis();
  HSV_Val2 = digitalRead(3);
  HSU_Val2 = digitalRead(2);         // Read the current U (or W) hall sensor value 
  direct2 = (HSV_Val2 == HSU_Val2) ? CW : CCW;
  pulseCount2 = pulseCount2 + (1 * direct2);
  pulseTimeV2 = startTime2 - prevTime2;        
  AvPulseTime2 = ((pulseTimeW2 + pulseTimeU2 + pulseTimeV2)/3);   
  PPM2 = (1000 / AvPulseTime2) * 60;          
  RPM2 = PPM2 / 90;
  prevTime2 = startTime2;
}

void HallSensorU2()
{
  startTime2 = millis();
  HSU_Val2 = digitalRead(2);
  HSW_Val2 = digitalRead(4);         // Read the current W (or V) hall sensor value    
  direct2 = (HSU_Val2 == HSW_Val2) ? CW : CCW;
  pulseCount2 = pulseCount2 + (1 * direct2);
  pulseTimeU2 = startTime - prevTime;        
  AvPulseTime2 = ((pulseTimeW2 + pulseTimeU2 + pulseTimeV2)/3);   
  PPM2 = (1000 / AvPulseTime2) * 60;          
  RPM2 = PPM2 / 90;
  prevTime2 = startTime2;
}
