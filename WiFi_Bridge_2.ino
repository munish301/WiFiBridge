
#include "defs.h"
#define BAUD_RATE 115200   

void setup() {   
 
  pinMode(LED_HB,   OUTPUT);
  pinMode(LED_COM,  OUTPUT);
  pinMode(LED_WIFI, OUTPUT);
  
  digitalWrite(LED_HB,   LOW);
  digitalWrite(LED_COM,  LOW);
  digitalWrite(LED_WIFI, LOW);
 
  Serial.begin(115200);
  delay(100);
  Serial.println("Code Started");
    
  SERIALX.begin(BAUD_RATE); 
  SERIALX.setTimeout(1);   
  /////////////////////////////////////////

  WiFi.mode(WIFI_STA);   //Set Device Wi-Fi Mode
  Set_Static_IP(); 
  Connect_to_WiFi();

  
  current_millis = millis();
  hb_previousMillis = current_millis;

  #ifdef USE_HTTP_PROTOCOL
    initHttpServer();
  #else  
    initSocketServer();
    xTaskCreatePinnedToCore(
        taskOne,      // Function that should be called
        "TaskOne",    // Name of the task (for debugging)
        10000,               // Stack size (bytes)
        0,               // Parameter to pass
        1,                  // Task priority
        0,               // Task handle
        1          // Core you want to run the task on (0 or 1)
    ); 
  #endif
}

void loop() {

  current_millis = millis();
  if (current_millis - hb_previousMillis >= hb_interval) {
    hb_previousMillis = millis();
    digitalWrite(LED_HB, !digitalRead(LED_HB));
    hb_count++;
    if(hb_count >= 4){
      if(WiFi.status() != WL_CONNECTED) { 
        Connect_to_WiFi();
      }      
    }
  }
  #ifdef USE_HTTP_PROTOCOL
    server.handleClient();
  #endif
}

void  Set_Static_IP(){
  //Configures static IP address
  if(!WiFi.config(myIP, myGW, mySN)) {
    Serial.println("STA Failed to configure");
  }
}

void  Connect_to_WiFi()
{
  #ifdef IS_DEBUG
   Serial.println("Disconnected from WiFi access point");
  #endif
  WiFi.disconnect();
  digitalWrite(LED_HB,   HIGH); 
  digitalWrite(LED_WIFI, HIGH);
  digitalWrite(LED_COM,  HIGH);

  hb_count = 0;
  //Try to Connect with  Router SSID and Password
  #ifdef IS_DEBUG
    Serial.println("");
    Serial.println("Connecting with Router SSID");
  #endif 
  WiFi.begin(wifiSSID, wifiPSWD);
  
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    digitalWrite(LED_WIFI, !digitalRead(LED_WIFI));
    Serial.print(".");
    hb_count++;
    if(hb_count >= 10){
     resetFunc();      
    }
  }
    
  #ifdef IS_DEBUG
      Serial.println("");
      Serial.print("Connected with ");
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());
  #endif
  digitalWrite(LED_HB,   LOW);    
  digitalWrite(LED_WIFI, LOW);
  digitalWrite(LED_COM,  LOW);
}
