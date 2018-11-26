#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

// Set these to run example.
#define FIREBASE_HOST "jogodavelha-e0bcf.firebaseio.com"
#define FIREBASE_AUTH "KpY56HlZz8m163RfjNXQwporGiHZwFf6aEtEsaQo"
#define WIFI_SSID "UFRN - EAJ"
#define WIFI_PASSWORD ""

//
IPAddress ip(10, 77, 12, 33);
IPAddress dns(8, 8, 8, 8);
IPAddress gateway(10, 77, 12, 1);
IPAddress subnet(255, 255, 248, 0);

/*
  ip informattica  10,77,12,33
  gateway informatica 12,1
  ip container 10,77,32,31
  gateway container 32,1

*/

int modo = 0;
int mudou;
int atual;
int cont = 0;

StaticJsonBuffer<1000> jsonBuffer;

// Functions
void enviarDistanciaFirebase();
void modoManual();
void modoEstacionamento();
void modoMapeamento();
String verificaModo();
void wifiTryConnect();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  wifiTryConnect();
  WiFi.config(ip, dns, gateway, subnet);
  while (!Serial) {
    ;
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println("loop");

  // get value
  /*
    Serial.print("Valor: ");
    Serial.println(Firebase.getString("jogo/"));
    delay(1000);
  */

  /*
     // set string value
    Firebase.pushFloat("jogo", 2.1);
    // handle error
    if (Firebase.failed()) {
        Serial.print("setting /message failed:");
        Serial.println(Firebase.error());
        return;
    }
  */

  //Setar um novo objeto ao firebase

  /*JsonObject& obj = jsonBuffer.createObject();
    obj["coluna"] = 0;
    obj["linha"] = cont++;

    Firebase.push("mapeamento", obj);
    if (Firebase.failed()) {
    Serial.print("setting /message failed:");
    Serial.println(Firebase.error());
    return;
    }*/


  /*if (verificaModo() == "mapeamento") {
    Serial.println("7-");
    atual = 1;
    enviarDistanciaFirebase();
    } else if (verificaModo() == "manual") {
    Serial.println("8-");
    atual = 2;
    modoManual();
    } else if (verificaModo() == "autonomo") {
    atual = 3;
    }*/

  /*
    if (atual == 1) {
    Serial.println("mapeamento");
    modo++;
    } else if (atual == 2) {
    Serial.println("manual");
    modo++;
    } else if (atual == 3) {

    if (modo == 0) {
      Serial.println("autonomo");
      mudou = false;
    } else {
      mudou = true;
    }

    modo++;
    }


    Firebase.setInt("distancia", 10);
    if (Firebase.failed()) {
    Serial.print("setting /message failed:");
    Serial.println(Firebase.error());
    return;
    }
  */

  String modo = verificaModo();
  if (modo == "manual") {
    Serial.println("Modo Manual ativado");
    modoManual();
  } else if (modo == "estacionamento")
    Serial.println("Modo Estacionamento ativado");
  else
    Serial.println("Modo Mapeamento ativado");


  delay(1000);
}


void enviarDistanciaFirebase() {

  /*
    JsonObject& obj = jsonBuffer.createObject();
    obj["coluna"] = 3;
    obj["linha"] = 0;
    obj["valor"] = false;

    Firebase.push("jogo", obj);
    if (Firebase.failed()) {
      Serial.print("setting /message failed:");
      Serial.println(Firebase.error());
      return;
    }

    Firebase.pushInt("distancia", distancia);
    if (Firebase.failed()) {
    Serial.print("setting /message failed:");
    Serial.println(Firebase.error());
    return;
    }
  */

  /*int distancia;

    if (Serial.available() > 0) {
    //valorRecebido = (byte)Serial.read();
    distancia = Serial.parseInt();
    //Serial.print("valor recebido: ");
    //Serial.println(distancia);
    //enviarDistanciaFirebase(distancia);
    }

    Firebase.setInt("distancia", distancia);
    if (Firebase.failed()) {
    Serial.print("setting /message failed:");
    Serial.println(Firebase.error());
    return;
    }*/

}

String verificaModo() {
  return Firebase.getString("/modo");
}

void modoManual() {
  boolean direcao_direita = Firebase.getBool("/carro/direita");
  boolean direcao_esquerda = Firebase.getBool("/carro/esquerda");
  boolean direcao_frente = Firebase.getBool("/carro/frente");
  boolean direcao_parar = Firebase.getBool("/carro/parar");

  /*
   * 00 - Manual
   * 01 - Fuzzy
   * 10 - Mapeamento
   * 
   * Direção:
   * 00 - Frente
   * 01 - Trás
   * 10 - Direita
   * 11 - Esquerda
   * 
   * Exemplo:
   * 0000 - Manual e frente
   * 0010 - Manual e direita
   */

  if (direcao_direita == 1) {
    if (direcao_frente == 1)
      //Serial.println("Frente para há Direita");
      Serial.println(0000);
      Serial.println(0010);
    else
      Serial.println(0010);
      //Serial.println("Para há direita");
  } else if (direcao_esquerda == 1) {
    if (direcao_frente == 1)
      //Serial.println("Frente para há Esquerda");
      Serial.println(0000);
      Serial.println(0011);
    else
      //Serial.println("Para há Esquerda");
      Serial.println(0011);
  } else if (direcao_frente == 1) {
    if (direcao_esquerda == 1)
      //Serial.println("Frente para há Esquerda");
      Serial.println(0000);
      Serial.println(0011);
    else if (direcao_direita == 1)
      //Serial.println("Frente para há Direita");
      Serial.println(0000);
      Serial.println(0010);
    else
      Serial.println(0000);
  } else {
    Serial.println(0100);
  }

}

void wifiTryConnect() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
}
