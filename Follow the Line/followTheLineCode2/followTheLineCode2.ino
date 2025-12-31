// Initialising our pins 
//DO NOT TOUCH OR MODIFY PINS
const int MOD = 3;

const int ENA = 5;   // Left motor PWM (Capable of manipulating speed for left motors)
const int PhA = 7;   // Left motor direction
const int ENB = 6;   // Right motor PWM (Capable of manipulating speed for right motors)
const int PhB = 8;   // Right motor direction

//Pins declared as const so they remain UNCHANGED.

// Sensor pins
const int R_S = A0;  // Right sensor
const int M_S = A1;  // Middle sensor
const int L_S = A2;  // Left sensor

int R, M, L;         // Sensor readings

// --- moved to global scope so readSensors() can use them ---
int THRESH = 500;    // threshold for analog -> digital
int L_d = 0;
int M_d = 0;
int R_d = 0;

int carSpeed;  // Forward speed - declared normally, as we may want to modify speed. 0 - 255.
int memory = 0;


void setup() {
  Serial.begin(9600); // Rate of baud used for communication - set as 9600. 

  // Initialising our pins as output
  pinMode(MOD, OUTPUT); 
  pinMode(ENA, OUTPUT);
  pinMode(PhA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(PhB, OUTPUT);

  // Sensors (analogue inputs do not need pinMode)
  
  //Think of this as a switch - it just simply turns on the Arduino. 
  digitalWrite(MOD, HIGH);

  // Delay in milliseconds
  delay(500);
}

void loop() {
  // READ SENSORS
  readSensors();

  Serial.print("L: "); Serial.print(L);
  Serial.print(" M: "); Serial.print(M);
  Serial.print(" R: "); Serial.println(R);

  // --- NORMAL LINE FOLLOWING LOGIC ---

  if (M_d == 1 && L_d == 0 && R_d == 0) {
      move_forward(250);          // straight
  }
  else if (L_d == 1 && M_d == 1 && R_d == 0) {
      sharp_left(140);            // hard left
  }
  else if (R_d == 1 && M_d == 1 && L_d == 0) {
      sharp_right(140);           // hard right
  }
  else if (L_d == 1 && M_d == 1 && R_d == 1) {
      move_forward(250);          // all sensors on line
  }
  else if (L_d == 1 && M_d == 0 && R_d == 0) {
      curve_left(130);            // slight left
  }
  else if (R_d == 1 && M_d == 0 && L_d == 0) {
      curve_right(130);           // slight right
  }

  // --- LOST LINE LOGIC (ONLY if 000) ---
    else if (L_d == 0 && M_d == 0 && R_d == 0) {
      // Peek LEFT
      sharp_left(150);
      delay(50);
      readSensors();
      if (L_d || M_d || R_d) return;

      // Peek RIGHT
      sharp_right(150);
      delay(100);
      readSensors();
      if (L_d || M_d || R_d) return;

      // Forward crawl to find line
      move_forward(150);
      delay(60);
      readSensors();
  }
}


// helper that reads analog sensors and sets digital flags
void readSensors() {
    L = analogRead(L_S);
    M = analogRead(M_S);
    R = analogRead(R_S);

    L_d = (L < THRESH) ? 0 : 1;
    M_d = (M < THRESH) ? 0 : 1;
    R_d = (R < THRESH) ? 0 : 1;
}


void move_forward(int carSpeed) {
  // LEFT motor
  analogWrite(ENA, carSpeed); // ENA controls the speed of the left motors - we use PWM to assign the amount of speed we want to apply to it. 
  digitalWrite(PhA, HIGH); // PhA controls the actual direction of the left motor - HIGH means Forward. 

  // RIGHT motor
  analogWrite(ENB, carSpeed); // ENB controls the speed of the right motors - we use PWM to assign the amount of speed we want to apply to it. 
  digitalWrite(PhB, HIGH); // PhB controls the actual direction of the right motor - HIGH means Forward. 
}
void sharp_left(int carSpeed){
  //LEFT motor
  analogWrite(ENA, carSpeed* 0.4);
  digitalWrite(PhA, LOW);
  //RIGHT motor
  analogWrite(ENB, carSpeed);
  digitalWrite(PhB, HIGH);
}
void sharp_right(int carSpeed){
  //LEFT motor
  analogWrite(ENA, carSpeed);
  digitalWrite(PhA, HIGH);
  //RIGHT motor
  analogWrite(ENB, carSpeed* 0.4);
  digitalWrite(PhB, LOW);
}
void curve_left(int carSpeed){
  analogWrite(ENA, carSpeed * 0.85); // smaller difference
  digitalWrite(PhA, HIGH);
  analogWrite(ENB, carSpeed);
  digitalWrite(PhB, HIGH);
}
void curve_right(int carSpeed){
  analogWrite(ENA, carSpeed);
  digitalWrite(PhA, HIGH);
  analogWrite(ENB, carSpeed * 0.85);
  digitalWrite(PhB, HIGH);
}
void stop_car() {
  analogWrite(ENA, 0); // We set the carSpeed for left motors to 0 - this will simply depower the left motors. 
  digitalWrite(PhA, LOW); // Left Motor Direction has been set to LOW - LOW means Backward.
  analogWrite(ENB, 0); // We set the carSpeed for right motors to 0 - this will simply depower the right motors. 
  digitalWrite(PhB, LOW); // Right Motor Direction has been set to LOW - LOW means backward. 
}
