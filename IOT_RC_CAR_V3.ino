/* IOT_RC_CAR_V3
   By Pie
   aravindvallab@protonmail.com
   This program has some cool symbos to make it easy fo beginners (⌐▨_▨ ),(°╭╮°),◝(^ᗜ^)◜,(•◡•),😅,😂,😃,😄,😆,😉,😁,😊,😋,😌,😍,😏,😒,😓,😔,😖,😘,😚,😜,😝,😠,😡,😢,😣,😤😥.😨.😩.😪.😫.😭.😰.😱.😲.😳.😵.😷.😇.😈.😎.😐.😶.♪.♩.♫.♬.♭.♮.♯.☺,☹,シ,ッ,ツ,ヅ,⍢,ӧ
   HAPPY BUILDINGS ◝[*∇^]◜
   
   MIT License

   Copyright (c) 2021 Pie

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in all
   copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.
*/

//RC CAR CONTROLLER-------------------------------------------------------

#define PWMA   12          // Enable/speed motors   (Right/Left)          GPIO12 (D6)        for controlling speed
#define IN_1   14          // tb6612FNG IN_1 motors (Right/Left)          GPIO14 (D5)
#define IN_2   2           // tb6612FNG IN_2 motors (Right/Left)          GPIO2  (D4)

#define PWMB   5           // Enable/speed motors   (Front/Back)          GPIO5  (D1)        for controlling speed
#define IN_3   4           // tb6612FNG IN_3 motors (Front/Back)          GPIO4  (D2)
#define IN_4   0           // tb6612FNG IN_4 motors (Front/Back)          GPIO0  (D3)

//LIGHT CONTROLLERS

#define IN_5   13          // B RED LED                                     GPIO13 (D7)
#define IN_6   15          // F BLUE LED                                    GPIO15 (D8)
#define IN_7   16          // B YELLOW LED                                  GPIO16 (D0)

#define BLYNK_PRINT Serial

//TEMPERATURE---------------------------------------------------------------

int output;
float voltage;                                    // change in voltage = change in temperature
float tempC;                                      //temperature in celsius
float tempF;                                      //temperature in fahrenheit

//LIBRARY--------------------------------------------------------------------

#include <ESP8266WiFi.h>                          // Include the Wi-Fi library.
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h>                     // Include the Wi-Fi-Multi library.
#include <ESP8266mDNS.h>                          // Include the mDNS library
#include <ESP8266WebServer.h>                     // Include the WebServer library.
#include <ArduinoOTA.h>                           // Include OTA library. 
#include  <ESP8266WiFi.h>
#include  <BlynkSimpleEsp8266.h>

ESP8266WiFiMulti wifiMulti;                       // Create an instance of the ESP8266WiFiMulti class, called 'wifiMulti'.
ESP8266WebServer server(80);                      // Create a webserver object that listens for HTTP request on port 80.

//----------------configure on which pin the led is connected---------------
const int led_pin = 15;
//-----------------------------------------------------------------------------

// For BLYNK connection --------------------
char ssid[] = "ssid";
char pass[] = "password";
--------------------------------------------

// HTML Error Pages .  HTML code was converted too suppord to arduino ide by this site ( https://davidjwatts.com/youtube/esp8266/esp-convertHTM.html# )

