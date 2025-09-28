const int LED_PIN = 9;  


const unsigned int PERIOD = 10000;  
const int DUTY= 100;  
int i = 0;
int k = 1;

void setup() {
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  i += k;

  if (i <= 0 || i >= DUTY) {
    k = -k;
  }
  
 
  unsigned int on = (unsigned long)i * PERIOD / 100;


  if (on > 0) {
    digitalWrite(LED_PIN, HIGH);
    delayMicroseconds(on);
    printf(on);
  }


  if (on < PERIOD) {
    digitalWrite(LED_PIN, LOW);
    delayMicroseconds(PERIOD - on);
   printf(PERIOD - on);
  }
}
