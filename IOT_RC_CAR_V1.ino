/* IOT_RC_CAR_V1
 * By Pie   
 
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
 
              //RC CAR CONTROLLERS

#define PWMA   12          // Enable/speed motors   (Right/Left)          GPIO12 (D6)
#define IN_1   14          // tb6612FNG IN_1 motors (Right/Left)          GPIO14 (D5)
#define IN_2   2           // tb6612FNG IN_2 motors (Right/Left)          GPIO2  (D4)

#define PWMB   5           // Enable/speed motors   (Front/Back)          GPIO5  (D1)
#define IN_3   4           // tb6612FNG IN_3 motors (Front/Back)          GPIO4  (D2)
#define IN_4   0           // tb6612FNG IN_4 motors (Front/Back)          GPIO0  (D3)
             

       
#include <ESP8266WiFi.h>                          // Include the Wi-Fi library.
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h>                     // Include the Wi-Fi-Multi library.
#include <ESP8266WebServer.h>                     // Include the WebServer library.

ESP8266WiFiMulti wifiMulti;                       // Create an instance of the ESP8266WiFiMulti class, called 'wifiMulti'.
ESP8266WebServer server(80);                      // Create a webserver object that listens for HTTP request on port 80.

String command;                                   //String to store app command state.
int speedCar = 800;              


 void setup() {
 
 pinMode(PWMA, OUTPUT);                             //Right/Left
 pinMode(PWMB, OUTPUT);                             //Front/Back  
 pinMode(IN_1, OUTPUT);                             //Right/Left
 pinMode(IN_2, OUTPUT);                             //Right/Left
 pinMode(IN_3, OUTPUT);                             //Front/Back
 pinMode(IN_4, OUTPUT);                             //Front/Back

Serial.begin(115200);                              // Start the Serial communication to send messages to the computer.
delay(10);
Serial.println('\n');

// Connecting WiFi

wifiMulti.addAP("ssid", "password");              // Wi-Fi networks you want to connect to. It will conect to the strongest network availaible
wifiMulti.addAP("----", "----");
wifiMulti.addAP("----", "----");

con();                                             //Con function is at the bottom after 'void loop(){.......}'
 
}  


void goAhead(){
      
      digitalWrite(IN_3, LOW);
      digitalWrite(IN_4, HIGH);
      analogWrite(PWMB, speedCar);
  }
      
void goBack(){
      
      digitalWrite(IN_3, HIGH);
      digitalWrite(IN_4, LOW);
      analogWrite(PWMB, speedCar);
  }

void goRight(){

       digitalWrite(IN_1, HIGH);
       digitalWrite(IN_2, LOW);
       analogWrite(PWMA,1023);
  }

void goLeft(){
  
      digitalWrite(IN_1, LOW);
      digitalWrite(IN_2, HIGH);
      analogWrite(PWMA,1023);

  }

void goAheadRight(){
      
      digitalWrite(IN_3, LOW);
      digitalWrite(IN_4, HIGH);
      analogWrite(PWMB, speedCar);
 
      digitalWrite(IN_1, HIGH);
      digitalWrite(IN_2, LOW);
      analogWrite(PWMA,1023);
   }

void goAheadLeft(){
      
      digitalWrite(IN_3, LOW);
      digitalWrite(IN_4, HIGH);
      analogWrite(PWMB, speedCar);

      digitalWrite(IN_1, LOW);
      digitalWrite(IN_2, HIGH);
      analogWrite(PWMA,1023);
  }

void goBackRight(){ 

      digitalWrite(IN_3, HIGH);
      digitalWrite(IN_4, LOW);
      analogWrite(PWMB, speedCar);

      digitalWrite(IN_1, HIGH);
      digitalWrite(IN_2, LOW);
      analogWrite(PWMA,1023);
  }

void goBackLeft(){ 

      digitalWrite(IN_3, HIGH);
      digitalWrite(IN_4, LOW);
      analogWrite(PWMB, speedCar);

      digitalWrite(IN_1, LOW);
      digitalWrite(IN_2, HIGH);
      analogWrite(PWMA,1023);
  }

void stopRobot(){  

      digitalWrite(IN_3, LOW);
      digitalWrite(IN_4, LOW);
      analogWrite(PWMB, 0);

      digitalWrite(IN_1, LOW);
      digitalWrite(IN_2, LOW); 
      analogWrite(PWMA, 0);
 }

 

void loop() {
     
     if (wifiMulti.run() != WL_CONNECTED) {                              //If rc car is not connected it will call "con" function.
        Serial.println(" Oops! You are disconnected ");
        delay(2000);
        Serial.println(" Reconnecting ..... ");
        con();                                                           //Con function
    }
   
    server.handleClient();
    
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
}
void HTTP_handleRoot(void) {

if( server.hasArg("State") ){
       Serial.println(server.arg("State"));
  }
  server.send ( 200, "text/html", "<h1>Connected to V1</h1>");
  delay(1);
}

void con() {                                                           //Con function
                                          
int i = 0;
while (wifiMulti.run() != WL_CONNECTED) {                   // Wait for the Wi-Fi to connect: scan for Wi-Fi networks, and connect to the strongest of the networks above.
delay(200);
Serial.print(++i);
Serial.print(' ');
}

Serial.println('\n');
Serial.print("You're Connected To Prototype ");
Serial.println(WiFi.SSID());                  // Tell us what network we're connected to.
Serial.print("This Is My IP Address:\t");
Serial.println(WiFi.localIP());               // Send the IP address of the ESP8266 to the computer.
 
 // Starting WEB-server 
     server.on ( "/", HTTP_handleRoot );
     server.onNotFound ( HTTP_handleRoot );
     server.begin();

 
}
