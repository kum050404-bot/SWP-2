
#define PIN_LED  9
#define PIN_TRIG 12
#define PIN_ECHO 13


#define SND_VEL 346.0
#define INTERVAL 25
#define PULSE_DURATION 10
#define _DIST_MIN 100.0
#define _DIST_MAX 300.0

#define TIMEOUT ((INTERVAL / 2) * 1000.0)
#define SCALE (0.001 * 0.5 * SND_VEL)

unsigned long last_sampling_time;

void setup() {
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  digitalWrite(PIN_TRIG, LOW);
  Serial.begin(57600);
}

void loop() {
  float dis;
  int duty;

  if (millis() < (last_sampling_time + INTERVAL)) {
    return;
  }
  last_sampling_time += INTERVAL;

  dis = USS_measure(PIN_TRIG, PIN_ECHO);

  if (dis >= 100.0 && dis <= 200.0) {
    duty = (int)(((200.0 - dis) / 100.0) * 255.0);
  } else if (dis > 200.0 && dis <= 300.0) {
    duty = (int)(((dis - 200.0) / 100.0) * 255.0);
  } else if (dis > 300.0) {
    duty = 255;
  } else {
    duty = 255;
  }

  if (dis >= 149.0 && dis <= 151.0) {
    duty = (int)(0.5 * 255);
  }
  if (dis >= 249.0 && dis <= 251.0) {
    duty = (int)(0.5 * 255);
  }

  duty = constrain(duty, 0, 255);

  analogWrite(PIN_LED, duty);

  Serial.print("Distance:");
  Serial.print(dis);
  Serial.print("mm, LED_Duty:");
  Serial.println(duty);
}

float USS_measure(int TRIG, int ECHO)
{
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(PULSE_DURATION);
  digitalWrite(TRIG, LOW);

  return pulseIn(ECHO, HIGH, TIMEOUT) * SCALE;
}

