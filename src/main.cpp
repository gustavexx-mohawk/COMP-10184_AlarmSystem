/* Statement of Authorship COMP-10184-01: I Gustavo Jose Marcano, 000812644 certify
that this material is my original work. No other person's work has been used without 
due acknowledgement. I have not made my work available to anyone else. */

#include <Arduino.h>
// digital input pin definitions
#define PIN_PIR D5
#define PIN_BUTTON D6

// define all possible alarm states.
#define ALARM_DISABLED 0
#define ALARM_ENABLE 1
#define ALARM_COUNTDOWN 2
#define ALARM_ACTIVE 3
#define DELAY 200
#define COUNTDOWN_TIME 40
#define BLINKING_INTERVAL 125

int iAlarmState;

bool stage2Flag = false;
bool stage5Flag = false;
bool btnState = true; // button representation
bool bPIR = false;     // false is on, true is off
bool iButton = true;   // button is up (not pressed)
bool countdownOver = false;

// *************************************************************
void setup()
{
  // configure the USB serial monitor
  Serial.begin(115200);

  // Configure the LED output
  pinMode(LED_BUILTIN, OUTPUT);

  // PIR sensor is an INPUT
  pinMode(PIN_PIR, INPUT);

  // Button is an INPUT
  pinMode(PIN_BUTTON, INPUT_PULLUP);
}

void stage1()
{
  bool bPIR; // 0 is on, 1 is off
  bPIR = digitalRead(PIN_PIR);
  Serial.println("bPIR is " + String(bPIR));

  if (bPIR)
  {
    digitalWrite(LED_BUILTIN, false);
    Serial.println("bPIR inside if " + String(bPIR));

    delay(10000);
  }
  else
  {
    digitalWrite(LED_BUILTIN, true);
    Serial.println("bPIR inside else " + String(bPIR));
  }
}

void stage2()
{
  bool bPIR; // 0 is on, 1 is off
  bPIR = digitalRead(PIN_PIR);
  Serial.println("bPIR is " + String(bPIR));

  if (bPIR)
  {
    for (int i = 0; i < 20; i++)
    {
      digitalWrite(LED_BUILTIN, HIGH); // turn the LED off (HIGH is the voltage level)
      delay(250);                      // wait for a second
      digitalWrite(LED_BUILTIN, LOW);  // turn the LED on by making the voltage LOW
      delay(250);
    }

    Serial.println("bPIR inside if " + String(bPIR));
  }
  else
  {
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("bPIR inside else " + String(bPIR));
  }
}

// void stage3() {
//   bool bPIR; // 0 is on, 1 is off
//   bPIR = digitalRead(PIN_PIR);

//   if(bPIR && !stage2Flag)
//   {
//     for(int i = 0; i < 20; i++)
//     {
//       digitalWrite(LED_BUILTIN, HIGH);   // turn the LED off (HIGH is the voltage level)
//       delay(250);                       // wait for a second
//       digitalWrite(LED_BUILTIN, LOW);    // turn the LED on by making the voltage LOW
//       delay(250);

//     }
//     stage2Flag = true;

//     Serial.println("bPIR inside if " + String(bPIR));

//   }
//   else
//   {
//     if(stage2Flag){
//       digitalWrite(LED_BUILTIN, LOW);

//     }
//     else {
//       digitalWrite(LED_BUILTIN, HIGH);

//     }
//     Serial.println("bPIR inside else " + String(bPIR));

//   }
// }
void collectInputs()
{
  iButton = digitalRead(PIN_BUTTON); // read the button

  // Toggle the state of the buttons if they are pressed on/off or off/on
  if (iButton == false && btnState == false)
  {
    btnState = true;
  }
  else if (iButton == false && btnState == true)
  {
    btnState = false;
  }
  /*********************************************************************/

  // Get inputs for movement sensor
  bPIR = digitalRead(PIN_PIR);
}
void stage4()
{

  bool bPIR; // 0 is on, 1 is off
  bPIR = digitalRead(PIN_PIR);

  // if the sensor is inactive
  if (bPIR && !stage2Flag)
  {
    for (int i = 0; i < 20; i++)
    {
      digitalWrite(LED_BUILTIN, HIGH); // turn the LED off (HIGH is the voltage level)
      delay(250);                      // wait for a second
      digitalWrite(LED_BUILTIN, LOW);  // turn the LED on by making the voltage LOW
      delay(250);

      bool iButton = digitalRead(PIN_BUTTON); // read the button

      if (iButton == false) // button pressed
      {
        // btnState = true;
        delay(1);

        // stage2Flag = false;
        // return ;
        // stage2Flag = false;
        break;
      }
    }
    stage2Flag = true;

    Serial.println("bPIR inside if " + String(bPIR));
  }
  else
  {
    if (bPIR)
    {
      digitalWrite(LED_BUILTIN, HIGH);
    }
    else
    {
      digitalWrite(LED_BUILTIN, HIGH);
      stage2Flag = false;

      // stage2Flag = false;
    }
    Serial.println("bPIR inside else " + String(bPIR));
  }
}

void checkAlarmState()
{

  // Alarm countdown from here it must go to
  // either Active or disabled
  if (btnState == true && bPIR && iButton == true && countdownOver == false)
  {
    Serial.println("Alarm Countdown (blinking) : " + String(ALARM_COUNTDOWN) + ", btnState: " + String(btnState));
    for (int i = 0; i < COUNTDOWN_TIME; i++)
    {
      // Start blinking for 10 seconds
      digitalWrite(LED_BUILTIN, HIGH); // turn the LED off (HIGH is the voltage level)
      delay(BLINKING_INTERVAL);                      // wait for a second
      digitalWrite(LED_BUILTIN, LOW);  // turn the LED on by making the voltage LOW
      delay(BLINKING_INTERVAL);

      iButton = digitalRead(PIN_BUTTON); // read the button
      //delay(BUTTON_DELAY);
      if (iButton == false) // if button pressed during blinking
      {  btnState = false;   // ten seconds passed and button was not pressed, next state DISABLED
         countdownOver = false;
         break;
      }
      else
      {
        countdownOver = true; // ten seconds passed and button was not pressed
      }
    }
  }
  // Alarm is DISABLED
  else if (btnState == false && countdownOver == false)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("Alarm DISABLED : " + String(ALARM_DISABLED) + ", btnState: " + String(btnState));
    delay(DELAY);
  }
  // Alarm ENABLED
  else if (btnState == true && !bPIR && countdownOver == false)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("Alarm ENABLED : " + String(ALARM_ENABLE) + ", btnState: " + String(btnState));
    delay(DELAY);
  }

  // Alarm Active
  else if (btnState == true && countdownOver)
  {
    Serial.println("Alarm Active (solid) : " + String(ALARM_ACTIVE) + ", btnState: " + String(btnState));
    delay(DELAY);
    digitalWrite(LED_BUILTIN, LOW);
  }
}

void loop()
{
  // Stage 5
  collectInputs();
  checkAlarmState();
  delay(DELAY);
  

  // stage1();
  // stage2();
  // stage4();
}