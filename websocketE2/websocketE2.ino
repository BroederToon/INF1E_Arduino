#include <Arduino.h>
#include <WiFi.h>
#include <WebSockets.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>
#include <analogWrite.h>
#include "Race.h"
#include "Butler.h"
#include "Maze.h"

int MRB = 16;
int MRF = 17;
int MLF = 5;
int MLB = 18;

//gets set to true if the game has started
boolean started = false;
//game currently being prepared
String currentGame = "";

//last direction a hole was seen in in the butler game
int holeDirectionButler = 0;

String gameStatus = "ready"; // temportaty hardcode!!

const char* ssid = "Wifi is saai";
const char* password = "2much4u2day";
const char* ip = "battlebot1.serverict.nl";
const int port = 33003;
int connectionAttempts = 0;

const char* macAddress = "84:CC:A8:7A:AF:D8";

StaticJsonDocument<200> doc_tx;
StaticJsonDocument<200> doc_rx;

WebSocketsClient webSocket;

int interval = 5000;
unsigned long previousMillis = 0;


void setup(){
  pinMode(MLF,OUTPUT);
  pinMode(MLB,OUTPUT);
  pinMode(MRF,OUTPUT);
  pinMode(MRB,OUTPUT);
  
  Serial.begin(115200);
  Adafruit_SSD1306 display(128, 64, &Wire, 4);
  Adafruit_VL53L0X lox = Adafruit_VL53L0X();
  
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;  // Don't proceed, loop forever
  }
  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while (1)
      ;
  }

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.display();
  
  WiFi.begin(ssid, password);
  Serial.println("Establishing connection to WiFi with SSID: " + String(ssid));
  
  while(WiFi.status() != WL_CONNECTED){
    delay(1000);
    //too many attempts requires a reset
    if(connectionAttempts > 8){
      ESP.restart();
    }
    Serial.println("Connecting to WiFi...");
    connectionAttempts++;
   }
  
  Serial.println("Connected to the WiFi network: ");
  Serial.println(WiFi.localIP());
  
  webSocket.begin(ip, port);
  webSocket.onEvent(webSocketEvent);
}

void loop(){
  if(started){
      if(currentGame == "race"){
      if(startRace()){
        String jsonRaceString = "";
        JsonObject object = doc_tx.to<JsonObject>();
        object["status"] = "finished";
        object["game"] = "race";
        serializeJson(doc_tx, jsonRaceString);
        Serial.println(jsonRaceString);
        webSocket.sendTXT(jsonRaceString);
        currentGame = "";
    }
      }

    if(currentGame == "maze"){
      if(startMaze()){
        String jsonRaceString = "";
        JsonObject object = doc_tx.to<JsonObject>();
        object["status"] = "finished";
        object["game"] = "maze";
        serializeJson(doc_tx, jsonRaceString);
        Serial.println(jsonRaceString);
        webSocket.sendTXT(jsonRaceString);
        currentGame = "";
      }
    }

      //for explanation startButler() return type see butler.h
      if(currentGame == "butler"){
        int butlerResult = startButler(holeDirectionButler);
      if(butlerResult == -1){
        String jsonRaceString = "";
        JsonObject object = doc_tx.to<JsonObject>();
        object["status"] = "finished";
        object["game"] = "butler";
        serializeJson(doc_tx, jsonRaceString);
        Serial.println(jsonRaceString);
        webSocket.sendTXT(jsonRaceString);
        currentGame = "";
      }else{
        holeDirectionButler = butlerResult;
      }
      }
     }
    
    
  
  webSocket.loop();

  unsigned long now = millis();
  if((unsigned long)(now - previousMillis) > interval){
    webSocket.sendTXT("{\"status\": \"" + gameStatus + "\",\"isDriving\": " + "false" + ",\"acceleration\":" + "3" + "}");
    previousMillis = now;
  }
}

void webSocketEvent(WStype_t type, uint8_t* payload, size_t length){
  switch (type){
    case WStype_DISCONNECTED:{
      Serial.println("Disconnected from the websocket");
      break;
    }
    case WStype_CONNECTED:{
      String jsonLoginString = "";
      JsonObject object = doc_tx.to<JsonObject>();
      object["action"] = "login";
      object["id"] = macAddress;
      serializeJson(doc_tx, jsonLoginString);
      Serial.println(jsonLoginString);
      webSocket.sendTXT(jsonLoginString);
      Serial.println("Connected to the websocket");
      break;
    }
    case WStype_PING:{
      Serial.printf("[WSc] get ping\n");
      break;
    }
    case WStype_TEXT: {
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

        if(String(action) == "prepare"){
          if(String(game) == "maze"){
            String jsonRaceString = "";
            JsonObject object = doc_tx.to<JsonObject>();
            object["status"] = true;
            object["game"] = "maze";
            serializeJson(doc_tx, jsonRaceString);
            Serial.println(jsonRaceString);
            webSocket.sendTXT(jsonRaceString);
            currentGame = "maze";
          }
          if(String(game) == "butler"){
            String jsonRaceString = "";
            JsonObject object = doc_tx.to<JsonObject>();
            object["status"] = true;
            object["game"] = "butler";
            serializeJson(doc_tx, jsonRaceString);
            Serial.println(jsonRaceString);
            webSocket.sendTXT(jsonRaceString);
            currentGame = "butler";
          }
          if(String(game) == "race"){
            String jsonRaceString = "";
            JsonObject object = doc_tx.to<JsonObject>();
            object["status"] = true;
            object["game"] = "race";
            serializeJson(doc_tx, jsonRaceString);
            Serial.println(jsonRaceString);
            webSocket.sendTXT(jsonRaceString);
            currentGame = "race";
          }
        }

        if(String(action) == "start"){
            started = true;
          
        }
      }
      break;
    }
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
