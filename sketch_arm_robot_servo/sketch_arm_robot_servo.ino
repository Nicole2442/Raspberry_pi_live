/*************************************************************************
* File Name          : nicole_makeblock.ino
* Author             : Nicole Liao
* Updated            : Nicole Liao
* Version            : V1.0.1 (servo test)
* Date               : 06/07/2016
* Description        : program for makeblock ultimate robot in logistics scenario
**************************************************************************/
#include <Wire.h>
#include <SoftwareSerial.h>
#include <Arduino.h>
#include <MeOrion.h>

MeDCMotor motorArm(PORT_1); //+speed:up; -speed:down
MeDCMotor motorHand(PORT_2); //+speed:open; -speed:close
MeDCMotor motorLeft(M1); //+speed:forward; -speed:backward
MeDCMotor motorRight(M2); //+speed:forward; -speed:backward

String mVersion = "1.0.1";

char serialRead;
int moveSpeed = 50;
int liftSpeed = 50;
int captureSpeed = 20;

/*********** servo for moving **************/
void Forward(int moveSpeed){
  motorLeft.run(moveSpeed);
  motorRight.run(moveSpeed);
}
void Backward(int moveSpeed){  
  motorLeft.run(-moveSpeed);
  motorRight.run(-moveSpeed);
}
void Left(int moveSpeed){
  motorLeft.run(-moveSpeed*2);
  motorRight.run(moveSpeed*2);
}
void Right(int moveSpeed){
  motorLeft.run(moveSpeed*2);
  motorRight.run(-moveSpeed*2);
}
void LiftUp(int liftSpeed){
  motorArm.run(liftSpeed*2);
}
void LiftDown(int liftSpeed){ 
  motorArm.run(-liftSpeed);
}
void HandOpen(int captureSpeed){
  motorHand.run(captureSpeed);
}
void HandClose(int captureSpeed){
  motorHand.run(-captureSpeed);
}
void StopMove(){
  motorLeft.stop();
  motorRight.stop();
}
void StopLift(){
  motorArm.stop();
  motorHand.stop();
}
/******** servo for moving *********/

void setup() {
  Serial.begin(115200);
  Serial.print("Version: nicole_makeblock ");
  Serial.println(mVersion);
}

void loop() {
  if(Serial.available()>0){
    serialRead = Serial.read();
    switch (serialRead){
      case 'L':{
        Serial.println("turn left");
        Left(moveSpeed);
      }
      break;
      case 'R':{
        Serial.println("turn right");
        Right(moveSpeed);
      }
      break;
      case 'F':{
        Serial.println("forward");
        Forward(moveSpeed);
      }
      break;
      case 'B':{
        Serial.println("backward");
        Backward(moveSpeed);
      }
      break;
      case 'U':{
        Serial.println("up");
        LiftUp(liftSpeed);
      }
      break;
      case 'D':{
        Serial.println("down");
        LiftDown(liftSpeed);
      }
      break;
      case 'O':{
        Serial.println("open");
        HandOpen(captureSpeed);
      }
      break;
      case 'C':{
        Serial.println("close");
        HandClose(captureSpeed);
      }
      break;
      default:{
        Serial.println("wrong command");
        StopMove();
        StopLift();
      }
      break;
    }
  }
}

