#include <LiquidCrystal.h>
#include "HX711.h"

//LCD使用到的PIN腳
const int pin_RS = 8; 
const int pin_EN = 9; 
const int pin_D4 = 4; 
const int pin_D5 = 5; 
const int pin_D6 = 6; 
const int pin_D7 = 7; 

//HX711使用到的PIN腳
const int DT_PIN = 12;
const int SCK_PIN = 11;

/*
重量的比例參數
HX711測到的是「電流」
將電流訊號轉換成重量的比例
由於沒有真實重量的值，下面的比例是我亂寫的
*/
const int scale_factor = 100;

//填入正確的PIN腳，建立LCD物件
LiquidCrystal lcd( pin_RS,  pin_EN,  pin_D4,  pin_D5,  pin_D6,  pin_D7);

//建立HX711物件
HX711 scale;

void setup() {
  Serial.begin(9600);
	scale.begin(DT_PIN, SCK_PIN);
  
  //稱重功能初始化，並將資訊顯示在LCD上
	lcd.begin(16, 2);
	lcd.setCursor(0,0);
	lcd.print("Measure Weight");
	lcd.setCursor(0,1);
	lcd.print("Initializing...");
	scale.set_scale(scale_factor);
	scale.tare();
  
  //稱重功能初始化完成，將資訊顯示在LCD上
  lcd.clear();
	lcd.setCursor(0,0);
	lcd.print("Measure Weight");
	lcd.setCursor(0,1);
	lcd.print("Finishing");
	delay(1000);  
}

void loop() {
  //weight是load cell與 HX711 得到的數據取十次平均
	int weight = scale.get_units(10);

  //由於可能發生校正沒校好，導致weight出現負數，可能導致後面出現Bug，所以要是出現負數，就將weight設為0
	if(weight < 0){
		weight = 0;
	}

  //將weight顯示在LCD上
	lcd.clear();
  lcd.setCursor(0, 0);
	lcd.print("Weight:");
	lcd.setCursor(7,0);
	lcd.print(weight);

  //經測試，一個輪框的weight約440，故將weight / 440，可以得到數量num
	int num = weight/440;
  //將num顯示在LCD
  lcd.setCursor(0, 1);
  lcd.print("num:");
  lcd.setCursor(4, 1);
  lcd.print(num);

  //讀取Keypad Shield的資訊

  int x;
  x = analogRead (0); //讀取A0的值
  lcd.setCursor(0,1); //把游標移到第2行第1個字
  if (x < 60){        //Right Buttom
    lcd.print ("send! ");
    Serial.println(weight);
    Serial.print(num);
  }else if (x < 200){ //Up Buttom
    //Do nothing
  }else if (x < 400){ //Down Buttom
    //Do nothing
  }else if (x < 600){ //Left Buttom
    //Do nothing
  }else if (x < 800){ //Select Buttom
    //Do nothing
  }
  delay(500); 
} 