String html_303 = "<!DOCTYPE html> <html> <head> </head> <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"> <!-- Add icon library --> <link rel=\"stylesheet\" href=\"https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.7.0/css/font-awesome.min.css\"> <style> body { background-image: url('https://user-images.githubusercontent.com/86643678/127834328-d48cde57-797c-4b74-a2cc-0cc850c02f78.png'); background-size: contain; background-repeat: no-repeat; background-attachment: fixed; background-position: center;} p{{text-align: left;}; } .button { border-radius: 10px; background-color: black; border:2px solid #d1fd1f; color: #FFFFFF; text-align: center; font-size: 20px; padding: 10px; width: 150px; transition: all 0.5s; cursor: pointer; margin: 0px; } .button span { cursor: pointer; display: inline-block; position: relative; transition: 0.5s; } .button span:after { content: '\\00bb'; position: absolute; opacity: 0; top: 0; right: -20px; transition: 0.5s; } .button:hover span { padding-right: 16px; } .button:hover span:after { opacity: 1; right: 0; } .btn { background-color: black; border: none; border-radius: 200px; color: white; padding: 12px 15px; font-size: 20px; cursor: pointer; text-align: center; position: relative; /* Adjust these values accordingly */ top: 5px; left: 5px; display: flex; } .btn:hover { background-color:royalblue ; } </style> <body> <h3>(>‿◠)✌️Let's get you back!</h3> <form action=\"http://192.168.247.106/\" method=\\\"get\\\" target=\\\"_blank\\\"> <button class=\"button\"><span>HOME</span></button> </form> <form action=\"https://github.com/HyperArx\\\" method=\\\"get\\\" target=\\\"_blank\\\"> <button class=\"btn\"><i class=\"fa fa-github\"> </form> </body> </html>";
String html_404 = "<!DOCTYPE html> <html> <head> <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"> <!-- Add icon library --> <link rel=\"stylesheet\" href=\"https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.7.0/css/font-awesome.min.css\"> <style> body {background-image: url('https://user-images.githubusercontent.com/86643678/127807009-082c3f5e-4249-4d11-b52e-6b2db776adf6.png'); background-size: contain; background-repeat: no-repeat; background-attachment: fixed; background-position: center;}. p{{text-align: left;}; } .btn { background-color: black; border: none; border-radius: 200px; color: white; padding: 12px 15px; font-size: 20px; cursor: pointer; text-align: center; display: flex; position: fixed; /* Adjust these values accordingly */ top: 40px; left: 10px; } .btn:hover { background-color:royalblue ; } </style> <body> <P>GITHUB</p> <form action=\"https://github.com/HyperArx\\\" method=\\\"get\\\" target=\\\"_blank\\\"> <button class=\"btn\"><i class=\"fa fa-github\"> </form> </body> </html>";
String html_400 = "<!DOCTYPE html> <html> <head> </head> <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"> <!-- Add icon library --> <link rel=\"stylesheet\" href=\"https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.7.0/css/font-awesome.min.css\"> <style> body { background-image: url('https://user-images.githubusercontent.com/86643678/127808081-8d811eb0-94b5-4dbd-86d2-a403f29a827d.jpg'); background-size: contain; background-repeat: no-repeat; background-attachment: fixed; background-position: center; } p{{text-align: left;}; } .button { border-radius: 10px; background-color: black; border:2px solid #d1fd1f; color: #FFFFFF; text-align: center; font-size: 20px; padding: 10px; width: 150px; transition: all 0.5s; cursor: pointer; margin: 0px; } .button span { cursor: pointer; display: inline-block; position: relative; transition: 0.5s; } .button span:after { content: '\\00bb'; position: absolute; opacity: 0; top: 0; right: -20px; transition: 0.5s; } .button:hover span { padding-right: 16px; } .button:hover span:after { opacity: 1; right: 0; } .btn { background-color: black; border: none; border-radius: 200px; color: white; padding: 12px 15px; font-size: 20px; cursor: pointer; text-align: center; display: flex; position: relative; /* Adjust these values accordingly */ top: 5px; left: 5px; } .btn:hover { background-color:royalblue ; } </style> <body> <h3>(>‿◠)✌️Let's get you back!</h3> <form action=\"http://192.168.247.106/\" method=\\\"get\\\" target=\\\"_blank\\\"> <button class=\"button\"><span>HOME</span></button> </form> <form action=\"https://github.com/HyperArx\\\" method=\\\"get\\\" target=\\\"_blank\\\"> <button class=\"btn\"><i class=\"fa fa-github\"> </form> </body> </html>";
String html_500 = "<!DOCTYPE html> <html> <head> <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"> <!-- Add icon library --> <link rel=\"stylesheet\" href=\"https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.7.0/css/font-awesome.min.css\"> <style> body {background-image: url('https://user-images.githubusercontent.com/86643678/127810603-a2d89e3c-878f-41f0-8679-bd3eb76ddb1a.png'); background-size: contain; background-repeat: no-repeat; background-attachment: fixed; background-position: center;}. p{{text-align: left;}; } .btn { background-color: black; border: none; border-radius: 200px; color: white; padding: 12px 15px; font-size: 20px; cursor: pointer; text-align: center; display: flex; position: fixed; /* Adjust these values accordingly */ top: 40px; left: 10px; } .btn:hover { background-color:royalblue ; } </style> <body> <P>GITHUB</p> <form action=\"https://github.com/HyperArx\\\" method=\\\"get\\\" target=\\\"_blank\\\"> <button class=\"btn\"><i class=\"fa fa-github\"> </form> </body> </html>";

