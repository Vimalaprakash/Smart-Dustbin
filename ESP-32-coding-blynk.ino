// Blynk details
#define BLYNK_TEMPLATE_ID "TMPL3KUHOGVFr"
#define BLYNK_TEMPLATE_NAME "Smart Bin"
#define BLYNK_AUTH_TOKEN "huIXadP-EVxNuacZvXTE__131dJciWaZ"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

// RX and TX pins for Serial1
int RXD1 = 16;
int TXD1 = 17;

// WiFi credentials
char ssid[] = "Vimal phone";
char pass[] = "Svp@2704#";

// BlynkTimer object
BlynkTimer timer;

// Define Ultrasonic sensor pins for bins
const int trigger_pin_1 = 15;  // Wet bin
const int echo_pin_1 = 2;
const int trigger_pin_2 = 0;   // Metal bin
const int echo_pin_2 = 4;
const int trigger_pin_3 = 5;  // Plastic bin
const int echo_pin_3 = 18;
const int trigger_pin_4 = 19;   // Paper bin
const int echo_pin_4 = 21;


// LED widgets in Blynk app
WidgetLED wetWasteLED(V4);   // Green LED for wet waste
WidgetLED metalWasteLED(V5); // Yellow LED for metal waste
WidgetLED plasticWasteLED(V6); // For plastic waste (can be customized)
WidgetLED paperWasteLED(V7);   // For paper waste (can be customized)

long duration;
int distance, fillLevel;
const int Bin_Height = 45;  // Bin height in cm

void setup() {
  Serial.begin(9600);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Initialize trigger and echo pins for the sensors
  pinMode(trigger_pin_1, OUTPUT);
  pinMode(echo_pin_1, INPUT);
  pinMode(trigger_pin_2, OUTPUT);
  pinMode(echo_pin_2, INPUT);
  pinMode(trigger_pin_3, OUTPUT);
  pinMode(echo_pin_3, INPUT);
  pinMode(trigger_pin_4, OUTPUT);
  pinMode(echo_pin_4, INPUT);

  // Set intervals for reading bin sensors
  timer.setInterval(2000L, wetbin);  // Call wetbin function every 2 sec
  timer.setInterval(2000L, metalbin); // Call metalbin every 6 sec
  timer.setInterval(2000L, plasticbin);
  timer.setInterval(2000L, paperbin);

  // Start Serial communication with Arduino
  Serial1.begin(9600, SERIAL_8N1, RXD1, TXD1);  
}

void loop() {
  Blynk.run();
  timer.run();

  if (Serial1.available()) {
    String messageFromArduino = Serial1.readStringUntil('\n');
    Serial.println("Message from Arduino: " + messageFromArduino);
    delay(100);
    // If wet waste is detected, show on Blynk and turn on green LED
    if (messageFromArduino.toInt() == 0) {
      Serial.println("WET WASTE is DETECTED");
      Blynk.virtualWrite(V8, "Wet Waste Detected");
      delay(100);
      wetWasteLED.on();  // Turn on green LED
      delay(4000);       // Delay to simulate action time
      wetWasteLED.off(); // Turn off LED after some time
      delay(100);
    }
    delay(100);
    // If moisture level is sent, display on Blynk
    if (messageFromArduino.toFloat() > 0) {
    Blynk.virtualWrite(V8, "Moisture Level: " + messageFromArduino + "%");
    delay(1000);
    }
    // If metal waste is detected, show on Blynk and turn on yellow LED
    if (messageFromArduino.toInt() == 1) {
      Serial.println("METAL WASTE is DETECTED");
      Blynk.virtualWrite(V8, "Metal Waste Detected");
      delay(100);
      metalWasteLED.on();  // Turn on yellow LED
      delay(4000);         // Delay to simulate action time
      metalWasteLED.off(); // Turn off LED after some time
      delay(100);
    }
    delay(100);
    if (messageFromArduino.toInt() == 2) {
      Serial.println("PLASTIC WASTE is DETECTED");
      Blynk.virtualWrite(V8, "PLASTIC WASTE is DETECTED");
      delay(100);
      plasticWasteLED.on();  // Turn on yellow LED
      delay(4000);         // Delay to simulate action time
      plasticWasteLED.off(); // Turn off LED after some time
      delay(100);
    }
    delay(100);
    if (messageFromArduino.toInt() == 3) {
      Serial.println("PAPER WASTE is DETECTED");
      Blynk.virtualWrite(V8, "PAPER WASTE is DETECTED");
      delay(100);
      paperWasteLED.on();  // Turn on yellow LED
      delay(4000);         // Delay to simulate action time
      paperWasteLED.off(); // Turn off LED after some time
      delay(100);
    }
    delay(100);
    if (messageFromArduino.toInt() == 5) {
      Serial.println("USE MEE - Smart Bin");
      Blynk.virtualWrite(V8, "USE MEE - Smart Bin");
      delay(1000);
    }
    delay(100);
    if (messageFromArduino.toInt() == 4) {
     Serial.println("Thank you for using me - Smart Bin");
     Blynk.virtualWrite(V8, "Thank you for using me - Smart Bin");
      delay(1000);
    }
  }
}

