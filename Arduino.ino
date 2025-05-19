#include <ServoSmooth.h>

#define Motr1_1 3
#define Motr1_0 11
#define Motr2_0 10
#define Motr2_1 9

#define PIN_TRIG 12
#define PIN_ECHO 13
#define SERVO_PIN 6

const long angl_range = 80;
const int angl_step = 5;

long buf_l, dist, angl_now=90-angl_range/2+angl_step, snk=1;
unsigned long timer;
ServoSmooth servos[1];

String inData = "";
String room = "0", skiptme;

long sonar() {
  if (angl_now==90+(angl_range/2) || angl_now==90-(angl_range/2)) {
    snk = snk*(-1);
  }
  angl_now = angl_now + angl_step * snk;
  
  servos[0].write(angl_now);
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(5);
  digitalWrite(PIN_TRIG, HIGH);

  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  buf_l = pulseIn(PIN_ECHO, HIGH);
  dist = (buf_l / 2) / 29.1;

  return (dist);
  delay(1000);
}

void ride(){
  analogWrite(Motr1_1,72);
  digitalWrite(Motr1_0,0);
  digitalWrite(Motr2_0,0);
  analogWrite(Motr2_1,72.5);
}

void vrash(){
  analogWrite(Motr1_0,1);
  digitalWrite(Motr1_1,173);
  analogWrite(Motr2_0,0);
  digitalWrite(Motr2_1,72.5);
}

void vrash2(){
  analogWrite(Motr1_0,0);
  analogWrite(Motr1_1,72);
  analogWrite(Motr2_0,1);
  digitalWrite(Motr2_1,172.5);
}

void maniouvr(){
  vrash();
  delay(600);
  ride();
  delay(5000);
  vrash2();
  delay(600);
  ride();
  delay(5000);
}

void blucon()
{
  if (Serial.available() > 0) {   
    inData = Serial.readString();  
    if ( inData[0] == 'S') {
      Serial.println(inData);
      room =  inData.substring(1, inData.length()-2);
      skiptme = inData.substring(inData.length()-1, inData.length());
    }
  }
  pinMode(Motr1_1, OUTPUT);
  pinMode(Motr1_0, OUTPUT);
  pinMode(Motr2_1, OUTPUT);
  pinMode(Motr2_0, OUTPUT);
}

void setup()
{
  Serial.begin(9600);
  room = "0";
  while (room == "0") {
	  blucon();
	  if (room!="0") {
      Serial.println(room);
      Serial.println(skiptme);
    }
    delay(500);
  }
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  servos[0].attach(SERVO_PIN);
  servos[0].setAccel(0.01);
  servos[0].setSpeed(70);
  servos[0].setTargetDeg(90-angl_range/2+angl_step);
}

void loop()
{
  ride();
  dist = sonar();
  if (dist < 50) {
    maniouvr();
    angl_now = angl_now + angl_step*5*snk;
    if (angl_now<90-(angl_range/2)) {angl_now=90-(angl_range/2);}
    if (angl_now>90+(angl_range/2)) {angl_now=90+(angl_range/2);}
    servos[0].setTargetDeg(angl_now);
  } 
  delay(100);
}

