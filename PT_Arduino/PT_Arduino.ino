#include <QTRSensors.h>
#include <Wire.h>
#include <ZumoShield.h>
#include <ZumoMotors.h>
#include <NewPing.h>

// these might need to be tuned for different motor types
// this might need to be tuned for different lighting conditions, surfaces, etc.
#define QTR_THRESHOLD    1999 // microseconds
#define REVERSE_SPEED     200 // 0 is stopped, 400 is full speed
#define TURN_SPEED        200
#define FORWARD_SPEED     200
#define REVERSE_DURATION  200 // ms
#define TURN_DURATION     300 // ms
#define NUM_SENSORS         6
#define TRIGGER_PIN         2  // Arduino pin  tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN            6 // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE       15   // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

#define LED_PIN            13

ZumoMotors motors;
ZumoBuzzer buzzer;
ZumoReflectanceSensorArray sensors(QTR_NO_EMITTER_PIN);
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

int incomingByte;      // a variable to read incoming serial data into
bool roomSearched = false;
int lastError = 0;
unsigned int sensor_values[NUM_SENSORS];
const int MAX_SPEED = 400;

//-----------------------------------------
//            SETUP
//-----------------------------------------

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);

  caliberate();
}

//-----------------------------------------
//            LOOP
//-----------------------------------------
void loop() {
  
  if (Serial.available() > 0) { 
    incomingByte = Serial.read();
    
    switch(incomingByte){         
        case 'w': {  // GO FORWARD
            goStraight();
            break;
        }

        case 's': { // GO BACKWARD
            goBack();
            break;
        }
  
        case 'd': { // GO RIGHT
            goRight();
            break;  
        }

        case 'a': { // GO LEFT
            goLeft();
            break;
        }

        case 'W':{
            goForwardABit();
            break;

        case 'S':{
            reverseABit();
            break;
        }

        case 'A': { // GO UPPER LEFT
           goUpperLeft();
            break;   
        } 

        case 'D': {  // GO UPPER RIGHT
          goUpperRight();
          break; 
        }
  
        case 'c':{ //AUTONOMOUS 
           autonomous();            
           break;    
        }
  
        case 'e' :{ //END AUTONOMOUS
          Serial.println("You have reached the end of the corridor...");
          break;
         }
          
        case 'r' :{ //Zumo stops outside the room on it's right
          foundRoomRight();
          break;
        }

        case 'f' : {
          foundRoomLeft();
          break;
        }
        
        case 'n':{
          finishRoomRight();
          break;
        }

        case 't':{
          finishRoomLeft();
          break;
        }

        case 'u':{
          uTurnRightWing();
          break;
        }

        case 'i':{
          uTurnLeftWing();
          break;
        }
      } 
    }    
  }
}

//-----------------------------------------
//            CALIBERATE
//-----------------------------------------

 void caliberate() {
   Serial.println("Zumo caliberating");
   Serial.println("-------------------------------");
   
   pinMode(13, OUTPUT);
   digitalWrite(13, HIGH);

    delay(1000);
    int i;
    for(i = 0; i < 80; i++)
    {
      if ((i > 10 && i <= 30) || (i > 50 && i <= 70))
        motors.setSpeeds(-200, 200);
      else
       motors.setSpeeds(200, -200);
       sensors.calibrate();
  
      // Since our counter runs to 80, the total delay will be
      // 80*20 = 1600 ms.
      delay(20);
    }
    motors.setSpeeds(0,0);
  
    // Turn off LED to indicate we are through with calibration
    digitalWrite(13, LOW);
    buzzer.play(">g32>>c32");
 }

//-----------------------------------------
//            GO STRAIGHT
//-----------------------------------------

 void goStraight(){
    Serial.println("Zumo is going straight");
    Serial.println("-------------------------------"); 

    for(int i = 0; i < 80; i++)
    {
      if ((i > 10 && i <= 30) || (i > 50 && i <= 70))
      motors.setSpeeds(200, 200);
      delay(10);
    }
    
    motors.setSpeeds(0,0);             
 }

//-----------------------------------------
//            GO BACKWARD
//-----------------------------------------
 
 void goBack(){
    Serial.println("Zumo is reversing");
    Serial.println("-------------------------------"); 

    for(int i = 0; i < 80; i++)
    {
      if ((i > 10 && i <= 30) || (i > 50 && i <= 70))
      motors.setSpeeds(-200, -200);
      delay(10);
    }
    
    motors.setSpeeds(0,0);            
 }

