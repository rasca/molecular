#include <NewPing.h>

#define TRIGGER_PIN 12
#define ECHO_PIN 14
#define MAX_DISTANCE 200

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

void setup()
{
  Serial.begin(9600);
}

const double alpha = 0.1;
double result = 0;

double cm;
double cm_smooth = 0;

int zeroes_count = 0;
int zeroes_threshold = 5;

void loop()
{
  delay(60);
  cm = sonar.ping_cm();
  
  if (cm == 0) {
    zeroes_count++;
  } else {
    zeroes_count = 0;
  }

  if (zeroes_count == 0 || zeroes_count > zeroes_threshold) {
    cm_smooth = cm_smooth * (1  - alpha) + cm * alpha;
  }

  Serial.print("cm: ");
  Serial.print(cm);
  Serial.print(" smooth: ");
  Serial.println(cm_smooth);

}