/*
 * Project swellstation
 * Description: an automated ocean condition display
 * Author:  Richard Sutherland  
 * Date: 5/1/2021
 */

const int hPinOut = D0;
const int pPinOut = D1;
const int greenLED = D2;
const int redLED = D3;

// These constants represent the values at which each dial will read 100%.
// The values were chosen based on historical data.
const float MAX_HEIGHT = 12.0;
const float MAX_PERIOD = 24.0;

class SwellStation {
  public:
    SwellStation(){
    }
    
    // height: the current height, in feet of the dominant swell.
    // period: the frequency of the dominant swell, in seconds.
    float height;
    float period;

    void setup(){
      // register the update function, so that when the particle.io cloud gets
      // a request to run the "update" function on this device, it knows what to do.
      Particle.function("update", &SwellStation::update, this);
      
      flashLED(greenLED, 5);
      flashLED(redLED, 5);
    }

    int update(String values){
      // reset error state LED
      digitalWrite(redLED, LOW);
      
      // parse the update string
      // format is HH.HPP
      // eg: 
      // 4ft at 15s would look like 04.015
      // 10.5ft at 8s would look like 10.508
      height = values.substring(0,4).toFloat();
      period = values.substring(4,6).toFloat();
      
      if(height == 0 || period == 0){
        // this is an error state
        digitalWrite(redLED, HIGH);
        return -1;
      }

      // hValue and pValue represent the scaled 
      // PWM signal to send to each analogue dial
      // if either scaled value is above 255 (max)
      // just set it at the max and turn on error led.
      float hValue = (height/MAX_HEIGHT) * 255;
      float pValue = (period/MAX_PERIOD) * 255;
      if(hValue > 255) {
        hValue = 255;
        digitalWrite(redLED, HIGH);
      }
      if(pValue > 255){
        pValue = 255;
        digitalWrite(redLED, HIGH);
      }
    
      analogWrite(hPinOut, hValue);
      analogWrite(pPinOut, pValue);
      
      // green LED flashes on successful update.
      flashLED(greenLED, 3);

      return 0;

    }

    // blink an LED count times.
    void flashLED(int led, int count){
      int i = 0;
      while (i < count){
        digitalWrite(led, HIGH);
        delay(500);
        digitalWrite(led, LOW);
        delay(500);
        i++;
      }
    }


};

SwellStation station;

// setup() runs once, when the device is first turned on.
void setup() {
  // hardware initialization
  pinMode(hPinOut, OUTPUT);
  pinMode(pPinOut, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);

  // setup the station
  station.setup();
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  
}