String html_home = "<!DOCTYPE html><html><head><style>body {background-image: url('https://user-images.githubusercontent.com/86643678/128638180-c97163fe-964f-4fa1-8319-3b1164c95e34.jpg'); background-size: contain;background-repeat: no-repeat;background-attachment: fixed;background-position: center;}.button {border: none;padding: 12px 40px;text-align: center;text-decoration: none;display: inline-block;font-size: 16px;margin: 4px 2px;cursor: pointer;}.button1 {background-color: black; color: white; border: 3px solid #d1fd1f;border-radius: 16px}body {text-align: center;}</style></head><body><h2>Robot prototype V3</h2><p>by- HYPERARX</p><form action=\"/LED\" method=\"POST\"><button class=\"button button1\">Toggle Led!</button></form>";

-------------------------------------------------------------------------------
   
boolean led_state = false;

// function prototypes for HTTP handlers

void handleRoot();
void handleLED();
void handleNotFound();
//-----------------------------------------------------------------------------

char auth[] = " Write Your Auth Code ";            // Auth code given by BLYNK

//-----------------------------------------------------------------------------

String command;                                    //String to store app command state.
int speedCar = 400;                                //400 to 1023
int speed_Coef = 3;

void setup() {

  pinMode(PWMA, OUTPUT);                            //Right/Left
  pinMode(PWMB, OUTPUT);                            //Front/Back
  pinMode(IN_1, OUTPUT);                            //Right/Left
  pinMode(IN_2, OUTPUT);                            //Right/Left
  pinMode(IN_3, OUTPUT);                            //Front/Back
  pinMode(IN_4, OUTPUT);                            //Front/Back
  pinMode(IN_5, OUTPUT);                            //B RED LED (D7)
  pinMode(IN_6, OUTPUT);                            //F BLUE LED (D8)
  pinMode(IN_7, OUTPUT);                            //B YELLOW LED (D0)


  Serial.begin(115200);                             // Start the Serial communication to send messages to the computer. Note: upload speed for arduino is 9600 baud whereas in esp it is 115200 baud
  delay(10);
  Serial.println('\n');

  Blynk.begin(auth,ssid,pass);

  // Connecting WiFi
  // Add the wifi networks name and password you want ESP-01 to connect to. E.g: wifiMulti.addAP("HOME_WIFI", "504682@#$");
  // You may add or comment out number of networks you want ESP-01 to connect to.

  wifiMulti.addAP("Ssid", "Password");             // Wi-Fi networks you want to connect to.
  wifiMulti.addAP("----", "---");                  // It will connect to the strongest Network 
  wifiMulti.addAP("----", "---"); 
  espCon();                                          //Connect function (espCon) is at the bottom after 'void loop(){.......}'

}

unsigned long previousTime = millis();
const unsigned long interval = 1000;

void goAhead() {                                      //this function controlls the motor for moving forward and the led

  digitalWrite(IN_5, LOW);                            //RED LED

  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, HIGH);
  analogWrite(PWMB, speedCar);                        //for controlling speed
}

void goBack() {                                       //this function controlls the motor for moving backward and the led

  digitalWrite(IN_5, LOW);                            //RED LED
  digitalWrite(IN_7, HIGH);                           //YELLOW LED

  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_4, LOW);
  analogWrite(PWMB, speedCar);                        //for controlling speed
}

void goRight() {                                      //this function controlls the motor for moving right

  digitalWrite(IN_5, HIGH);                            //RED LED

  digitalWrite(IN_1, HIGH);
  digitalWrite(IN_2, LOW);
  analogWrite(PWMA, 1023);                            //turning speed will be full.If you want to change write {analogWrite(PWMA, speedCar);} in the place of {analogWrite(PWMA,1023);}
}

void goLeft() {                                       //this function controlls the motor for moving left

  digitalWrite(IN_5, HIGH);                            //RED LED

  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, HIGH);
  analogWrite(PWMA, 1023);                            //turning speed will be full.If you want to change write {analogWrite(PWMA, speedCar);} in the place of {analogWrite(PWMA,1023);}

}

void goAheadRight() {                                 //this function controlls the motor for moving forward and for turning right

  digitalWrite(IN_5, LOW);                            //RED LED

  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, HIGH);
  analogWrite(PWMB, speedCar);                        //for controlling speed

  digitalWrite(IN_1, HIGH);
  digitalWrite(IN_2, LOW);
  analogWrite(PWMA, 1023);                            //turning speed will be full.If you want to change write {analogWrite(PWMA, speedCar);} in the place of {analogWrite(PWMA,1023);}
}

void goAheadLeft() {                                  //this function controlls the motor for moving forward and for turning left

  digitalWrite(IN_5, LOW);                            //RED LED

  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, HIGH);
  analogWrite(PWMB, speedCar);                        //for controlling speed

  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, HIGH);
  analogWrite(PWMA, 1023);                            //turning speed will be full.If you want to change write {analogWrite(PWMA, speedCar);} in the place of {analogWrite(PWMA,1023);}
}

