#include <Servo.h>  
#include <SoftwareSerial.h>

int pos = 0;  

// Define Servo objects
Servo Servo1;
Servo Servo2;
Servo Servo3;
Servo Servo4;
Servo Servo5;
Servo Servo6;
Servo Servo7;

// Define pin constants
const int spin1 = 5;
const int spin2 = 6;
const int spin3 = 7;  // Servo motor 3 pin
const int spin4 = 8; // Servo motor 4 pin
const int spin5 = 9;
const int spin6 = 12;
const int spin7 = 13;
const int CMS_pin = A0;  // Capacitive Moisture Sensor Pin
const int IPS = 3;  // Inductive Proximity Sensor
const int IR = 4; // Motion Sensor
const int CPS = 2;

// Define RX and TX pins for communication with ESP32
const int rxPin = 10;  // Pin connected to ESP32's TX
const int txPin = 11;  // Pin connected to ESP32's RX

// Create a SoftwareSerial object for ESP32 communication
SoftwareSerial espSerial(rxPin, txPin);



void centralservo(){

    Servo3.attach(spin3);
   for (pos = 0; pos <= 180; pos += 10) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    Servo3.write(pos);              // tell servo to go to position in variable 'pos'
    delay(35);                 // waits 15 ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 10) { // goes from 180 degrees to 0 degrees
    Servo3.write(pos);              // tell servo to go to position in variable 'pos'
    delay(35);  
  }
    Servo3.detach();

}




// Function for capacitive moisture sensor (Wet Waste)
void wetwaste(int analog_value) {
  int moisturevalue = analogRead(analog_value);
  float moisturelevel = map(moisturevalue, 0, 1023, 0, 100);
  Serial.println(moisturelevel);
  delay(1000);

  if (moisturelevel <= 45) {
    
    // Send message to ESP32 indicating wet waste
    espSerial.println("0");
    delay(5000);
    // Send moisture level to ESP32
    espSerial.println(moisturelevel);
    delay(1000);

    Servo2.write(90);
    delay(100);
    Servo2.write(0);
    delay(1000);
    Servo2.write(90);
    delay(110);

    
    Servo4.attach(spin4);
    Servo4.write(0);
    Servo4.write(180);
    delay(125);
    Servo4.detach();
    centralservo();
    delay(5000);
    Servo4.attach(spin4);
    Servo4.write(0);
    delay(100);
    Servo4.detach();
    delay(5000);
    
    Serial.println("Sensor Value: " + String(analog_value));
    Serial.println("Moisture Level: " + String(moisturelevel) + "%");

    // Send message to ESP32 indicating wet waste
    espSerial.println("0");
    delay(5000);
    // Send moisture level to ESP32
    espSerial.println(moisturelevel);
    delay(1000);
  } 
  else {
    metalwaste(IPS);  // Call metal waste detection if no wet waste
  }
}



// Function for metal waste detection
void metalwaste(int IPS_pin) {
  int metalobject = digitalRead(IPS_pin);
  Serial.println(metalobject);
  delay(1000);

  if (metalobject == LOW) 
  {
    // Send message to ESP32 indicating metal waste
    espSerial.println("1");
    delay(1000);

    Serial.println("Metal Object is detected");
    Servo2.write(90);
    delay(100);
    Servo2.write(0);
    delay(1000);
    Servo2.write(90);
    delay(110);

    
    Servo5.attach(spin5);
    Servo5.write(0);
    Servo5.write(180);
    delay(115);
    Servo5.detach();
    centralservo();
    delay(5000);
    Servo5.attach(spin5);
    Servo5.write(0);
    delay(100);
    Servo5.detach();
    delay(5000);

    // Send message to ESP32 indicating metal waste
    espSerial.println("1");
    delay(1000);

  }
  else {
    plasticwaste(CPS);  // Call metal waste detection if no wet waste
  }
}

void plasticwaste(int CPS){
  int plasticobject = digitalRead(CPS);
  if (plasticobject == HIGH){
    Serial.println("Plastic Object is detected");

    Servo2.attach(spin2);
    Servo2.write(100);
    delay(1000);
    Servo2.write(0);
    delay(5000);
    Servo2.write(100);
    delay(1000);
   // Servo6.detach();

    Servo6.attach(spin6);
    Servo6.write(180);
    Servo6.write(0);
    delay(120);
    Servo6.detach();
    centralservo();
    delay(5000);
    Servo6.attach(spin6);
    Servo6.write(180);
    delay(100);
    Servo6.detach();
    delay(5000);

    // Send message to ESP32 indicating metal waste
    espSerial.println("2");
    delay(1000);

  }
  else{
    paperwaste();
    delay(10000);
    wetwaste(CMS_pin);
  }
}

void paperwaste(){
  // Send message to ESP32 indicating metal waste
    espSerial.println("3");
    delay(1000);

    Serial.println("Paper Object is detected");
    Servo2.write(90);
    delay(100);
    Servo2.write(0);
    delay(1000);
    Servo2.write(90);
    delay(110);


    Servo7.attach(spin7);
    Servo7.write(180);
    Servo7.write(0);
    delay(120);
    Servo7.detach();
    centralservo();
    delay(5000);
    Servo7.attach(spin7);
    Servo7.write(180);
    delay(100);    
    Servo7.detach();
    delay(5000);

    // Send message to ESP32 indicating metal waste
    espSerial.println("3");
    delay(1000);

}

void setup() {
  pinMode(IPS, INPUT);  // Set IPS pin as input
  pinMode(CPS, INPUT);
  Servo1.attach(spin1);
  Servo2.attach(spin2);
  Servo3.attach(spin3);
  Servo4.attach(spin4);
  Servo5.attach(spin5);
  Servo6.attach(spin6);
  Servo7.attach(spin7);
 // Servo2.write(120);
 // delay(100);
  Serial.begin(9600);
  espSerial.begin(9600);  // Start Serial communication with ESP32
}

void loop() {
  
  int motion = digitalRead(IR);
  
  if(motion == LOW){
    Serial.println("motion is detected"); 
    
    espSerial.println("4");
    delay(1000);

    Servo1.attach(spin1);
    delay(100);
    Servo1.write(90);
    delay(100);
    Servo1.write(0);
    delay(5000);  // Delay to control loop speed
    Servo1.write(90);
    delay(100);
    Servo1.detach();
    metalwaste(IPS);  // Call wet waste function

  }
  else {
    Serial.println("Wating for the motion");
    espSerial.println("5");
    delay(1000);
  }
}