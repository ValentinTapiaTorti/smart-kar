#include <SoftwareSerial.h>

SoftwareSerial serialBlue(11, 12); //Rx, Tx.
int command = 0;
int lastCommand = 0;
int carSpeed = 150;

bool automatic = false;
int trig = 3;
int echo = 2;
int duration = 0;
int distance = 10;

int enA = 10;
int in1 = 9;
int in2 = 8;
int in3 = 7;
int in4 = 6;
int enB = 5;

void setup() {
  // put your setup code here, to run once:  
  serialBlue.begin(38400);
  Serial.begin(38400);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(enB, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (serialBlue.available() > 0) {
    command = serialBlue.read();
    executeAction();
  }
  if (automatic) {
    automaticDriving();
  }
}

void automaticDriving() {
 getDistance();
  if(distance > 10) {
    up();
  } else {
    leftReverse();
    delay(900);
    up();
  }
}

void getDistance() {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);

  digitalWrite(trig, HIGH);
  delayMicroseconds(10);

  digitalWrite(trig, LOW);
  duration = pulseIn(echo, HIGH);

  distance = duration/58.2;
}

void executeAction() {
  switch (command) {
    case 'u':
      up();
      changeCommand();
      break;
    case 'd':
      down();
      changeCommand();
      break;
    case 'l':
      if (lastCommand == 'd')
        leftReverse();
      else
        left();
      changeCommand();
      break;
    case 'r':
      if (lastCommand == 'd')
        rightReverse();
      else
        right();
      changeCommand();
      break;
    case 's':
      stopKar();
      changeCommand();
      break;
    case 'a':
      automatic = !automatic;
      if(!automatic) stopKar(); 
      else {
        carSpeed = 180;
      }
      break;
    case 'v':
      while (true) {
        String auxSpeed = ""; //This is on charge of keep all the numbers that comform the speed value
        while (serialBlue.available() > 0) {
          command = serialBlue.read(); //Cach each number of the speed
          if (command != '\n') //command == '\n' means that the speed has been totally pass
            auxSpeed += (char)command;
          else
            break;
        }
        carSpeed = auxSpeed.toInt();
        break;
      }
      command = lastCommand;
      executeAction();
      break;
  }
}

void changeCommand() {
  lastCommand = command;
  command = 0;
}

void up() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);

  analogWrite(enA, carSpeed);
  analogWrite(enB, carSpeed);
}

void down() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);

  analogWrite(enA, carSpeed);
  analogWrite(enB, carSpeed);
}

void left() {
  stopKar();
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);

  analogWrite(enB, carSpeed);
}

void leftReverse() {
  stopKar();
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);

  analogWrite(enA, carSpeed);
}

void right() {
  stopKar();
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);

  analogWrite(enA, carSpeed);
}

void rightReverse() {
  stopKar();
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);

  analogWrite(enB, carSpeed);
}

void stopKar() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

