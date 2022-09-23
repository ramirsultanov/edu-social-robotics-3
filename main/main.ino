#include <SoftwareSerial.h>

const int ENA = 5;
const int ENB = 6;
const int IN1 = 7;
const int IN2 = 8;
const int IN3 = 9;
const int IN4 = 10;
const int TRIG = 12;
const int ECHO = 13;

const int LPWR = 100;
const int RPWR = 255;

const int TX = 1;
const int RX = 0;

class Robot {
public:
  void init();
  int getDistanceToObstacle() const;
  void move(int left, int right);
  void forward(float vel);
  void backward(float vel);
  void left(float vel);
  void right(float vel);
  void stop();
protected:
private:
};

Robot robot;

void setup() {
  robot.init();
}

void loop() {
  if (Serial.available()) {
    int r = Serial.read();
    switch (r) {
    case 'f':
      robot.forward(1.f);
      break;
    case 'b':
      robot.backward(1.f);
      break;
    case 'l':
      robot.left(1.f);
      break;
    case 'r':
      robot.right(1.f);
      break;
    case 's':
      robot.stop();
      break;
    default:
      break;
    }
  }
}

void Robot::init() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  Serial.begin(9600);
}

int Robot::getDistanceToObstacle() const {
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  long t = pulseIn(ECHO, HIGH);
  return t / 58;
}

void Robot::move(int left, int right) {
  int l = 0;
  if (left < 0) {
    l = -left;
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
  } else {
    l = left;
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
  }
  analogWrite(ENA, l);

  int r = 0;
  if (right < 0) {
    r = -right;
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
  } else {
    r = right;
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  }
  analogWrite(ENB, r);
}

void Robot::forward(float vel) {
  return this->move(vel * LPWR, vel * RPWR);
}

void Robot::backward(float vel) {
  return this->move(-vel * LPWR, -vel * RPWR);
}

void Robot::left(float vel) {
  return this->move(-vel * LPWR, vel * RPWR);
}

void Robot::right(float vel) {
  return this->move(vel * LPWR, -vel * RPWR);
}

void Robot::stop() {
  return move(0, 0);
}
