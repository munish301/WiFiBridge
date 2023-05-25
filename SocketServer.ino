

#ifndef USE_HTTP_PROTOCOL
  void taskOne( void * parameter ){
    while(1){
      WebsocketsClient client = server.accept();
      if(client.available()) {
        WebsocketsMessage msg = client.readBlocking();
        // log message
        Serial.print("Got Message: ");
        Serial.println(msg.data());
        String mssg = ExecuteCommands(msg.data());
        Serial.print("return_mssg: "); Serial.println(mssg);
        // return echo
        client.send(mssg);
        // close the connection
        //client.close();
      }
    }
  }

  void initSocketServer() {
    server.listen(8081);
    Serial.print("Is server live? ");
    Serial.println(server.available());
  }
#endif
