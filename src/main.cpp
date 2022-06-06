///// Sensor /////
#include <NewPing.h>

#define TRIGGER_PIN 12
#define ECHO_PIN 14
#define MAX_DISTANCE 200

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

///// First order filter /////
///// and ignore first zeroes /////
const double alpha = 0.051;
const int zeroes_threshold = 15;
int zeroes_count = 0;

double measurament;
double filtered = 0;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  delay(60);
  measurament = sonar.ping_cm();
  
  if (measurament == 0) {
    zeroes_count++;
  } else {
    zeroes_count = 0;
  }

  if (zeroes_count == 0 || zeroes_count > zeroes_threshold) {
    filtered = filtered * (1  - alpha) + measurament * alpha;
  }

  Serial.print("measurament: ");
  Serial.print(measurament);
  Serial.print(" smooth: ");
  Serial.println(filtered);

}