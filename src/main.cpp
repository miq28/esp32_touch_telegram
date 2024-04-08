#include <Arduino.h>



/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/telegram-esp32-motion-detection-arduino/
  
  Project created using Brian Lough's Universal Telegram Bot Library: https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot
*/

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

// Replace with your network credentials
static const char* WIFI_SSID = ENV_WIFI_SSID;
static const char* WIFI_PASSWORD = ENV_WIFI_PASSWORD;

// Initialize Telegram BOT
#define BOTtoken ENV_BOT_TOKEN  // your Bot Token (Get from Botfather)

// Use @myidbot to find out the chat ID of an individual or a group
// Also note that you need to click "start" on a bot before it can
// message you
#define CHAT_ID ENV_CHAT_ID

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

const int motionSensor = 27; // PIR Motion Sensor
bool motionDetected = false;

// Indicates when motion is detected
void IRAM_ATTR detectsMovement() {
  //Serial.println("MOTION DETECTED!!!");
  motionDetected = true;
}

// set pin numbers
const int touchPin = 4; 
const int ledPin = 2;

// change with your threshold value
const int threshold = 40;
// variable for storing the touch pin value 
int touchValue;

bool touch1detected = false;

void gotTouch1(){
 touch1detected = true;
}




// put function declarations here:
int myFunction(int, int);

void setup() {
  Serial.begin(115200);

  // PIR Motion Sensor mode INPUT_PULLUP
  pinMode(motionSensor, INPUT_PULLUP);
  // Set motionSensor pin as interrupt, assign interrupt function and set RISING mode
  attachInterrupt(digitalPinToInterrupt(motionSensor), detectsMovement, RISING);

  // initialize the LED pin as an output:
  pinMode (ledPin, OUTPUT);

  touchAttachInterrupt(touchPin, gotTouch1, threshold);

  // Attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(WIFI_SSID);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // bot.sendMessage(CHAT_ID, "Bot started up", "");
}

void loop() {
  if(motionDetected){
    bot.sendMessage(CHAT_ID, "Motion detected!!", "");
    Serial.println("Motion Detected");
    motionDetected = false;
  }

  // // read the state of the pushbutton value:
  // touchValue = touchRead(touchPin);
  // Serial.print(touchValue);
  // // check if the touchValue is below the threshold
  // // if it is, set ledPin to HIGH
  // if(touchValue < threshold){
  //   // turn LED on
  //   digitalWrite(ledPin, HIGH);
  //   Serial.println(" - LED on");

  //   bot.sendMessage(CHAT_ID, "Motion detected!!", "");
  //   Serial.println("Motion Detected");
  //   motionDetected = false;
  // }
  // else{
  //   // turn LED off
  //   digitalWrite(ledPin, LOW);
  //   Serial.println(" - LED off");
  // }

  if(touch1detected){
    touch1detected = false;
    Serial.println("Touch 1 detected");
    // turn LED on
    digitalWrite(ledPin, HIGH);
    bot.sendMessage(CHAT_ID, "Touch 1 detected!", "");
  }

    // turn LED off
    digitalWrite(ledPin, LOW);

  // Serial.println(touchRead(T0));  // get value using T0
  delay(500);
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}