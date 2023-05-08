/*
 * Erin Butler, Nithya Muralidaran, Arunagiri Ravichandran
 * Robots for Disability - ROBGY 6413
 * Final Prototype Project
 * Sensorized Therapy "Ball" for Improved Rehabilitation Feedback
 * Professor Kapilla
 * Fall 2022
 */

// Initialize variable for how many pressure sensor pins there are. 
#define count 5

// Initialize array of sensor pins.
byte Pins[count] = {A0, A1, A2, A3, A4};
// Initialize array of LEDs
int LEDs[count] = {3, 4, 5, 6, 7};

//Initialize calibration values.
int minimum[count];
int maximum[count]; 
float goal = .8; // Goal pressure percentage as a decimal. 
int threshold[count];  

// Initialize an array for storing the voltages read from the sensor pins. 
int voltages[count];

// Initialize functions. 
void calibrateMode();

void setup() {
  Serial.begin(9600); // For debug

  // Set the analog pins to input. Set the LED pins to output. 
  for (int i = 0; i < count; i++){
    pinMode(Pins[i], INPUT);
    pinMode(LEDs[i], OUTPUT);
  }
}

void loop() {
  /* 
   * Run calibration to get minimum and maximum values. Calibration 
   * needs to happen every time the device is turned on, especially 
   * when device is switched between hands. Calibration will also 
   * determine threshold values for each sensor.
   */ 
  Serial.println("Running calibration."); // For debug. 
  calibrateMode();
  delay(2000);
  Serial.println("MIN   MAX   THRESHOLD"); // For debug.
  Serial.print(minimum[0]); // For debug.
  Serial.print("\t"); // For debug.
  Serial.print(maximum[0]); // For debug.
  Serial.print("\t"); // For debug.
  Serial.println(threshold[0]); // For debug.

  /* 
   *  The following while loop is run infinitely until therapy is 
   *  finished, the device is switched to a new hand, or a new 
   *  exercise is to be completed. 
   */
  while(1){
    Serial.println("Begin therapy."); // For debug.
    while(1){
      // Read the voltage value of each of the pressure sensors. 
      for (int i = 0; i < count; i++) {
        voltages[i] = analogRead(Pins[i]);
      }
      Serial.print(voltages[0]); // For debug.
      Serial.print("\t"); // For debug.
      Serial.println(threshold[0]); // For debug.
  
      // Turn on LED if corresponding sensor has reached its threshold. 
      for (int i = 0; i < count; i++){
        if (voltages[i] > threshold[i]) {
          digitalWrite(LEDs[i], HIGH);
        }
        // Turn LED off if measured value falls below the threshold.
        else {
          digitalWrite(LEDs[i], LOW);
        }
      }
      delay(80);
    } 
  }
}

void calibrateMode(){
  // Reset the minimum and maximum array values.
  for (int i = 0; i < count; i++){
    minimum[i] = 1000;
    maximum[i] = 0; 
  }

  // Flash all LEDs 5 times to indicate entering calibration mode. 
  for (int i = 0; i < 5; i++){
    for (int j = 0; j < count; j++){
      digitalWrite(LEDs[j], HIGH);
    }
    delay(300);
    for (int j = 0; j < count; j++){
      digitalWrite(LEDs[j], LOW);
    }
    delay(300);    
  }

  // Instruct user to hold the device loosely.
  Serial.println("Hold the device loosely.");
  // Turn the green LED on to indicate to the user to hold loosely.
  digitalWrite(LEDs[2], HIGH);
  delay(1000);
  // For each pressure sensor, find the minimum value detected. 
  int t = millis();
  while((millis() - t) < 5000){
    for (int i = 0; i < count; i++) {
      voltages[i] = analogRead(Pins[i]);
      if (voltages[i] < minimum[i]){
        minimum[i] = voltages[i];
      }
    }
  }
  digitalWrite(LEDs[2], LOW);

  // Instruct user to hold the device tightly.
  Serial.println("Hold the device tightly.");
  // Turn the red LED on to indicate to the user to hold tightly.
  digitalWrite(LEDs[1], HIGH);
  delay(1000);
  // For each pressure sensor, find the maximum value detected. 
  t = millis();
  while((millis() - t) < 5000){
    for (int i = 0; i < count; i++) {
      voltages[i] = analogRead(Pins[i]);
      if (voltages[i] > maximum[i]){
        maximum[i] = voltages[i];
      }
    }
  }
  digitalWrite(LEDs[1], LOW);

  /* 
   *  Based on the goal percentage set above, determine threshold values
   *  for each sensor.
   */
  float temp[count];
  for (int i = 0; i < count; i++){
    temp[i] = (float (maximum[i] - minimum[i])) * goal;
    threshold[i] = minimum[i] + int (temp[i]);
  }  

  // Flash all LEDs 5 times to indicate exiting calibration mode. 
  for (int i = 0; i < 5; i++){
    for (int j = 0; j < count; j++){
      digitalWrite(LEDs[j], HIGH);
    }
    delay(300);
    for (int j = 0; j < count; j++){
      digitalWrite(LEDs[j], LOW);
    }
    delay(300);    
  }
}
