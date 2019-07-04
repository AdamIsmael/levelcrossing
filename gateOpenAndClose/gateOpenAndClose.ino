
// place the servos wires facing together, label side up
#include <Servo.h>

class Flasher{
  int ledPin;
  long onTime;
  long offTime;

  //maintain current state
  int ledState;
  unsigned long previousMillis;

  public:
  Flasher(int pin, long on, long off, int startState)
  {
    ledPin = pin;
    pinMode(ledPin, OUTPUT);
    onTime = on;
    offTime=off;
    ledState=startState;
    previousMillis=0;
  }

  void Flash(){
    unsigned long currentMillis = millis();

    if((ledState == HIGH) && (currentMillis - previousMillis >= onTime))
    {
      ledState = LOW;
      previousMillis = currentMillis;
      digitalWrite(ledPin, ledState);
    }
    else if ((ledState == LOW) && (currentMillis - previousMillis >= offTime))
    {
      ledState = HIGH;
      previousMillis = currentMillis;
      digitalWrite(ledPin,ledState);
    }
  }

  void Off(){
    digitalWrite(ledPin,LOW);
  }

  void On(){
    digitalWrite(ledPin, HIGH);
  }
  
};

Servo WOneServo; 
Servo EOneServo;

Servo ETwoServo;
Servo WTwoServo;

int redLED1 = 7;
int redLED2 = 12;
int yelLED = 4;

int stopShow = 0;
int barriersClosed = 0;

int previousMillisSweep = 0;
int timeBetweenLightsNBarrier = 5000;

Flasher led1(redLED1, 500, 500,0);
Flasher led2(redLED2, 500, 500,1);

void setup() {
  Serial.begin(9600);

  attachServos();
  setServosOpen();

  //Yellow LED on for five seconds
  pinMode(4, OUTPUT);
  digitalWrite(4,HIGH);
  delay(5000);
  digitalWrite(4,LOW);

  //Red LEDs on for 1 second 
  led1.On();
  led2.On();
  delay(1000);
}

void loop() {
  if(stopShow == 0){
    led1.Flash();
    led2.Flash();
  }

  unsigned long  curMillis = millis() - 5999;
  unsigned long diff = curMillis - previousMillisSweep;
  
  if(diff >= timeBetweenLightsNBarrier & stopShow == 0){
    if(barriersClosed==1){
      //delay(10000);
      Decrease(EOneServo, WOneServo); //Open
      Increase(ETwoServo, WTwoServo); //Open
      stopShow = 1;
      led1.Off();
      led2.Off();
      stopTheBuzz();
    }else{
      Increase(EOneServo, WOneServo); //Close
      Decrease(ETwoServo, WTwoServo); //Close
      previousMillisSweep=curMillis;
      barriersClosed=1;
    }
  }
}

void Increase(Servo left, Servo right){
  for (int angle = 0; angle <= 90; angle += 1) {
    left.write(left.read() + 1);  
    right.write(right.read() + 1);
    delay(15);      
    led1.Flash();
    led2.Flash();               
  }
}

void Decrease(Servo left, Servo right){
  for (int angle = 0; angle <= 90; angle += 1) {
    left.write(left.read() - 1);  
    right.write(right.read() - 1);  
    delay(15);      
    led1.Flash();
    led2.Flash();               
  }
}

void setServosOpen(){
  EOneServo.write(69);
  ETwoServo.write(118);
  WOneServo.write(28);
  WTwoServo.write(118);
}

void setServosClosed(){
  EOneServo.write(159); 
  ETwoServo.write(28);
  WOneServo.write(118);
  WTwoServo.write(28);
}

void attachServos(){
  EOneServo.attach(9);
  WOneServo.attach(10);
  ETwoServo.attach(3);
  WTwoServo.attach(11);
}

void stopTheBuzz(){
  EOneServo.detach();
  WOneServo.detach(); 
  ETwoServo.detach();
  WTwoServo.detach();
}



