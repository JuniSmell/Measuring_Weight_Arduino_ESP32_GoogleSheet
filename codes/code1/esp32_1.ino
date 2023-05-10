#include <WiFi.h>
#include <HTTPClient.h>

#define RXD1 16
#define TXD1 17

const char* ssid = "wifi名稱"; //輸入wifi ssid
const char* password = "wifi密碼"; //輸入wifi 密碼

// The URL key of GAS of Google sheet
String GAS = "GAS部屬後會獲得網址";

const byte numChars=32;
char receivedChars[numChars];

String String0; //從Arduino 接收到的String
String String1; //String0的前半部，代表weight
String String2; //String0的後半部，代表num
int weight;
int num;

void setup() {
  Serial.begin(115200);
  Serial1.begin(9600, SERIAL_8N1, RXD1, TXD1);

  WiFi.begin(ssid, password);
  //wifi 初始化
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connecting to ");
  Serial.println(ssid);
  Serial.println("");
  Serial.println("STAIP address: ");
  Serial.println(WiFi.localIP());  
}

void loop() {
  //wifi connected enter
  if ((WiFi.status() == WL_CONNECTED)) {
    // start working..
    Serial.println("=================================");
    //持續監聽序列埠，若有string傳進來，將內容切成weight與num
    while(true){
      if (Serial1.available()){
        if(Serial1.available()){
          Serial.println("The message from Arduino Uno:");
          String0 = Serial1.readString();
          String1 = String0.substring(0, String0.indexOf("\n"));
          String2 = String0.substring(String0.indexOf("\n") + 1, String0.length());
          weight = String1.toInt();
          num = String2.toInt();
          break;
        }
      }
    }
  }
  
  String GASurl = GAS + "?weight="+String((int)weight)+"&num="+String((int)num);
  HTTPClient http;
  Serial.print("===HTTP Send data to Google===\n");
  http.begin(GASurl);
  Serial.print("===HTTP GET Status===\n");
  int httpCode = http.GET(); //Get status
          if(httpCode > 0) {
            Serial.printf("HTTP to get Feeback status: %d\n", httpCode);
            if(httpCode == HTTP_CODE_OK) {
                String payload = http.getString();
                Serial.println(payload); //印出feedback字串
            }
            else {
            Serial.printf("HTTP to get Feedback failed, error: %s\n", http.errorToString(httpCode).c_str());
            }
          Serial.println("=================================");
          http.end();
        }
    delay(30000);//30秒傳送一次，傳送資料最少20~30秒，勿低於30秒   
}