//-----------------------------------------
//            GO LEFT
//-----------------------------------------
 void goLeft(){
    Serial.println("Zumo is turning left");
    Serial.println("-------------------------------");

    for(int i = 0; i < 80; i++)
    {
      if ((i > 10 && i <= 30) || (i > 50 && i <= 70))
      motors.setSpeeds(-100, 150);
      delay(10);
    }
    
    motors.setSpeeds(0,0);            
 }
 
//-----------------------------------------
//            GO RIGHT
//-----------------------------------------
 void goRight(){
    Serial.println("Zumo is turning right");
    Serial.println("-------------------------------");

    for(int i = 0; i < 80; i++)
    {
      if ((i > 10 && i <= 30) || (i > 50 && i <= 70))
      motors.setSpeeds(150, -100);
      delay(10);
    }
    
    motors.setSpeeds(0,0);         
 }

 //-----------------------------------------
//            GO FORWARD A BIT
//-----------------------------------------

void goForwardABit(){
     Serial.println("Zumo is going straight");

    for(int i = 0; i < 80; i++)
    {
      if ((i > 10 && i <= 30) || (i > 50 && i <= 70))
      motors.setSpeeds(200, 200);
      delay(3);
    }
    
    motors.setSpeeds(0,0); 
    Serial.println("-------------------------------");            
 }
 
//-----------------------------------------
//            REVERSE A BIT
//-----------------------------------------
 
 void reverseABit(){
   Serial.println("Zumo is reversing a bit...");

   for(int i = 0; i < 80; i++)
  {
    if ((i > 10 && i <= 30) || (i > 50 && i <= 70))
    motors.setSpeeds(-200, -200);
    delay(3);
  }
  
  motors.setSpeeds(0,0); 
  Serial.println("-------------------------------");            
 } 

//-----------------------------------------
//            GO RIGHT A BIT
//-----------------------------------------
 void goUpperRight(){
    Serial.println("Zumo is moving a little bit to the right");

    for(int i = 0; i < 80; i++)
    {
      if ((i > 10 && i <= 30) || (i > 50 && i <= 70))
      motors.setSpeeds(150, -100);
      delay(3);
    }
    
    motors.setSpeeds(0,0); 
    Serial.println("-------------------------------");    
 }

//-----------------------------------------
//            GO LEFT A BIT
//-----------------------------------------
 void goUpperLeft(){
    Serial.println("Zumo is moving a little bit to the left");

    for(int i = 0; i < 80; i++)
    {
      if ((i > 10 && i <= 30) || (i > 50 && i <= 70))
      motors.setSpeeds(-100, 150);
      delay(3);
    }
    
    motors.setSpeeds(0,0);  
    Serial.println("-------------------------------");           
 }

//-----------------------------------------
//            U-TURN AT THE RIGHT WING
//-----------------------------------------
void uTurnRightWing(){
  reverseABit();
  delay(40);
  
  goRight();
  delay(40);
  
  goRight();
  delay(40);

  autonomous();  
}

//-----------------------------------------
//            U-TURN AT THE LEFT WING
//-----------------------------------------
void uTurnLeftWing(){
  reverseABit();
  delay(40);
  
  goLeft();
  delay(40);
  
  goLeft();
  delay(40);

  autonomous();  
}

