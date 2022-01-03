#include <Password.h>
 

#include <LiquidCrystal_I2C.h>
 

#include <Keypad.h>

 
#include <pitches.h>
 

Password password = Password( "1234" );
 
const byte ROWS = 4;
const byte COLS = 4; 
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};

 
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
 
LiquidCrystal_I2C lcd(0X21,10, 9);
 
const int armedLed = A0;      
int armed = 0;                
 
const int laserSense =12 ;            
int sensorHit = 0;                   
const int buzzer = 13;                
int notes[]={                         
    NOTE_A4, NOTE_B4, NOTE_C3 };
 
void setup() {
  Serial.begin (9600);
 
  
  lcd.begin(16, 2);
  keypad.addEventListener(keypadEvent); 
  keypad.setDebounceTime(100);  
 
  pinMode (armedLed, OUTPUT);        
  pinMode (laserSense, INPUT);       
  pinMode (buzzer, OUTPUT);          
  analogWrite(armedLed, 0);         
 
  
  lcd.print("Code to arm:");
 
}
 
void loop() {
 
  lcd.setCursor(0, 1);
 
  sensorHit=analogRead(laserSense);
  Serial.println (sensorHit);            
  if (sensorHit < 700 && armed == 1){    
    tone(buzzer,notes[3],2000);           
  }
  keypad.getKey();                       
}
 


void keypadEvent(KeypadEvent eKey){
  switch (keypad.getState()){
      case PRESSED:
      lcd.print(eKey);  
    switch (eKey){
      case '#': guessPassword(); break; 
      default:
      password.append(eKey);
    }
  }  
}
 
void guessPassword(){
  if (password.evaluate()){      
    lcd.clear();                 
    lcd.print("VALID PASSWORD");  
    password.reset();            
    delay(1500);                  
    lcd.clear();                 
    if (armed==0){               
      lcd.print("ARMED!");        
      analogWrite(armedLed, 255); 
      armed=1;                      
      delay(2000);                  
      lcd.clear();               
      lcd.setCursor(0, 0);         
      lcd.print("Code to disarm:"); 
    }
    else{
      lcd.print("DISARMED!");         
      analogWrite(armedLed, 0);      
      armed=0;                       
      delay(2000);                 
      lcd.clear();             
      lcd.setCursor(0, 0);          
      lcd.print("Code to arm:");     
    }
  }
  else{                            
    lcd.clear();
    lcd.print("INVALID PASSWORD");
    password.reset();             
    tone(buzzer,notes[1],200);    
    delay(2000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Retry Code:");
  }
}
