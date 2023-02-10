#include <RunningAverage.h>
#include <LiquidCrystal.h>
#include <EEPROM.h>


byte p1[8] = {
  0x10,
  0x10,
  0x10,
  0x10,
  0x10,
  0x10,
  0x10,
  0x10};
 
byte p2[8] = {
  0x18,
  0x18,
  0x18,
  0x18,
  0x18,
  0x18,
  0x18,
  0x18};
 
byte p3[8] = {
  0x1C,
  0x1C,
  0x1C,
  0x1C,
  0x1C,
  0x1C,
  0x1C,
  0x1C};
 
byte p4[8] = {
  0x1E,
  0x1E,
  0x1E,
  0x1E,
  0x1E,
  0x1E,
  0x1E,
  0x1E};
 
byte p5[8] = {
  0x1F,
  0x1F,
  0x1F,
  0x1F,
  0x1F,
  0x1F,
  0x1F,
  0x1F};

double percent=100.0;
unsigned char b;
unsigned int peace;
unsigned long int millisLast = millis();


    RunningAverage kmhAvg(5);

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

long rotations = 0;


void setup() {


  lcd.createChar(0, p1);
  lcd.createChar(1, p2);
  lcd.createChar(2, p3);
  lcd.createChar(3, p4);
  lcd.createChar(4, p5);
 
  lcd.begin(16, 2);

  
}
    
void loop() { 
     float interval = millis() - millisLast;
     float kmhPercent = kmhAvg.getAverage() * 1.3354*1.5;
      
       
      if(digitalRead(A0) == HIGH){
        rotations++;
        
        interval = millis() - millisLast;
        delay(100);
        millisLast = millis();
   
        float kmh = 7488/interval;
        kmhAvg.addValue(kmh);
        
        lcd.setCursor(12, 1);
        lcd.print(kmhAvg.getAverage());
        
        printCurrentInfo(rotations);
        drawBars(kmhPercent);
        }

      if(interval > 5000){
        stoppedProcedure(rotations);
        kmhAvg.fillValue(0.01, 20);
        lcd.clear();
        }

    interval = millis() - millisLast;
    Serial.println(interval);
    Serial.println(kmhAvg.getAverage());

    if(millis()/500 % 2 ==0){
      //empty
    }
}


void printCurrentInfo(long rotations){
      float tripKm = rotations * 2.0828/1000;
      float tripMi = rotations * 0.00129419;
  
      lcd.setCursor(0,0);
    if(millis()/10000 % 2 ==0){
      lcd.print(tripKm); 
      lcd.print(" km "); 
 
      }
    else{
      lcd.print(tripMi); 
      lcd.print(" mi "); 
      }
      lcd.setCursor(11, 0 );
      lcd.print(rotations);
}


void stoppedProcedure(float tripMi){
    int addr = 0;
    float totalMiles = 0;

  //find next available eeprom space
          for(int i = 0; i < EEPROM.length(); i++){
          if(EEPROM.read(i) == 0){
            addr = i;
          }
    }
        
    while(true){
      for(int i = 0; i < EEPROM.length(); i++){
        totalMiles += EEPROM.read(i);
        totalMiles = totalMiles / 5;
        }
      
      lcd.setCursor(0,1);

      lcd.print(totalMiles + tripMi);
      lcd.print(" mi total");
      lcd.setCursor(0,0);
      lcd.print((totalMiles * 1.60934 + tripMi * 1.60934));
      lcd.print(" km total");
        
      EEPROM.write(addr, (int)floor(tripMi * 5));

      if(digitalRead(A0) == HIGH){
          break;
          }
        
      delay(100);
     }
}

void drawBars(float kmhPercent){
  
  if(kmhPercent > 99.0){kmhPercent = 99.0;}

  
  double a = (11.0 / 100 ) * kmhPercent;


  lcd.setCursor(0,1);
  
  // drawing black rectangles on LCD
  if (a>=1) {
    for (int i=1;i<a;i++) {
      lcd.print((char)4);
      b=i;
      }
      a=a-b;
    }
 
  peace=a*5;
 
  // drawing charater's colums
  switch (peace) {
 
  case 0:
 
    break;
 
  case 1:
      
lcd.print((char)0);
 
    break;
 
  case 2:
  
lcd.print((char)1);
    break;
 
  case 3:
  
lcd.print((char)2);
    break;
 
  case 4:
  
lcd.print((char)3);
    break;
 }
 
  //clearing line
  for (int i =0;i<(11.0-b);i++) {
    lcd.print(" ");
    }
  
}
