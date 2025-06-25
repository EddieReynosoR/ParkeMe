#include <Servo.h> //includes the servo library
#include <Wire.h> 
// #include <LiquidCrystal_I2C.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h> //I2C expander i/o class header

// LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
// LiquidCrystal_I2C lcd(0x27,16,2);
hd44780_I2Cexp lcd(0x27);

Servo myservo;

// #define ir_enter 2
// #define ir_back  4

#define ir_car1 2
#define ir_car2 3
#define ir_car3 4
#define ir_car4 5
#define ir_car5 6
#define ir_car6 7

int S1=0, S2=0, S3=0, S4=0, S5=0, S6=0;
int slot = 6;  

int parkingSlots[6];

void setup(){
  Serial.begin(9600);

  pinMode(ir_car1, INPUT);
  pinMode(ir_car2, INPUT);
  pinMode(ir_car3, INPUT);
  pinMode(ir_car4, INPUT);
  pinMode(ir_car5, INPUT);
  pinMode(ir_car6, INPUT);

  lcd.begin(16, 2);  
  lcd.setCursor (1,0);
  lcd.print("Car parking");
  lcd.setCursor (1,1);
  lcd.print(" System ");
  delay (2000);
  lcd.clear();   

  Read_Sensor();

  int total = S1+S2+S3+S4+S5+S6;
  slot = slot-total; 
}

void loop(){

  Read_Sensor();

  lcd.setCursor (0,0);
  if(S1==1){lcd.print("1:F ");}
      else{lcd.print("1:E");}

  lcd.setCursor (6,0);
  if(S2==1){lcd.print("2:F ");}
      else{lcd.print("2:E ");}

  lcd.setCursor (11,0);
  if(S3==1){lcd.print("3:F ");}
      else{lcd.print("3:E ");}

  
  lcd.setCursor (0,1);
  if(S4==1){lcd.print("4:F ");}
      else{lcd.print("4:E ");}

  lcd.setCursor (6,1);
  if(S5==1){lcd.print("5:F ");}
      else{lcd.print("5:E ");}

  lcd.setCursor (11,1);
  if(S6==1){lcd.print("6:F ");}
      else{lcd.print("6:E ");}

  parkingSlots[0] = S1;
  parkingSlots[1] = S2;
  parkingSlots[2] = S3;
  parkingSlots[3] = S4;
  parkingSlots[4] = S5;
  parkingSlots[5] = S6;

  for (int i = 0; i < 6; i++) {
    Serial.print(String(parkingSlots[i]));
    if (i < 2) {
      Serial.print(",");
    }
  }

  Serial.println();

  delay(200);
  
}

void Read_Sensor(){
  S1=0, S2=0, S3=0, S4=0, S5=0, S6=0;

  if(digitalRead(ir_car1) == 0){S1=1;}
  if(digitalRead(ir_car2) == 0){S2=1;}
  if(digitalRead(ir_car3) == 0){S3=1;} 
  if(digitalRead(ir_car4) == 0){S4=1;}
  if(digitalRead(ir_car5) == 0){S5=1;}
  if(digitalRead(ir_car6) == 0){S6=1;}
}



