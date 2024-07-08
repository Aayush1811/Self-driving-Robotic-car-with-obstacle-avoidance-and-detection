#include "Servo.h"

#include <MPU6050_tockn.h>
#include <Wire.h>

#define ren 11
#define rin1 12
#define rin2 10
#define len 6
#define lin1 7
#define lin2 8

MPU6050 mpu6050(Wire);
Servo myservo;
int pos = 90;
int current_pos = 90;
int pre_pos = 90;
int x = 0;

int start = 0;
int pre = 0;
int angle[180];
int index = 0;
int end = 0;

// red - trig
// green - echo
// y - gnd
// o - vcc


float p_dis;
float dis;
float dis_arr;
int i = 0;

int flag = 0;

//int end = 0;

int max_value_pos;
int maxVal;

int degree;
int ang;
int change = 0;


#define trig 4
#define echo 3

void setup() {
  
  Serial.begin(9600);
  myservo.attach(9);
  myservo.write(90);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(ren, OUTPUT);
  pinMode(len, OUTPUT);
  pinMode(rin1, OUTPUT);
  pinMode(rin2, OUTPUT);
  pinMode(lin1, OUTPUT);
  pinMode(lin2, OUTPUT);
  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);

  digitalWrite(trig,LOW);
  

}

void loop() 
{

  mpu6050.update();

  digitalWrite(trig,HIGH);
  delayMicroseconds(10);
  digitalWrite(trig,LOW);

    p_dis = pulseIn(echo,HIGH);
    dis = 0.017 * p_dis;

  analogWrite(len,120);
  analogWrite(ren,120);
  digitalWrite(lin1, LOW);
  digitalWrite(lin2, HIGH);
  digitalWrite(rin1, HIGH);
  digitalWrite(rin2, LOW);

  
  
  //Serial.println(dis);

  


  if(dis<20)
  {
    flag = 1;
    analogWrite(len, 0);
    analogWrite(ren, 0);
  }

  if(flag == 1)
  {

  while(true)
  {
    start = millis();
    digitalWrite(trig,HIGH);
    delayMicroseconds(10);
    digitalWrite(trig,LOW);

    p_dis = pulseIn(echo,HIGH);
    dis = 0.017 * p_dis;
     

  

   if(current_pos > 1)
   {

    
    

    if(start - pre >= 50)
    {
      //Serial.println(dis);
      //Serial.println(current_pos);
      pos --;
      myservo.write(pos);
      current_pos = myservo.read();
      pre = start;
  
    }

    if(abs(current_pos - pre_pos) >=5)
    {
      //Serial.println(current_pos);
      dis_arr = dis;
      angle[index] = dis_arr;
      //Serial.println(angle[index]);

      if(angle[index]>500)
      {
        angle[index] = 500;
      }
      index++;
     
      pre_pos = current_pos;

    }

   }
   else  
  {
    

    maxVal = angle[0];
    for (i ; i < (sizeof(angle) / sizeof(angle[0])); i++) 
    {
      if (angle[i] > maxVal) 
      {
        maxVal = angle[i];
        max_value_pos = i+1;
      }
    
    }
    
    degree = max_value_pos * 5;

    pos = 90;
    current_pos = 90;
    pre_pos = 90;
    i = 0;

    flag = 2;
    myservo.write(90);
    if(flag == 2)
    {
      break;
    }
    
  }
  
  }

  }

  if(flag == 2)
  {
    while(ang <= degree)
    {
      mpu6050.update();
      ang = mpu6050.getAngleZ()-change;

      Serial.print("cu_ang: ");
      Serial.print(mpu6050.getAngleZ());
      Serial.print("set");
      Serial.println(degree);

      analogWrite(len,150);
      analogWrite(ren, 150);
      digitalWrite(lin1, LOW);
      digitalWrite(lin2, HIGH);
      digitalWrite(rin1, LOW);
      digitalWrite(rin2, HIGH);
      
    }
    Serial.print("complete");
    change = change+degree;
    ang = mpu6050.getAngleZ()-change;

    flag = 0;

  }

  
  
  // Serial.print("\tangleZ : ");

  Serial.println(dis);
 

}
