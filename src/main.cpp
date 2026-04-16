#include <Arduino.h>
#include "HX711.h"

// Pin definitions
const uint8_t DT_PIN = 32;
const uint8_t SCK_PIN = 33;

HX711 scale;

// Adjust this calibration factor after calibration
float calibration_factor = 1.0; 

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n--- AE-HX711 Load Cell Demo ---");
  Serial.println("Initialising the scale...");

  scale.begin(DT_PIN, SCK_PIN);

  // Reset to default
  scale.reset();

  Serial.println("Taring... Please ensure no weight is on the scale.");
  scale.tare(20); // Average of 20 readings for better accuracy during tare
  
  scale.set_scale(calibration_factor);

  Serial.println("Scale ready!");
  Serial.println("Commands:");
  Serial.println("  't' - Tare (zero) the scale");
  Serial.println("  'k' - Calibration mode (helper)");
  Serial.println("  '+' - Increase calibration factor by 10");
  Serial.println("  '-' - Decrease calibration factor by 10");
  Serial.println("----------------------------------------");
}

void loop() {
  if (scale.is_ready()) {
    // Read weight (average of 5 readings)
    float weight = scale.get_units(5);
    
    Serial.print("Weight: ");
    Serial.print(weight, 2); // 2 decimal places
    Serial.print(" units | Calibration Factor: ");
    Serial.println(scale.get_scale());
  } else {
    Serial.println("HX711 not found.");
  }

  // Handle Serial commands
  if (Serial.available()) {
    char temp = Serial.read();
    if (temp == 't' || temp == 'T') {
      Serial.println("Taring...");
      scale.tare(10);
      Serial.println("Done.");
    } 
    else if (temp == '+') {
      calibration_factor += 10;
      scale.set_scale(calibration_factor);
    } 
    else if (temp == '-') {
      calibration_factor -= 10;
      scale.set_scale(calibration_factor);
    }
    else if (temp == 'k' || temp == 'K') {
      Serial.println("\n--- Calibration Instructions ---");
      Serial.println("1. Clear the scale and press 't' to tare.");
      Serial.println("2. Place a known weight (e.g., 500g) on the scale.");
      Serial.print("3. Read the current value. If it says 1000 and you put 500g, ");
      Serial.println("your factor should be roughly (Current Value / Known Weight).");
      Serial.println("4. Use '+' and '-' to adjust until the Weight matches your known weight.");
      Serial.println("----------------------------------\n");
    }
  }

  delay(500);
}
