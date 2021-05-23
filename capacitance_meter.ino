/****************************************************************************************************************************************
 * 
 * Author:    Aly Khan Nuruddin
 * Date:      May 11, 2021
 * Purpose:   This program permits the user to determine the capacitance of a test capacitor by setting up a resistance-capacitor circuit 
 *            with known resistance and computing the corresponding time constant for discharge of voltage by two-thirds of its capacity.
 *            
 ****************************************************************************************************************************************/

int inputPin = A0;                                  // Define the Analog Input Pin that "senses" the output voltage.
int outputPin = A2;                                 // Define the Analog Output Pin that provides a connection to ground.

int sensorValue = 0;                                // Initialize the value of the sensor at the Analog Input Pin.
int maxAnalog = 1024;                               // Initialize the value of the maximum analog voltage reading.

float extraCapacitance = 0.0;                       // Initialize the value of the extra capacitance in nanofarads (nF).
float strayCapacitance = 24.48;                     // Initialize the value of the stray capacitance in nanofarads (nF).
float groundCapacitance = strayCapacitance;         // Initialize the value of the ground capacitance in nanofarads (nF).
float pullupResistance = 34.8;                      // Initialize the value of the pullup resistance in Ohms (Ω).

float buffer = 0.0;                                 // Initialize the sensorValue of the placeholder for calculations.

void setup(){
  pinMode(outputPin, OUTPUT);                       // Configure the Analog Output Pin to be recognized as an Output.
  pinMode(inputPin, OUTPUT);                        // Configure the Analog Input Pin to be recognized as an Output.
  Serial.begin(9600);                               // Initialize communication with the Serial Monitor at 9600 baud.
}

void loop(){
    pinMode(inputPin, INPUT);                       // Configure the Analog Input Pin to be recognized as an Input.
    digitalWrite(outputPin, HIGH);                  // Set the ouput voltage value at the Analog Output Pin to 5 V.
    sensorValue = analogRead(inputPin);             // Read the output voltage from the Analog Input Pin.
    digitalWrite(outputPin, LOW);                   // Set the ouput voltage value at the Analog Output Pin to 0 V.

    if (sensorValue < 1000){                        // Define the condition where the analog voltage reading is less than 1000. 
      pinMode(inputPin, OUTPUT);                    // Configure the Analog Input Pin to be recognized as an Output.

      float capacitance = (float)sensorValue * groundCapacitance / (float)(maxAnalog - sensorValue);    // Compute the capacitance of the capacitor in picofarads (pF).

      Serial.print(F("Capacitance = "));            // Define the character string for the capacitance.
      Serial.print(capacitance, 3);                 // Print the value of the capacitance on the Serial Monitor.
      Serial.print(F(" pF ("));                     // Print the prefix for 'pico' on the Serial Monitor.
      Serial.print(sensorValue);                    // Print the value of the output voltage on the Serial Monitor.
      Serial.println(F(") "));                      // Print a 'space' on the Serial Monitor.
    }

    else{                                           // Define the condition where the analog voltage reading is greater than 1000.
      pinMode(inputPin, OUTPUT);                    // Configure the Analog Input Pin to be recognized as an Output.
      delay(1);                                     // Delay the execution of the program by 1 milliseconds.
      pinMode(outputPin, INPUT_PULLUP);             // Configure the Analog Output Pin to be recognized as an Input for the internal pullup resistor.
      unsigned long startTime = micros();           // Return the number of microseconds for the program start time.
      unsigned long timeConstant;                   // Return the number of microseconds for the program run time.
      int digitalsensorValue;                       // Define the sensor value detected at the Digital Output Pin.

      do{                                               
        digitalsensorValue = digitalRead(outputPin);                                    // Read the output voltage from the Digital Output Pin.
        unsigned long endTime = micros();                                               // Return the number of microseconds for the program end time.
        timeConstant = endTime > startTime ? endTime - startTime : startTime - endTime; // Compute the number of microseconds for the program run time.
      } while ((digitalsensorValue < 1) && ( timeConstant < 400000L));                  // Repeat the loop when sensor value and run time are less than 1 and 400000 Hz respectively.

      pinMode(outputPin, INPUT);                                                        // Configure the Analog Output Pin to be recognized as an Input.
      sensorValue = analogRead(outputPin);                                              // Read the output voltage from the Analog Output Pin.
      digitalWrite(inputPin, HIGH);                                                     // Set the ouput voltage value at the Analog Input Pin to 5 V.
      int dischargeTime = (int)( timeConstant / 1000L) * 5;                             // Compute the number of microseconds required for voltage to discharge.
      delay(dischargeTime);                                                             // Delay the execution of the program by the discharge time.
      pinMode(outputPin, OUTPUT);                                                       // Configure the Analog Output Pin to be recognized as an output.
      digitalWrite(outputPin, LOW);                                                     // Set the ouput voltage value at the Analog Output Pin to 0 V.
      digitalWrite(inputPin, LOW);                                                      // Set the ouput voltage value at the Analog Input Pin to 0 V.

      float capacitance = -(float) timeConstant / pullupResistance / log(1.0 - (float)sensorValue / (float)maxAnalog);    // Compute the capacitance of the capacitor in nanofarads (nF).

      Serial.print(F("Capacitance = "));             // Define the character string for the capacitance.                           
      if (capacitance > 1000.0){                     // Define the condition where the capacitance exceeds 1000 nanofarads (nF).
        Serial.print(capacitance / 1000.0, 2);       // Print the value of the capacitance in microfarads (µF).
        Serial.print(F(" µF"));                      // Print the prefix for 'micro' on the Serial Monitor.
      }
      else{                                          // Define the condition where the capacitance is less than 1000 nanofarads (nF).
        Serial.print(capacitance, 2);                // Print the value of the capacitance in nanofarads (nF).
        Serial.print(F(" nF"));                      // Print the prefix for 'nano' Serial Monitor.
      }

      Serial.print(F(" ("));                                                            // Print a 'space' on the Serial Monitor.
      Serial.print(digitalsensorValue == 1 ? F("Normal") : F("High sensorValue"));      // Print the status of the output voltage as 'Normal or 'High' on the Serial Monitor.  
      Serial.print(F(", en= "));                                                        // Print a 'space' and an 'assignment operator' on the Serial Monitor.
      Serial.print(timeConstant);                                                       // Print the value of the time constant on the Serial Monitor.
      Serial.print(F(" us, ADC= "));                                                    // Print a 'space' and an 'assignment operator' on the Serial Monitor.
      Serial.print(sensorValue);                                                        // Print the value of the output voltage on the Serial Monitor. 
      Serial.println(F(")"));                                                           // Print a 'space' on the Serial Monitor.
    }
    while (millis() % 1000 != 0);                    // Delay the display of successive data values by 1000 milliseconds.
}
