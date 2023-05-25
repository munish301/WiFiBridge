
#define IS_DEBUG  

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <StringTokenizer.h>
#include <ArduinoJson.h>
#include <ArduinoWebsockets.h>

// declare reset fuction at address 0
void(* resetFunc) (void)              = 0;


#define SUNIL_SIR_HOME_LUDHIANA       1
#define RAJKUMAR_HOME_LUDHIANA        2
#define PP176_MOHALI                  3

#define OLD_CAN_BUS_PCB               1
#define NEW_WIFI_TO_UART_PCB          2

//#define USE_HTTP_PROTOCOL           //WebSocket Server or Http Server

//#define CUSTOMER                    SUNIL_SIR_HOME_LUDHIANA 
//#define CUSTOMER                    RAJKUMAR_HOME_LUDHIANA 
//#define CUSTOMER                    PP176_MOHALI 

#if(CUSTOMER == SUNIL_SIR_HOME_LUDHIANA)
  #define PCB_USED                    OLD_CAN_BUS_PCB
  #define IP_3                        1
  const char* wifiSSID                = "Airel_9815510062";
  const char* wifiPSWD                = "air14405";   
#elif(CUSTOMER == RAJKUMAR_HOME_LUDHIANA)
  #define PCB_USED                    OLD_CAN_BUS_PCB
  #define IP_3                        1
  const char* wifiSSID                = "Raj amar";
  const char* wifiPSWD                = "raj@1969";   
#elif(CUSTOMER == PP176_MOHALI)
  #define PCB_USED                    NEW_WIFI_TO_UART_PCB
  #define IP_3                        1
  const char* wifiSSID                = "ElkraftHa";
  const char* wifiPSWD                = "afbuteba@409"; 
#else
  const char* wifiSSID                = "compac322";
  const char* wifiPSWD                = "afbuteba@322";
  #define PCB_USED                    NEW_WIFI_TO_UART_PCB
  #define IP_3                        29  
#endif

uint8_t hb_count                      = 0;
unsigned long hb_interval             = 500;
unsigned long current_millis          = 0;
unsigned long hb_previousMillis       = 0;

#if(PCB_USED == OLD_CAN_BUS_PCB)
  const int LED_HB                    = 14; 
  const int LED_COM                   = 26;
  const int LED_WIFI                  = 32;
  HardwareSerial                      Serial4(1);
  #define  SERIALX                    Serial4
#else   //white color square shaped box
  const int LED_HB      = 27; 
  const int LED_COM     = 26;
  const int LED_WIFI    = 25;
  #define  SERIALX      Serial2
#endif

IPAddress myIP(192, 168, IP_3, 200);
IPAddress myGW(192, 168, IP_3, 1);
IPAddress mySN(255, 255, 255, 0);


#ifdef USE_HTTP_PROTOCOL
  WebServer server(8081);
#else
  using namespace websockets;
  WebsocketsServer server;
#endif

  
