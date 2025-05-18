#include<LiquidCrystal.h>
#define bButt 2
#define rButt 3
#define oButt 4
#define wButt 5
#define bRgb 6
#define rRgb 7
#define oRgb 8
#define wRgb 9
#define buzz 10
unsigned long lastSoundTime = 0;//millis()
unsigned long lastLedTime = 0;//millis()
int leds[] = {bRgb, rRgb, oRgb, wRgb};
int buttons[] = {bButt, rButt, oButt, wButt};
int tones[] = {262, 330, 392, 523}; //لكل ليد ازعاج خاص يسه
int level = 1;
int sequence[8];
int userSequence[8];
LiquidCrystal lcd(12,12,A1,A2,A3,A4);
void setup() {
  randomSeed(analogRead(0));// نضمن بيها ان الارقام العشوائية نفسها مش بتتكرر (srand(time(0)))
  lcd.begin(16,2);
  Serial.begin(9600);
  pinMode(buzz, OUTPUT);
  for (int i = 0; i < 4; i++) {//ضربنا عصفورين بحجر
    pinMode(leds[i], OUTPUT);
    pinMode(buttons[i], INPUT);
  }
  
  StartTone();//اول حاجة بتشتغل
  delay(1000);
}

void loop() {
  generate();//دالة توليد رقم عشوائي وتخزنو في مصوصفة
  play();//نولع الانوار بالمصفوفة العملناها بدالة توليد الارقام العشوائية
    if (UserInput()) {//يعني لو الازرار اللي ضغطها المستخدم شبه اللي ولدتها الدالة العشوائية الشرط ده هيبقى صح 
    level++;
    winsound();
    delay(1000);
    } else {//غير كدة المستخدم ضغط زر مبيشبهش اللي عملتو دالة التوليد العشوائي كدة خسرت واللفل هيرجع واحد
    gameOver();
    delay(1000);
  }
  //شرط الفوز اللفل يوصل 8 وتفوز 
   if(level==9){
    lcd.clear();
    lcd.setCursor(4,1);
    lcd.print("good job");
    lcd.clear();
    delay(500);
    lcd.print("you have strong ");
    lcd.setCursor(6,1);
    lcd.print("memory");
    delay(2000);
    lcd.clear();
    lcd.print("lets do it again");
    delay(1000);
    lcd.clear();
    winsound();
    delay(1000);
    level=1;
  }
}
// دي للروقان لما اللعبة تبدا بس
void StartTone() {
  for(int i=0 ; i<2 ;i++){  
    lcd.setCursor(1 , 0);
  	lcd.print("lets test your ");
    lcd.setCursor(6 , 1);
    lcd.print("memory");
	digitalWrite(bRgb , HIGH);
  	tone(buzz , 261);
  	delay(170);
  	digitalWrite(bRgb , LOW);
  
  	digitalWrite(rRgb , HIGH);
   	tone(buzz , 330);
  	delay(150);
  	digitalWrite(rRgb , LOW);
  
  	digitalWrite(oRgb , HIGH);
   	tone(buzz , 392);
  	delay(150);
  	digitalWrite(oRgb , LOW);
  
  	digitalWrite(wRgb , HIGH);
    tone(buzz , 440);
  	delay(150);
  	digitalWrite(wRgb , LOW);
  	noTone(buzz);
  	lcd.clear();
    
  }
}
//كل مانعدي لفل هنشتغل
void winsound() {
 
  tone(buzz, 523);
  delay(100);
  noTone(buzz);
  delay(50);

  tone(buzz, 659);
  delay(100);
  noTone(buzz);
  delay(50);

  tone(buzz, 784);
  delay(100);
  noTone(buzz);
  delay(50);

  tone(buzz, 1046); 
  delay(150);
  noTone(buzz);
  delay(70);
  for (int i = 0; i < 4; i++) {
    digitalWrite(leds[i], HIGH);
    delay(100);
    digitalWrite(leds[i], LOW);
  }

  for (int i = 0; i < 4; i++) {
    digitalWrite(leds[i], HIGH);
  }
  delay(200);
  for (int i = 0; i < 4; i++) {
    digitalWrite(leds[i], LOW);
  }
  if (level < 9)
  {
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("You Win!");
  lcd.setCursor(4, 1);
  lcd.print("Level: ");
  lcd.print(level);
  delay(1000);
  lcd.clear();
  }
}
//  بتولد ارقام بين ال 0 و3 وبتحفظهم في مصفوفة عدد الارقام اللي بتولدهم بتعتمد على ال لفل
void generate() {
  for (int i = 0; i < level; i++) {
    sequence[i] = random(4);//زي (rand()%4)في c++ 
  }
}
// بتاخد الرقام من المصوفة اللي خزنا فيها ارقام عشوائية وبتولع بيهم الانواؤ علشان نشوفها
void play() {
  for (int i = 0; i < level; i++) {
    int index = sequence[i];  
    digitalWrite(leds[index], HIGH);
    playTone(index); 
    delay(250);
    digitalWrite(leds[index], LOW);
    noTone(buzz);
    delay(100);
  }
}
/* اول مانضغط زر هنولع النور الي تابع للزر ونخزن رقم الزر اللي اتداس في مصفوفة جديدة
 ونقارن لو رقم الزر بيشبه رقم النور اللي الي نور في المصفوفة العشوائية لو تمام هيزيد اللفل */
bool UserInput() {
  for (int i = 0; i < level; i++) {
    bool buttonPressed = false;
    while (!buttonPressed) {
      for (int j = 0; j < 4; j++) {
        if (digitalRead(buttons[j]) == HIGH) {
          digitalWrite(leds[j], HIGH);
          playTone(j);
          delay(250);  
          digitalWrite(leds[j], LOW);
          userSequence[i] = j;
          delay(100);
          buttonPressed = true;
          break;
        }
      }
    }
    if (userSequence[i] != sequence[i]) {
      return false;
    }
  }
  return true;
}
// دي للازعاج ملناش دعوة بيها لكل زر صوت خاص علشان العذاب
void playTone(int buttonIndex) {
  tone(buzz, tones[buttonIndex]);
  delay(250);
  noTone(buzz);
}
//لو خسرنا ربنا يعوضنا على الازعاج. اللفل هترجع ل1 من جديد
void gameOver() {
  level = 1;
  for(int i=1 ;i<3 ;i++){
  tone(buzz, 130);
   digitalWrite(bRgb , HIGH);
   digitalWrite(rRgb , HIGH); 
   digitalWrite(oRgb , HIGH); 
   digitalWrite(wRgb , HIGH);
   lcd.clear();
   lcd.setCursor(3, 0);
   lcd.print("bad memory");
   lcd.setCursor(0, 1);
   lcd.print("level back to: ");
   lcd.setCursor(15, 1);
   lcd.print(level);
   delay(500);
   noTone(buzz);
   digitalWrite(bRgb , LOW);
   digitalWrite(rRgb , LOW); 
   digitalWrite(oRgb , LOW); 
   digitalWrite(wRgb , LOW);
   lcd.clear();
   delay(500);
  }
}