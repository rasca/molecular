#include <NewPing.h>

#define TRIGGER_PIN 12
#define ECHO_PIN 14
#define MAX_DISTANCE 400

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  delay(60);
  Serial.print("Ping: ");
  Serial.print(sonar.ping_cm());
  Serial.println("cm");
}