//-----------------------------------------
//            GO AUTONOMOUSLY
//-----------------------------------------
void autonomous(){    
  bool canMove = true;

  Serial.println("Moving in the corridor...");    
    while(canMove){
       sensors.read(sensor_values);
       //Serial.println("in autonomous mode");
       //if (canMove)Serial.println("moving...");
         // else Serial.println("stopped"); 
         if (sensor_values[1] > QTR_THRESHOLD)
          {
            // if leftmost sensor detects line, reverse and turn to the right
            motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
            delay(REVERSE_DURATION);
            motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
            delay(TURN_DURATION);
            motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
            //Serial.println("pass sensor 0");
          }
          else if (sensor_values[1] > 200)
          {
            //Serial.println("pass sensor 1");
            motors.setSpeeds(0,0);
            canMove = false;  
          }
          else if (sensor_values[2] > 200)
          {
            //Serial.println("pass sensor 2");
            motors.setSpeeds(0,0);
            canMove = false;
            
          }
          else if (sensor_values[3] > 200)
          {
            //Serial.println("pass sensor 3");
            motors.setSpeeds(0,0);
            canMove = false;
            
          }
          else if (sensor_values[4] > 200)
          {
            //Serial.println("pass sensor 4");
            motors.setSpeeds(0,0);
            canMove = false;
            
          }
          else if (sensor_values[5] > QTR_THRESHOLD)
          { 
            // if rightmost sensor detects line, reverse and turn to the left
            motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
            delay(REVERSE_DURATION);
            motors.setSpeeds(-TURN_SPEED, TURN_SPEED);
            delay(TURN_DURATION);
            motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
            //Serial.println("pass sensor 5");
          }
          else
          {
            motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
          } 
      } 
      Serial.println("STOP");
      delay(1000);
  }

//---------------------------------------------------
// AUTONOMOUS ROOM //
// - When the Zumo has entered a room (right or left), 
//   it now moves autonomously and scan the room. 
//---------------------------------------------------
  void autonomousRoom(){
    Serial.println("Autonomous Room");
    digitalWrite(LED_PIN, HIGH);
    
      for(int i = 0; i < 80; i++)
      {
        if ((i > 10 && i <= 30) || (i > 50 && i <= 70))
        {
          motors.setSpeeds(-200, 200);
          if (sonar.ping_cm() > 0)
          {
           Serial.print("Ping: ");
           Serial.print(sonar.ping_cm());
           Serial.println("cm");
           Serial.println("PERSON FOUND IN THE ROOM!");
           Serial.println("-------------------------------");
           delay(20);
          }
        }
        else
        {
         motors.setSpeeds(200, -200);
         if (sonar.ping_cm() > 0)
          {
           Serial.print("Ping: ");
           Serial.print(sonar.ping_cm());
           Serial.println("cm");
           Serial.println("PERSON FOUND IN THE ROOM!");
           Serial.println("-------------------------------");
           delay(20);
          }
        } 
        delay(40);
      }
      motors.setSpeeds(0,0);
      
    }
 
//-----------------------------------------
//          FOUND ROOM ON THE RIGHT //r
//-----------------------------------------
 void foundRoomRight(){

    Serial.println("Found a room to your right!");
    Serial.println("-------------------------------");
    
    goRight();
    delay(40);
    
    Serial.println("Now entering the room...");
    Serial.println("-------------------------------");

    
    for(int i = 0; i < 80; i++)
    {
      if ((i > 10 && i <= 30) || (i > 50 && i <= 70))
      motors.setSpeeds(100, 100);
      delay(5);
    }
    
    delay(100);
    autonomousRoom(); 

  }

//-----------------------------------------
//         FOUND ROOM ON THE LEFT SIDE 
//-----------------------------------------
 void foundRoomLeft(){
    Serial.println("Found a room to your left!");
    Serial.println("-------------------------------");
    
    goLeft();
    delay(40);
    
    Serial.println("Now entering the room...");
    Serial.println("-------------------------------");

    for(int i = 0; i < 80; i++)
    {
      if ((i > 10 && i <= 30) || (i > 50 && i <= 70))
      motors.setSpeeds(100, 100);
      delay(5);
    }

    delay(40);
    autonomousRoom(); 

    

  }
  
//-----------------------------------------
//     FINISH SCANNING THE RIGHT ROOM //n
//-----------------------------------------  
void finishRoomRight(){

    Serial.println("SCANNING COMPLETE");
    Serial.println("Reverse from right");

    for(int i = 0; i < 80; i++)
    {
      if ((i > 10 && i <= 30) || (i > 50 && i <= 70))
      motors.setSpeeds(-150, 0);
      delay(20);
    }
    
    motors.setSpeeds(0,0);             
}


//-----------------------------------------
//     FINISH SCANNING THE LEFTROOM
//-----------------------------------------  
void finishRoomLeft(){
    Serial.println("SCANNING COMPLETE");
    Serial.println("Reverse from left");

    for(int i = 0; i < 80; i++)
    {
      if ((i > 10 && i <= 30) || (i > 50 && i <= 70))
      motors.setSpeeds(-75, -150);
      delay(20);
    }
    
    motors.setSpeeds(0,0);             
}
