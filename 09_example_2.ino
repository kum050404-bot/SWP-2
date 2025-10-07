#define LED_PIN 9
#define TRIG_PIN 12
#define ECHO_PIN 13

#define SOUND_SPEED 346.0
#define SAMPLE_INTERVAL 25
#define PULSE_TIME 10
#define DIST_MIN 100
#define DIST_MAX 300

#define ECHO_TIMEOUT ((SAMPLE_INTERVAL / 2) * 1000.0)
#define DIST_SCALE (0.001 * 0.5 * SOUND_SPEED)

#define EMA_WEIGHT 0.1

#define WINDOW_SIZE 30
float data_buffer[WINDOW_SIZE];
int data_index = 0;
int data_count = 0;

unsigned long prev_sample_time;
float distance_ema;

float getMedianValue() {
  float temp[WINDOW_SIZE];
  for (int i = 0; i < data_count; i++) temp[i] = data_buffer[i];

  for (int i = 1; i < data_count; i++) {
    float key = temp[i];
    int j = i - 1;
    while (j >= 0 && temp[j] > key) {
      temp[j + 1] = temp[j];
      j--;
    }
    temp[j + 1] = key;
  }

  if (data_count % 2 == 1)
    return temp[data_count / 2];
  else
    return (temp[data_count / 2 - 1] + temp[data_count / 2]) / 2.0;
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  digitalWrite(TRIG_PIN, LOW);
  Serial.begin(57600);
}

void loop() {
  float distance_raw, distance_median;

  if (millis() < prev_sample_time + SAMPLE_INTERVAL)
    return;

  distance_raw = measureDistance(TRIG_PIN, ECHO_PIN);

  data_buffer[data_index] = distance_raw;
  data_index = (data_index + 1) % WINDOW_SIZE;
  if (data_count < WINDOW_SIZE) data_count++;

  distance_median = getMedianValue();
  distance_ema = EMA_WEIGHT * distance_median + (1 - EMA_WEIGHT) * distance_ema;

  Serial.print("Min:");    Serial.print(DIST_MIN);
  Serial.print(",raw:");   Serial.print(distance_raw);
  Serial.print(",ema:");   Serial.print(distance_ema);
  Serial.print(",median:");Serial.print(distance_median);
  Serial.print(",Max:");   Serial.print(DIST_MAX);
  Serial.println("");

  if ((distance_median < DIST_MIN) || (distance_median > DIST_MAX))
    digitalWrite(LED_PIN, 1);
  else
    digitalWrite(LED_PIN, 0);

  prev_sample_time += SAMPLE_INTERVAL;
}

float measureDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(PULSE_TIME);
  digitalWrite(trigPin, LOW);
  return pulseIn(echoPin, HIGH, ECHO_TIMEOUT) * DIST_SCALE;
}