void goBackRight() {                                  //this function controlls the motor for moving backward and turning right

  digitalWrite(IN_7, HIGH);                           //YELLOW LED
  digitalWrite(IN_5, LOW);                            //RED LED

  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_4, LOW);
  analogWrite(PWMB, speedCar);                        //for controlling speed

  digitalWrite(IN_1, HIGH);
  digitalWrite(IN_2, LOW);
  analogWrite(PWMA, 1023);                            //turning speed will be full.If you want to change write {analogWrite(PWMA, speedCar);} in the place of {analogWrite(PWMA,1023);}
}

void goBackLeft() {                                   //this function controlls the motor for moving backward and turning right

  digitalWrite(IN_7, HIGH);                           //YELLOW LED
  digitalWrite(IN_5, LOW);                            //RED LED

  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_4, LOW);
  analogWrite(PWMB, speedCar);                        //for controlling speed

  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, HIGH);
  analogWrite(PWMA, 1023);                            //turning speed will be full.If you want to change write {analogWrite(PWMA, speedCar);} in the place of {analogWrite(PWMA,1023);}
}

void stopRobot() {                                    //this function is used to turn off the robot but the red led will be on until the robot goes forward or backward

  digitalWrite(IN_3, LOW);                            //OFF
  digitalWrite(IN_4, LOW);
  analogWrite(PWMB, 0);                               //OFF

  digitalWrite(IN_1, LOW);                            //OFF
  digitalWrite(IN_2, LOW);
  analogWrite(PWMA, 0);                               //OFF

  digitalWrite(IN_5, HIGH);                           //RED LED (D7) HIGH
  digitalWrite(IN_7, LOW);                            //YELLOW LED (D0) low
}

void temperatureAlert() {

  output = analogRead(A0);
  voltage = output * (5000.0 / 1024.0);
  tempC = (voltage - 500) / 100;
  tempF = (tempC * 9.0 / 5.0) + 32.0;
  yield();

}

void loop(void) {

  Blynk.run();

  if (wifiMulti.run() != WL_CONNECTED) {                               //If rc car is not connected it will call "con" function.
    stopRobot();
    Serial.println(" Oops! You are disconnected (°╭╮°) Wait!\n");
    delay(200);
    Serial.println(" Reconnecting ..... ");
    espCon();                                                           //Connect function
  }

  ArduinoOTA.handle();

  unsigned long diff = millis() - previousTime;
  if (diff > interval) {
    digitalWrite(IN_5, !digitalRead(IN_5));                            // Change the state of the LED
    previousTime += diff;
  }

  server.handleClient();                                               // Listen for HTTP requests from clients

  command = server.arg("State");
  if (command == "F") goAhead();
  else if (command == "B") goBack();
  else if (command == "L") goLeft();
  else if (command == "R") goRight();
  else if (command == "I") goAheadRight();
  else if (command == "G") goAheadLeft();
  else if (command == "J") goBackRight();
  else if (command == "H") goBackLeft();
  else if (command == "0") speedCar = 400;
  else if (command == "1") speedCar = 470;
  else if (command == "2") speedCar = 540;
  else if (command == "3") speedCar = 610;
  else if (command == "4") speedCar = 680;
  else if (command == "5") speedCar = 750;
  else if (command == "6") speedCar = 820;
  else if (command == "7") speedCar = 890;
  else if (command == "8") speedCar = 960;
  else if (command == "9") speedCar = 1023;
  else if (command == "S") stopRobot();

  yield();

  //temperatureAlert();                                                //Calling temperature function (temperatureAlert) for checking the temperature of circuit

  yield();

  // ESP shares the cpu resource. So if the loop function takes more time you may experience longer site load time. To prevent this I have added yield(). Basically it helps manage cpu resource between sketch execution and ESP networking
  if (led_state == true) {                                             // check if the buz_state is high and on the led
    digitalWrite(led_pin, HIGH);
    delay(1);
    yield();
    digitalWrite(led_pin, LOW);
    yield();
  }

}

// Handling Joystick data
BLYNK_WRITE(V1) {
  int x = param[0].asInt();
  int y = param[1].asInt();
  if (x == -1 && y == 0) {  //Backward
    goBack();
    Serial.println("B");
  } else if (x == 0 && y == 0) {  //Stay
    stopRobot();
    Serial.println("S");
  } else if (x == 1 && y == 0) {  //Forward
    goAhead();
    Serial.println("F");
  } else if (x == 0 && y == -1) {  //Left Turn
    goLeft();
    Serial.println("L");
  } else if (x == 0 && y == 1) {  //Right turn
    goRight();
    Serial.println("R");
  }
}

