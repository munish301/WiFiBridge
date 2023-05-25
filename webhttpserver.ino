
String ExecuteCommands(String str_json) {
    int array_count = 0;
    digitalWrite(LED_COM, HIGH);
    DynamicJsonDocument doc(5000);
    DeserializationError error = deserializeJson(doc, str_json);
  
    String str_response = "{\"data\":[";   
    JsonObject postObj = doc.as<JsonObject>();  
    Serial.print("Data : ");Serial.println(postObj["data"].as<String>());
//    if(server.method() == HTTP_POST){     
    if(postObj.containsKey("data")){
      JsonArray array = postObj["data"].as<JsonArray>();
      int array_size = array.size();
      for(JsonVariant v : array) {
        JsonObject object = v.as<JsonObject>(); 
  
         uint16_t _slave_id  = (uint16_t)object["slave"].as<int>();
         uint16_t _reg_addr  = (uint16_t)object["reg"].as<int>();
         uint16_t _value     = (uint16_t)object["val"].as<int>();
      
         uint8_t u8_err_code=0;
         SERIALX.write(_slave_id); 
         SERIALX.write(_reg_addr); 
         SERIALX.write(_value);
  
         str_response += "{";
         str_response += "\"slave\":";                 
         str_response += _slave_id;
         str_response += ",\"reg\":";                 
         str_response += _reg_addr;
         str_response += ",\"val\":";                 
         str_response += _value;
         str_response += ",\"err\":";                 
         str_response += u8_err_code;
         str_response += "}"; 
        if(array_count >= 0){
          if(array_count < array_size-1){
            str_response += ",";
          }
        }
        array_count++;          
      }     
    }else{
      str_response += "{";
      str_response += "\"err\":5";
      str_response += ",\"message\":\"Invalid JSON\"";                
      str_response += "}";          
      Serial.print(F("NO SLAVE KEY"));
    }
//    }else{
//        str_response += "{";
//        str_response += "\"err\":6";
//        str_response += ",\"message\":Invalid METHOD";                
//        str_response += "}";            
//        Serial.print(F("HTTP Method: NOT POST"));
//    } 
    //}// end else if (error) {
    str_response += "]}"; 
    Serial.println(str_response);
    digitalWrite(LED_COM, LOW);
    return str_response;  
}

#ifdef USE_HTTP_PROTOCOL
  void handleRoot() { 
    int array_count                         = 0;
    String str_response                     = "raman";
    for(uint8_t i = 0; i < server.args(); i++) {
      Serial.print(" Get ");
      Serial.print(server.argName(i));Serial.print(" : ");Serial.println(server.arg(i));
    }
    String str_json = server.arg("plain");
    Serial.print("str_json : ");
    Serial.println(str_json);
    ExecuteCommands(str_json);
    server.send(200, "plain/text", str_response);
  }// end void handleRoot()
  
  
  void handleNotFound() {
    
    String message = F("File Not Found\n\n");

    message += F("URI: ");
    message += server.uri();
    message += F("\nMethod: ");
    message += (server.method() == HTTP_GET) ? F("GET") : F("POST");
    message += F("\nArguments: ");
    message += server.args();
    message += F("\n");
  
    for (uint8_t i = 0; i < server.args(); i++) {
      message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    }
  
    server.send(404, F("text/plain"), message);
  }
  
  void initHttpServer(){
    server.on(F("/"), HTTP_ANY, handleRoot);
    server.on(F("/inline"), []() {
      server.send(200, F("text/plain"), F("SERVER START"));
    });
  
    server.onNotFound(handleNotFound);
    server.begin();
  }
#endif
