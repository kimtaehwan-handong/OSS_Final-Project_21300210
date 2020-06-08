#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"

//GPIO Pin mapping 부분, nodemcu와 아두이노간 pin이 달라서 초기 설정 필요
#define D0 16
#define D1 5
#define D2 4
#define D3 0
#define D4 2
#define D5 14
#define D6 12
#define D7 13
#define D8 15

const char* ssid = "echo"; // 사용하는 공유기 이름
const char* password = "123456789"; // 공유기 password 입력

// 개별 nodeMCU 설정시 수정해야하는 부분(숫자를 자신의 주어진 번호로 모두 수정) - 자신의 NodeMCU NAME
//Publish를 위한 Topic
const char* nodeName = "iotClient1";
const char* DHTTopic = "iot/1/DHT22";
const char* cdsTopic = "iot/1/CDS";
const char* doorTopic = "iot/1/DOOR";
//Subscribe하기 위한 Topic
const char* topic1 = "iot/1"; //nodeMCU Topic
const char* topic2 = "iot/all/#"; // #로 하면 모든 토픽으로부터 수신을 한다.
// 해당코드에서 1을 자신의 학번으로 수정하면 됨.


const char* server = "192.168.0.27"; //MQTT broker address(자신의 Raspberry Pi3 IP), 노트북을 통한 모바일 핫스팟에서 IP check

WiFiClient wifiClient; //클라이언트로 작동

int ledPin = D0; 
int dhtPin = D3;
int doorPin = D4;
int cdsPin = A0;

// 온습도 센서 설정
#define DHTTYPE DHT22
DHT dht(dhtPin, DHTTYPE);

void callback(char* topic, byte* payload, unsigned int length);

PubSubClient client(server, 1883, callback, wifiClient);

void callback(char* topic, byte* payload, unsigned int length){ //Web으로 부터 수신한 Message에 따른 동작 제어 함수
  char message_buff[100]; //initialise storage buffer
  String msgString;
  int i = 0;

  Serial.println("Message arrived: topic: " + String(topic));
  Serial.println("Length: "+ String(length,DEC));

  //create character buffer with ending null terminator (string)
  for(i=0; i<length; i++){
    message_buff[i] = payload[i];
  }
  message_buff[i]= '\0';

  msgString = String(message_buff);
  Serial.println("Payload: "+ msgString);
  
  int led_state = digitalRead(ledPin);
  
  //전송된 메시가 "led"이면 LED를 받을 때마다 켜고 끈다.(토글)
  if (msgString == "led"){
    digitalWrite(ledPin, !led_state);
    Serial.println("Switching LED");
  }else if(msgString =="ledon"){
    digitalWrite(ledPin, HIGH);
  }else if(msgString =="ledoff"){
    digitalWrite(ledPin, LOW);
  }
}

void setup() {
  Serial.begin(115200);
  delay(10);
  
  dht.begin();

  
  pinMode(ledPin, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  digitalWrite(ledPin, LOW);   //LED off

  pinMode(cdsPin,INPUT);

  pinMode(doorPin, INPUT);


  InitWiFi();

  client.setServer(server, 1883);
  
  //Connection to broker
  if (client.connect(nodeName)){
    //client.publish("nodemcu", "Hello World");
    client.subscribe(topic1);
    client.subscribe(topic2);//Subscribe 할 Topic
  }
}

float temp, tempf, humi;
int lightValue;
int doorValue;
char data[80]; //publishing 할때 사용

void loop() {  
  delay(1000);
  if(!client.connected()){
    reconnect();
  }

  humi = dht.readHumidity();
  temp = dht.readTemperature();   
  tempf = dht.readTemperature(true);
        
  if(isnan(humi) || isnan(temp) || isnan(tempf))
      Serial.println("Failed to read from DHT sensor!");
      
  String payload_dht = "{";
  payload_dht += "Humidity : ";
  payload_dht += humi;   
  payload_dht += ", ";
  payload_dht += "Temperature : ";
  payload_dht += temp;
  payload_dht += "'C";
  payload_dht += "}";
  payload_dht.toCharArray(data, (payload_dht.length() + 1));
  client.publish(DHTTopic, data);
    
  lightValue = analogRead(cdsPin);
  String payload_light = "{";
  payload_light += "Light : ";
  payload_light += lightValue;
  payload_light += "}";
  payload_light.toCharArray(data, (payload_light.length() + 1));
  client.publish(cdsTopic, data);



  doorValue = analogRead(doorPin);

  String payload_door = "{";
  payload_door += "Door : ";
  if(doorValue >500)
    payload_door += "Open";
  else
    payload_door += "Close";
  payload_door += "}";
  payload_door.toCharArray(data, (payload_door.length() + 1));
  client.publish(doorTopic, data);

  Serial.println(humi);
  Serial.println(temp);

  Serial.println(lightValue);
  Serial.println(payload_door);

  
  client.loop();
}

void InitWiFi(){
  Serial.println();//시리얼모니터창에 보기 좋게 줄을 바꿈
  Serial.println();
  Serial.println("Connectiong to WiFi..");
  // attempt to connect to WiFi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.println(".");
  }
  // 접속이 되면 출력
  Serial.println();
  Serial.println("Connected to AP");
  //접속 정보를 출력
  Serial.println(WiFi.localIP());
}

void reconnect(){
  int status;
  //네트워크 접속
  while (!client.connected()){
    status = WiFi.status();
    if( status != WL_CONNECTED) {
      WiFi.begin(ssid, password);
      while(WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.println(".");
       }
        Serial.println("Connected to AP again");
     }
     delay(5000); 
  }//end while
}//Network 접속 확인 함수
