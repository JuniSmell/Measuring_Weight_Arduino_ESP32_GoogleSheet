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

//紀錄上一筆weight，由於weight訊號可能會有小幅度的上下跳動，所以紀錄上一筆的weight
//如果 weight 與 last_weight 差異太小，就當作是誤差，不理它
int last_weight = 0;
//如果weight長期沒有改變，就當作訊號穩定了
//這裡的「長期」，是指重複兩次
int timer = 2;
//顯示出來的weight，因為weight會有各種誤差，需要經過穩定確認才能確定這是我們要的weight
int present_weight = 0;
//上一筆present_weight，用來計算真的重量差，
int last_present_weight = 0;
//總數
int num = 0;
//兩種輪框的個別數量
int nums[2] = {0, 0};

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
  //取得重量訊號
	int weight = scale.get_units(10);
  //如果因為校正沒有做好，導致訊號變成負的，可能會導致bug，所以做這個防呆
	if(weight < 0){
		weight = 0;
	}
  //穩定檢查，先去掉變化太少的雜訊，然後當數據重複兩次了，它才會更新呈現出來的訊號
  if(weight != last_weight){  //如果weight發生了改變
    if(weight - last_weight > 5 || weight - last_weight < 5){ //但改變的幅度太小
      timer = 2;
    }else{  //改變的夠大
      timer--;
      if(timer == 0){
        timer = 2;
        present_weight = weight;
      }
    }
  }else{
    timer--;
    if(timer == 0){
      timer = 2;
      present_weight = weight;
    }
  }  
  
  if(present_weight - last_present_weight > 460 && present_weight - last_present_weight < 485){
    nums[0]++;
    num++;
  }else if(present_weight - last_present_weight > 485 && present_weight - last_present_weight < 520){
    nums[1]++;
    num++;
  }else if(present_weight - last_present_weight < -485 && present_weight - last_present_weight > -520){
    nums[1]--;
    num--;
  }else if(present_weight - last_present_weight < -460 && present_weight - last_present_weight > -485){
    nums[0]--;
    num--;
  }
  //輸出Num A 到LCD  
  lcd.clear();
  lcd.setCursor(0, 0);
	lcd.print("Num A:");
	lcd.setCursor(7,0);
	lcd.print(nums[0]);
  //輸出Num B 到LCD
  lcd.setCursor(0, 1);
  lcd.print("Num B:");
  lcd.setCursor(7, 1);
  lcd.print(nums[1]);
	
  int x;
  x = analogRead (0);  //讀取A0的值
  lcd.setCursor(0,1); //把游標移到第2行第11個字
  if (x < 60){
    lcd.print ("send! ");
    Serial.println(present_weight);
    Serial.println(num);
    Serial.println(nums[0]);
    Serial.print(nums[1]);
  }
  last_weight = weight;
  if(last_present_weight != present_weight){
    last_present_weight = present_weight;
  }
  delay(500); 
} 
