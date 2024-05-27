#define BLYNK_TEMPLATE_ID "TMPL6dnpdn3Hm"
#define BLYNK_TEMPLATE_NAME "Smart car parking"
#define BLYNK_AUTH_TOKEN "fLUlE89oGdbPkjV3rhmF2aPU6trrt8eV"
#define BLYNK_PRINT Serial
#include <Servo.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SPI.h>
#include <MFRC522.h>
#include <EEPROM.h>
#include <SoftwareSerial.h>
#define BUZZER  16 //D0
 //  SCK -- D5 MOSI -- D7  MISO -- D6
#define SS_PIN 4   //D2
#define RST_PIN 5   //D1
#define servo_v 0  //D3
#define servo_r 2  //D4
Servo myservo_v;
Servo myservo_r;
MFRC522 mfrc522(SS_PIN, RST_PIN);
unsigned long uidDec, uidDecTemp;
int ARRAYindexUIDcard;
int EEPROMstartAddr;
bool beginCard = 0;
int flag2=0;
bool addCard = 1;
bool skipCard = 0;
int LockSwitch;
unsigned long CardUIDeEPROMread[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
char auth[] = "fLUlE89oGdbPkjV3rhmF2aPU6trrt8eV";
char ssid[] = "HuyennThuongg";
char pass[] = "Huyenthuong30522";
WidgetLCD lcd(V0);
BlynkTimer timer;
String myString; 
char rdata; // received charactors
int firstVal, secondVal,thirdVal; // sensors
int led1,led2,led3,sv,sr;
void success_buzzer()
{ digitalWrite(BUZZER, HIGH);
  delay(2000);
  digitalWrite(BUZZER, LOW); }
void Failure_buzzer()
{ for (int i = 0; i < 3; i++)
  { digitalWrite(BUZZER, HIGH);
    delay(100);
    digitalWrite(BUZZER, LOW);
    delay(50); } }
void beep()
{ digitalWrite(BUZZER, HIGH);
  delay(100);
  digitalWrite(BUZZER, LOW); }
void myTimerEvent()
{ Blynk.virtualWrite(V1, millis() / 1000); }
void setup() {
   Serial.begin(9600);
  pinMode(BUZZER, OUTPUT);
myservo_r.attach(servo_v);
myservo_v.attach(servo_r);
  SPI.begin();
  mfrc522.PCD_Init();
  Blynk.begin(auth, ssid,pass);
  lcd.clear(); //Use it to clear the LCD Widget
  EEPROM.begin(512);
  DisplayWAiT_CARD();
  EEPROMreadUIDcard();
timer.setInterval(1000L,sensorvalue1);
timer.setInterval(1000L,sensorvalue2);
timer.setInterval(1000L,sensorvalue3); }
void sensorvalue1()
{ int sdata = led1;
Blynk.virtualWrite(V10, sdata); }
void sensorvalue2()
{ int sdata = led2;
Blynk.virtualWrite(V11, sdata); }
void sensorvalue3()
{ int sdata = led3;
Blynk.virtualWrite(V12, sdata); }
String getValue(String data, char separator, int index)
{ int found = 0;
int strIndex[] = { 0, -1 };
int maxIndex = data.length() - 1;
for (int i = 0; i <= maxIndex && found <= index; i++) {
if (data.charAt(i) == separator || i == maxIndex) {
found++;
strIndex[0] = strIndex[1] + 1;
strIndex[1] = (i == maxIndex) ? i+1 : i; }}
return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
void loop() {
 rdata = Serial.read();
myString = myString+ rdata;

if( rdata == '\n')
{
String l = getValue(myString, ',', 0);
String m = getValue(myString, ',', 1);
String n = getValue(myString, ',', 2);
String v =getValue(myString, ',', 3);
String r =getValue(myString, ',', 4);
led1 = l.toInt(); led2 = m.toInt(); led3 = n.toInt();
sv=v.toInt(); sr=r.toInt();
myString = "";}
if (sv==1 || sr ==1){
  if (beginCard == 0) {
    if ( ! mfrc522.PICC_IsNewCardPresent()) {  //Look for new cards.
      Blynk.run();  return;}
    if ( ! mfrc522.PICC_ReadCardSerial()) {  //Select one of the cards.
      Blynk.run(); return; }}
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    uidDecTemp = mfrc522.uid.uidByte[i];
    uidDec = uidDec * 256 + uidDecTemp; }
  if (beginCard == 1 || LockSwitch > 0)EEPROMwriteUIDcard();  
int j; int k =0;
  if (LockSwitch == 0) {
    for (int j = 0; j <= 9; j++) {
      if (CardUIDeEPROMread[j] > 0) {
        if (CardUIDeEPROMread[j] == uidDec) {
          lcd.clear();
          lcd.print(0, 0, "CARD ACCESS OPEN");
          beep();
          if (sv==1){
             myservo_v.write (0); delay(2000);
           myservo_v.write (90);
           }else if (sr==1){
       myservo_r.write (0); delay(2000); myservo_r.write (90);
    }
          lcd.print(3, 1, uidDec);
          k=1;
          delay(2000); break; }}}   
     if (CardUIDeEPROMread[j] != uidDec && k==0 && uidDec >0) { // 
      lcd.clear();
       Failure_buzzer();
      lcd.print(0, 0, " Card not Found ");
      lcd.print(0, 1, "                ");
      lcd.print(0, 1, "ID : ");
      lcd.print(5, 1, uidDec);
      for (int i = 0; i <= 2; i++)delay(100), digitalWrite(PiezoPin, HIGH), delay(100), digitalWrite(PiezoPin, LOW);
      delay(2000); }
    DisplayWAiT_CARD();}}
Blynk.run();
  timer.run(); }
BLYNK_WRITE(V1) {
  int a = param.asInt();
  if (a == 1){
LockSwitch=1;
   beginCard = 1;} else beginCard = 0; }
BLYNK_WRITE(V2) {
  int a = param.asInt();
  if (a == 1) {
    skipCard = 1;
    LockSwitch=1;
    if (EEPROMstartAddr / 5 < 10) EEPROMwriteUIDcard();
  } else {
    skipCard = 0;
  }}
BLYNK_WRITE(V3) {
  int a = param.asInt();
  if (a == 1) {
    lcd.clear();
    lcd.print(0, 0, " APP UNLOCK OK ");
    lcd.print(0, 1, "   DOOR OPEN   ");
      beep();
    if (sv==1){
     myservo_v.write (0);
           delay(3000);
           myservo_v.write (90);
    }else if (sr==1){
       myservo_r.write (90);
           delay(3000);
           myservo_r.write (180);
    } delay(2000);
    DisplayWAiT_CARD();}}
void EEPROMwriteUIDcard() {
  if (LockSwitch == 0) {
    lcd.clear();
    lcd.print(0, 0, " START REC CARD ");
    lcd.print(0, 1, "PLEASE SCAN CARDS");
    delay(500); }
  if (LockSwitch > 0) {
    if (skipCard == 1) {  //uidDec == adminID
      lcd.clear();
      lcd.print(0, 0, "   Remove RECORD   ");
     success_buzzer();
      for (int i = 0; i <= 9; i++) {
    if (CardUIDeEPROMread[i]== uidDec){
       CardUIDeEPROMread[i]=0;
        LockSwitch=0;
        skipCard = 0; }}       
    } else {
      EEPROM.write(EEPROMstartAddr, uidDec & 0xFF);
      EEPROM.write(EEPROMstartAddr + 1, (uidDec & 0xFF00) >> 8);
      EEPROM.write(EEPROMstartAddr + 2, (uidDec & 0xFF0000) >> 16);
      EEPROM.write(EEPROMstartAddr + 3, (uidDec & 0xFF000000) >> 24);
      EEPROM.commit();
      if (ARRAYindexUIDcard <10){
      EEPROMreadUIDcard();
       ARRAYindexUIDcard ++;
       EEPROMstartAddr += 5;
       delay(10);
       success_buzzer();
      lcd.clear();
      lcd.print(0, 1, "                ");
      lcd.print(0, 0, "RECORD OK! IN   ");
      uidDec = 0;
      delay(500);
      LockSwitch=0; }}}
  if (EEPROMstartAddr / 5 == 10) {
    lcd.clear();
    lcd.print(0, 0, "RECORD FINISH");
    delay(2000);
    EEPROMstartAddr = 0;
    uidDec = 0;
    ARRAYindexUIDcard = 0; }}
void EEPROMreadUIDcard() {
  for (int i = 0; i <= 9; i++) {
    if (CardUIDeEPROMread[i]!= uidDec){
    byte val = EEPROM.read(EEPROMstartAddr + 3);
    CardUIDeEPROMread[ARRAYindexUIDcard] = (CardUIDeEPROMread[ARRAYindexUIDcard] << 8) | val;
    val = EEPROM.read(EEPROMstartAddr + 2);
    CardUIDeEPROMread[ARRAYindexUIDcard] = (CardUIDeEPROMread[ARRAYindexUIDcard] << 8) | val;
    val = EEPROM.read(EEPROMstartAddr + 1);
    CardUIDeEPROMread[ARRAYindexUIDcard] = (CardUIDeEPROMread[ARRAYindexUIDcard] << 8) | val;
    val = EEPROM.read(EEPROMstartAddr);
    CardUIDeEPROMread[ARRAYindexUIDcard] = (CardUIDeEPROMread[ARRAYindexUIDcard] << 8) | val;
break; }}}
void DisplayWAiT_CARD() {
  lcd.clear();
  lcd.print(0, 0, "    SCAN THE   ");
  lcd.print(0, 1, "      CARD      ");
}
