#include <Arduino.h>
#include <WiFi.h>
#include <WebSockets.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>

const char* gameStatus = "ready"; // temportaty hardcode!!

// Daadwerkelijk ding
const char* ssid = "battlebot_nl";
const char* password = "battlebot_nl";
const char* ip = "battlebot1.serverict.nl";
const int port = 3003;

// Youri hotspot
//const char* ssid = "Wifi is saai";
//const char* password = "2much4u2day"; 
//const char* ip = "192.168.179.155";
//const int port = 33003;

StaticJsonDocument<200> doc_tx;
StaticJsonDocument<200> doc_rx;

WebSocketsClient webSocket;

int interval = 5000;
unsigned long previousMillis = 0;


void setup(){
  Serial.begin(115200);
  
  WiFi.begin(ssid, password);
  Serial.println("Establishing connection to WiFi with SSID: " + String(ssid));
  
  while(WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.println("Connecting to WiFi...");
   }
  
  Serial.println("Connected to the WiFi network: ");
  Serial.println(WiFi.localIP());
  
  webSocket.begin(ip, port);
  webSocket.onEvent(webSocketEvent);

  String jsonLoginString = "";
  JsonObject object = doc_tx.to<JsonObject>();
  object["action"] = "login";
  object["id"] = "84:CC:A8:7A:A2:A8";
  serializeJson(doc_tx, jsonLoginString);
  Serial.println(jsonLoginString);
  webSocket.sendTXT(jsonLoginString);
}

void loop(){
  webSocket.loop();

  unsigned long now = millis();
  if((unsigned long)(now - previousMillis) > interval){
    String jsonString = "";
    JsonObject object = doc_tx.to<JsonObject>();
    object["status"] = gameStatus;
    object["isDriving"] = false;
    object["acceleration"] = 3;
    serializeJson(doc_tx, jsonString);
    Serial.println(jsonString);
    webSocket.sendTXT(jsonString);

    previousMillis = now;
  }
}

void webSocketEvent(WStype_t type, uint8_t* payload, size_t length){
  switch (type){
    case WStype_DISCONNECTED:
      Serial.println("Disconnected from the websocket");
      break;
    case WStype_CONNECTED:
      Serial.println("Connected to the websocket");
      break;
    case WStype_PING:
      Serial.printf("[WSc] get ping\n");
      break;
    case WStype_TEXT:
      DeserializationError error = deserializeJson(doc_rx, payload);
      if(error){
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
      }
      else{
        const char* loggedin = doc_rx["loggedin"];
        const char* action = doc_rx["action"];
        const char* game = doc_rx["game"];

        Serial.println("Received info: ");
        Serial.println("Logged in: " + String(loggedin));
        Serial.println("Action: " + String(action));
        Serial.println("Game: " + String(game));

        if(action == "prepare" && gameStatus == "in_game"){
          const char* returnError = "ALREADY_PLAYING_GAME";
          handleError;
        }

        if(game != "maze" || game != "butler" || game != "race"){
          const char* returnError = "GAME_NOT_FOUND";
          handleError;
        }

        if(action == "prepare"){
          if(game == "maze"){
            startMaze;
          }
          if(game == "butler"){
            startButler;
          }
          if(game == "race"){
            startRace;
          }
        }
      }
      Serial.println("");
      break;
   }
}

void handleError(char returnError){
  String jsonErrorString = "";
  JsonObject object = doc_tx.to<JsonObject>();
  object["error"] = returnError;
  serializeJson(doc_tx, jsonErrorString);
  Serial.println(jsonErrorString);
  webSocket.sendTXT(jsonErrorString);
}

// send status true + game name back if successful
void startMaze(){
  
}

void startButler(){
  
}

void startRace(){
  
}
