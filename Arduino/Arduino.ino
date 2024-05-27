#include <SoftwareSerial.h>
#include<Wire.h>
#include<LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0X27,16,2);
SoftwareSerial nodemcu(2,3);
int parking1_slot1_ir_s = 40; 
int parking1_slot2_ir_s = 38;
int parking1_slot3_ir_s = 36;
int sensor_enter = 34;
int sensor_back = 32;
String sensor1;
int count1;
String sensor2;
int count2;
String sensor3;
int count3;
String sensor4;
String sensor5;
String cdata =""; // complete data, consisting of sensors values
void setup(){
lcd.init();
lcd.backlight();
Serial.begin(9600);
nodemcu.begin(9600);
pinMode(parking1_slot1_ir_s, INPUT);
pinMode(parking1_slot2_ir_s, INPUT);
pinMode(parking1_slot3_ir_s, INPUT);
pinMode(sensor_enter, INPUT);
pinMode(sensor_back, INPUT); }
void loop(){
int count =0;
int slot=3; p1slot1(); p1slot2(); p1slot3();p1slot4();p1slot5();
count=sensor1.toInt()+sensor2.toInt()+sensor3.toInt();
slot=slot-count;
lcd.setCursor(0,0);
lcd.print("VACANT SLOT: ");
lcd.setCursor(2,1);
lcd.print(slot);
cdata = cdata + sensor1 +"," + sensor2 + ","+ sensor3+","+sensor4+","+sensor5 ; // comma will be used a delimeter
Serial.println(cdata);
nodemcu.println(cdata);
delay(1000); // 100 milli seconds
cdata = "";
digitalWrite(parking1_slot1_ir_s, HIGH);
digitalWrite(parking1_slot2_ir_s, HIGH);
digitalWrite(parking1_slot3_ir_s, HIGH);
digitalWrite(sensor_enter, HIGH);
digitalWrite(sensor_back, HIGH);}
void p1slot1() // parkng 1 slot1{
if( digitalRead(parking1_slot1_ir_s) == LOW){
sensor1 = "1";
delay(10);}
if( digitalRead(parking1_slot1_ir_s) == HIGH){
sensor1 = "0";
delay(10);}}
void p1slot2() // parking 1 slot2{
if( digitalRead(parking1_slot2_ir_s) == LOW){
sensor2 = "1";delay(10);}
if( digitalRead(parking1_slot2_ir_s) == HIGH){
sensor2 = "0";delay(200);}}
void p1slot3() // parking 1 slot3{
if( digitalRead(parking1_slot3_ir_s) == LOW){
sensor3 = "1";delay(10);}
if( digitalRead(parking1_slot3_ir_s) == HIGH){
sensor3 = "0";delay(10);}}
void p1slot4() {
if( digitalRead(sensor_enter) == LOW){
sensor4 = "1";delay(10);}
if( digitalRead(sensor_enter) == HIGH){
sensor4 = "0";delay(10);}}
void p1slot5() 
{if( digitalRead(sensor_back) == LOW)
{
sensor5 = "1";delay(10);}
if( digitalRead(sensor_back) == HIGH)
{sensor5 = "0";delay(10);}}
