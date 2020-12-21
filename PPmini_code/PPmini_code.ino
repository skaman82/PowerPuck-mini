// ATtinyCore AT85 (https://github.com/SpenceKonde/ATTinyCore)
// millis and tone, 8mhz
// upload through USBTinyISP


#define voltage_scale   4.975   // play with this number to adjust voltage scale
#define R1              10000   // Voltage Divider Resistor1 10k
#define R2              10000   // Voltage Divider Resistor2 10k

#define BZ_pin          3       // Buzzer
#define GATE_pin        1       // Mosfet
#define VBAT_pin        A1      // vsens pin
#define BT_pin          0       // Button
#define LED_pin         4       // LED
#define longpresstime   1000    // in ms
#define alarmADDR       1       // EEPROM Adress

#include <EEPROM.h>
#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(1, LED_pin, NEO_GRB + NEO_KHZ800);

byte pressedbut = 0;
byte i_butt = 0;
float voltage;
float cellvoltage;
byte celldetect = 0;
long previousMillis = 0;
long buzzermillis = 0;
byte alarmstate = 0; //lst
int vsens;
byte onstate = 1; //lst
int timer = 0;
int32_t alarmvalueEEP;
float alarmvoltage;
int menuselect;

void setup() {
  //pinMode(BZ_pin, OUTPUT);        //Buzzer
  pinMode(GATE_pin, OUTPUT);      //Mosfet Gate
  pinMode(VBAT_pin, INPUT);       //Voltage Input
  pinMode(BT_pin, INPUT_PULLUP);  //Button
  //pinMode(LED_pin, OUTPUT);       //Status LED

  pixels.begin(); // This initializes the NeoPixel library.
  pixels.clear();

  alarmvalueEEP = EEPROM.read(alarmADDR);

  if (alarmvalueEEP == 1)
  {
    alarmvoltage = 3.35;
  }
  else if (alarmvalueEEP == 2)
  {
    alarmvoltage = 3.25;
  }
  else if (alarmvalueEEP == 3)
  {
    alarmvoltage = 3.15;
  }
  else { }


digitalWrite(GATE_pin, HIGH);

}


byte buttoncheck()
{
  int i_butt = 0;
  byte buttonz = 0;
  if (digitalRead(BT_pin) != 1) {
    while (digitalRead(BT_pin) != 1)
    {
      delay(2);
      i_butt++;
    }
    buttonz = 1; //Button pressed

    if (i_butt > (longpresstime / 2))
    {
      buttonz += 1; //Button pressed long
      delay(2);
    }

  }


  pressedbut = buttonz;
  return buttonz;
}




void loop(void) {

  if (menuselect != 1) {

    buttoncheck();
    ReadVoltage();




    if (celldetect == 0) {

      //do something while voltage is not there yet
      tone(BZ_pin, 1000, 600); //beep for 500ms
    }

    else {

      if (cellvoltage < alarmvoltage) {
        unsigned long buzzertime = millis();
        if (buzzertime - buzzermillis > 1000) { // one second loop
          buzzermillis = buzzertime;

          if (alarmstate == 0) {
            alarmstate = 1;
            tone(BZ_pin, 500, 200); //beep for 200ms
            pixels.setPixelColor(0, 255, 0, 0); // red
            pixels.show();

          }
          else {
            alarmstate = 0;
            noTone(BZ_pin); //no beep
            pixels.setPixelColor(0, 0, 0, 0); // off
            pixels.show();
          }
        }
        else { }
      }


      if (pressedbut == 1) {

        if (onstate == 0) {
          onstate = 1;
          tone(BZ_pin, 100, 200); //beep for 100ms
          delay(200);
          tone(BZ_pin, 400, 150); //beep for 400ms
          //digitalWrite(LED_pin, HIGH);
          digitalWrite(GATE_pin, HIGH);

        }
        else {
          onstate = 0;
          tone(BZ_pin, 400, 200); //beep for 400ms
          delay(200);
          tone(BZ_pin, 100, 150); //beep for 100ms
          //digitalWrite(LED_pin, LOW);
          digitalWrite(GATE_pin, LOW);
        }
      }

      if (pressedbut == 2)
      {
        menuselect = 1;
        menu();
      }
      else { }

      updateLED();
    }

  }
  else {  }

}

