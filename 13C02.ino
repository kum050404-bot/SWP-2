#include <Servo.h>

#define PIN_SERVO 9
#define _SERVO_SPEED1 3.0
#define _SERVO_SPEED2 0.3

Servo myServo;

void setup() {
  myServo.attach(PIN_SERVO);
  myServo.write(0);
  delay(1000);

  Move(0, 180, 60);
  delay(2000);

  Move(0, 90, 300);
  delay(2000);
}

void loop() {
}

void Move(int start, int end, int duration) {
  int step;
  if (end > start) {
    step = 1;
  } else {
    step = -1;
  }

  int angle = start;
  int totalSteps = abs(end - start);
  float delayTime = (duration * 1000.0) / totalSteps;

  for (int i = 0; i <= totalSteps; i++) {
    myServo.write(angle);
    angle += step;
    delay((unsigned long)delayTime);
  }
}