// Function to calculate distance and fill level for the wet bin
void wetbin() {
  digitalWrite(trigger_pin_1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger_pin_1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger_pin_1, LOW);

  duration = pulseIn(echo_pin_1, HIGH);
  distance = duration * 0.0343 / 2;

  fillLevel = ((Bin_Height - distance) * 100) / Bin_Height;

  if (fillLevel < 0) fillLevel = 0;
  if (fillLevel > 100) fillLevel = 100;

  Blynk.virtualWrite(V0, fillLevel);

  Serial.print("Wet Bin - Distance: ");
  Serial.print(distance);
  Serial.print(" cm, Fill Level: ");
  Serial.print(fillLevel);
  Serial.println("%");
}

// Function to calculate distance and fill level for the metal bin
void metalbin() {
  digitalWrite(trigger_pin_2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger_pin_2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger_pin_2, LOW);

  duration = pulseIn(echo_pin_2, HIGH);
  distance = duration * 0.0343 / 2;

  fillLevel = ((Bin_Height - distance) * 100) / Bin_Height;

  if (fillLevel < 0) fillLevel = 0;
  if (fillLevel > 100) fillLevel = 100;

  Blynk.virtualWrite(V1, fillLevel);

  Serial.print("Metal Bin - Distance: ");
  Serial.print(distance);
  Serial.print(" cm, Fill Level: ");
  Serial.print(fillLevel);
  Serial.println("%");
}

void plasticbin(){
  digitalWrite(trigger_pin_3, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger_pin_3, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger_pin_3, LOW);

  duration = pulseIn(echo_pin_3, HIGH);
  distance = duration * 0.0343 / 2;

  fillLevel = ((Bin_Height - distance) * 100) / Bin_Height;

  if (fillLevel < 0) fillLevel = 0;
  if (fillLevel > 100) fillLevel = 100;

  Blynk.virtualWrite(V2, fillLevel);

  Serial.print("Plastic Bin - Distance: ");
  Serial.print(distance);
  Serial.print(" cm, Fill Level: ");
  Serial.print(fillLevel);
  Serial.println("%");
}

void paperbin(){
  digitalWrite(trigger_pin_4, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger_pin_4, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger_pin_4, LOW);

  duration = pulseIn(echo_pin_4, HIGH);
  distance = duration * 0.0343 / 2;

  fillLevel = ((Bin_Height - distance) * 100) / Bin_Height;

  if (fillLevel < 0) fillLevel = 0;
  if (fillLevel > 100) fillLevel = 100;

  Blynk.virtualWrite(V3, fillLevel);

  Serial.print("Paper Bin - Distance: ");
  Serial.print(distance);
  Serial.print(" cm, Fill Level: ");
  Serial.print(fillLevel);
  Serial.println("%");
}