void ReadVoltage(void) {

  vsens = analogRead(VBAT_pin);

  unsigned long updatetime = millis();

  if (updatetime - previousMillis > 1500) { // update voltage reading after some time
    previousMillis = updatetime;

    voltage = vsens * (voltage_scale / 1023.0) * ((R1 + R2) / R2); // Convert the analog reading (which goes from 0 - 1023) to a voltage, considering the voltage divider:

    if (celldetect == 0) {
      //detect cell count (dont really needed as the hardware is ment to be a 2S pack only)

      if (voltage > 17.1) {
        celldetect = 5;
      }
      else if ((voltage <= 17.1) && (voltage > 12.8)) {
        celldetect = 4;
      }
      else if ((voltage <= 12.8) && (voltage > 8.8)) {
        celldetect = 3;
      }
      else if ((voltage <= 8.8) && (voltage > 5.5)) {
        celldetect = 2;
      }
      else {
        celldetect = 1;
      }
    }

    cellvoltage = voltage / celldetect;

  }


}


void updateLED(void) {

  if (onstate == 1) {

    if (cellvoltage >= 3.65) {
      pixels.setPixelColor(0, 0, 150, 0); // green
      pixels.show();
    }
    else if ((cellvoltage < 3.65) && (cellvoltage >= 3.45)) {
      pixels.setPixelColor(0, 150, 69, 0); // yellow
      pixels.show();
    }
    else if ((cellvoltage < 3.45) && (cellvoltage >= alarmvoltage)) {
      pixels.setPixelColor(0, 150, 0, 0); // red
      pixels.show();
    }
    else if (cellvoltage < alarmvoltage) {
      //blinking red

      if (timer <= 2000) {
        // pixels.setPixelColor(0, 255, 0, 0); // blue just for testing
        // pixels.show();
        timer++;

        if ((timer <= 1000) && (timer < 2000)) {
          //  pixels.setPixelColor(0, 0, 0, 0); // off
          //  pixels.show();
        }

        if (timer == 2000) {
          timer = 0;
        }

      }

    }

  }

  else {
    pixels.setPixelColor(0, 0, 0, 0); // off
    pixels.show();
  }
}







void menu() {
  while (menuselect == 1) {
    buttoncheck();

    if (pressedbut == 2) {
      
      EEPROM.write(alarmADDR, alarmvalueEEP);
      tone(BZ_pin, 1000, 600); //beep for 500ms
      pixels.clear();
      menuselect = 0;
      return;
    }


    if (alarmvalueEEP == 1) { // green 3.35
      
      if (pressedbut == 1) {
        alarmvoltage = 3.25;
        alarmvalueEEP = 2;
      }

      //led color here
      pixels.setPixelColor(0, 0, 255, 0); // ^ green
      pixels.show();

    }
    else if (alarmvalueEEP == 2) { //yellow 3.25
      if (pressedbut == 1) {
        alarmvoltage = 3.15;
        alarmvalueEEP = 3;
      }
      //led color here
      pixels.setPixelColor(0, 200, 100, 0); // ^ yellow
      pixels.show();
      
      
      
    }

    else if (alarmvalueEEP == 3) { //red 3.15
      if (pressedbut == 1) {
        alarmvoltage = 3.35;
        alarmvalueEEP = 1;
      }
      //led color here
      pixels.setPixelColor(0, 255, 0, 0); // ^ white
      pixels.show();
     
    }

    else { 
pixels.setPixelColor(0, 250, 250, 250); // ^ white for error
      pixels.show();
      delay(1000);
      alarmvalueEEP = 1;
      }

  }

}
