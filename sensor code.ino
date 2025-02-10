#include <SoftwareSerial.h>
int pwmPin=6;
  int pwmValue=20;
SoftwareSerial BT(8, 9);

void setup() {
  pinMode(6, OUTPUT);
  analogWrite(pwmPin, pwmValue);
    BT.begin(9600);
    Serial.begin(9600);
}

void loop() {
      
    double Vmax = 4.8782;
    delay(2000);
    Serial.println("Ensure the sensor is connected");
    BT.println("Ensure the sensor is connected");

    double val0 = analogRead(A0);  // Read digital value of V
    double V0 = val0 * 5 / 1023;   // Obtain stable V
    double R0 = (1000 * V0) / (Vmax - V0);  // Obtain stable R

    Serial.println("Ensure the sensor is connected");
    BT.println("Ensure the sensor is connected");

    // Sensor connection validation loop
    for (int b = 0; b < 10;) {
        val0 = analogRead(A0);   // Read digital value of V
        V0 = val0 * 5 / 1023;    // Obtain stable V
        R0 = (1000 * V0) / (Vmax - V0);  // Obtain stable R

        if (R0 < 60000 && R0 > 0) {
            Serial.print("R = ");
            Serial.println(R0);
            BT.print("R = ");
            BT.println(R0);
            delay(990);
            b++;
        } else {
            Serial.print("R = ");
            Serial.println(R0);
            BT.print("R = ");
            BT.println(R0);
            b = 0;
            Serial.println("Please recheck the sensor connection");
            BT.println("Please recheck the sensor connection");
            delay(990);
        }
    }

    Serial.println("Connection confirmed");
    BT.println("Connection confirmed");
    delay(1000);

    Serial.println("Countdown in 60 seconds");
    BT.println("Countdown in 60 seconds");
    delay(1000);

    // Countdown loop to collect data
    double Rsum = 0;
    double Vsum = 0;
    for (int b = 1; b < 61; b++) {
        val0 = analogRead(A0);   // Read digital value of V
        V0 = val0 * 5 / 1023;    // Obtain stable V
        R0 = (1000 * V0) / (Vmax - V0);  // Obtain stable R
        Vsum += V0;
        Rsum += R0;

        Serial.print("Ready to drop in __");
        Serial.print(60 - b);
        Serial.println("__ seconds");
        BT.print("Ready to drop in __");
        BT.print(60 - b);
        BT.println("__ seconds");
        delay(995);
    }

    double Ravg = Rsum / 60;
    double Vavg = Vsum / 60;
    Serial.println("DROP!");
    BT.println("DROP!");

    Serial.print("Average sensor resistance = ");
    Serial.println(Ravg);
    BT.print("Average sensor resistance = ");
    BT.println(Ravg);

    // Measurement loop for 240 seconds
    for (int i = 0; i < 240; i++) {
        double val1 = analogRead(A0);  // Read V1 value
        double V1 = val1 * 5 / 1023;   // Obtain V1
        double R1 = (1000 * V1) / (Vmax - V1);  // Obtain R1
        double deltaV = V1 - Vavg;  // Change in V
        double deltaR = R1 - Ravg;  // Change in R
        double x1 = ((deltaR / Ravg) - 0.3476) / 0.2899;
        double ppm1 = pow(10, x1);  // Calculate ppm

        if (ppm1 <= 0) {
            Serial.println("Not detected");
            BT.println("Not detected");
            Serial.print("R1 = ");
            Serial.println(R1);
            BT.print("R1 = ");
            BT.println(R1);
        } else if (ppm1 > 0 && ppm1 < 500) {
            Serial.print("ppm = ");
            Serial.println(ppm1);
            BT.print("ppm = ");
            BT.println(ppm1);
            Serial.print("R1 = ");
            Serial.println(R1);
            BT.print("R1 = ");
            BT.println(R1);
        } else if (ppm1 > 500) {
            Serial.println("Out of the available detecting range");
            BT.println("Out of the available detecting range");
            Serial.print("R1 = ");
            Serial.println(R1);
            BT.print("R1 = ");
            BT.println(R1);
        }
        delay(995);  // Delay to prevent rapid looping
    }
}