BLYNK_WRITE(V0) {
  int value = param.asInt();                                   //get value as integer
  Serial.println(value);
  analogWrite(PWMB, value);                                    //write PWM value to PWMB
}

BLYNK_WRITE(V2) {
  int x = param[0].asInt();
  int y = param[1].asInt();
  if (x == 0 && y == -1) {  //Left Turn
    goLeft();
    Serial.println("L");
  } else if (x == 0 && y == 1) {  //Right turn
    goRight();
    Serial.println("R");
  } else if (x == -1 && y == 0) {  //Backward
    goBack();
    Serial.println("B");
  } else if (x == 0 && y == 0) {  //Stay
    stopRobot();
    Serial.println("S");
  } else if (x == 1 && y == 0) {  //Forward
    goAhead();
    Serial.println("F");
  }
}


void handleRoot() {

  if ( server.hasArg("State") ) {
    Serial.println(server.arg("State"));
  }
  server.send(200, "text/html", html_home + "Current state: " + led_state + "<b></b><br><br><i>0 - Led off<br>1 - Led on</i></body></html>");
}

void handleLED() {                                                                   // If a POST request is made to URI /LED
  led_state = !led_state;                                                           // toogle the led state
  server.sendHeader("Location", "/");                                               // Add a header to respond with a new location for the browser to go to the home page again
  server.send(303);                                                                 // Send it back to the browser with an HTTP status 303 (See Other) to redirect
}

void handleNotFound() {
  server.send(400, "text/html", html_400);
  server.send(404, "text/html", html_400);
  server.send(500, "text/html", html_500);
  server.send(303, "text/html", html_303);
}

void mDNS() {

  if (MDNS.begin("rockinfreakshow.local")) {                                                // Start the mDNS responder for rockinfreakshow. In this case you can access Node_MCU by typping "YOUR CREATED NAME" on any browser connected to the same WiFi as the Node_MCU
    Serial.println("mDNS responder started ◝(^ᗜ^)◜");
    Serial.println("mDNS is rockinfreakshow.local");
  }  else  {
    Serial.println("Error setting up MDNS responder! (╥_╥)");
  }

}

void espCon() {                                                      //ESP CONNECT

  digitalWrite(IN_5, HIGH);                                          //RED LED

  Serial.println("Connecting ...");
  int i = 0;
  while (wifiMulti.run() != WL_CONNECTED) {                          // Wait for the Wi-Fi to connect: scan for Wi-Fi networks, and connect to the strongest of the networks above.
    Serial.print(++i); Serial.print(' ');
    digitalWrite(IN_6, !digitalRead(IN_6));
    digitalWrite(IN_5, !digitalRead(IN_6));
  }

  Serial.println('\n');
  Serial.print("(>‿◠)✌ You're Connected To Prototype V3 by ");
  Serial.println(WiFi.SSID());                                       // Tell us what network we're connected to.
  Serial.print("(⌐▨_▨ ) This Is My Cool IP Address:\t");
  Serial.println(WiFi.localIP());                                    // Send the IP address of the ESP8266 to the computer.
  Serial.print("😶 Tis is my MAC Address:\t");
  Serial.println(WiFi.macAddress());                                 // Tell us what is the Mac address

  //----------------------------------------------------------------------------------------------------------------------------------------------------------------------

  mDNS();

  //-----------------------------------------------------------------------------------------------------------------------------------------------------------------------

  // Starting WEB-server
  server.on("/", HTTP_GET, handleRoot);                               // Call the 'handleRoot' function when a client requests URl "/"
  server.on("/LED", HTTP_POST, handleLED);                            // Call the 'handleLED' function when a POST request is made to URI "/LED"
  server.onNotFound(handleNotFound);                                  // When a client requests an unknown URI (i.e. something other than "/"), call function "handleNotFound"
  server.begin();                                                     // Actually start the server
  Serial.println("HTTP server started");

  //ArduinoOTA -------

  ArduinoOTA.setHostname("Pie");
  ArduinoOTA.setPassword("147258");
  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Authentication Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("OTA ready");
  digitalWrite(IN_5, 1);

  digitalWrite(IN_5, HIGH);                            //RED LED
  digitalWrite(IN_6, LOW);                             //BLUE LED
  digitalWrite(IN_7, LOW);                             //YELLOW LED
}
