// Initialising our pins 
//DO NOT TOUCH OR MODIFY PINS
const int MOD = 3;

const int ENA = 5;   // Left motor PWM
const int PhA = 7;   // Left motor direction
const int ENB = 6;   // Right motor PWM
const int PhB = 8;   // Right motor direction

// Sensors
const int R_S = A0;
const int M_S = A1;
const int L_S = A2;

// Raw sensor readings
int L, M, R;

// Digital interpreted readings
int L_d, M_d, R_d;

// Thresholds per sensor
int TH_L = 70;     // Left sensor: black = LOW
int TH_M = 300;    // Middle sensor: black = HIGH
int TH_R = 100;    // Right sensor: black = LOW

void readSensors() {
  L = analogRead(L_S);
  M = analogRead(M_S);
  R = analogRead(R_S);

  // Convert to digital black/white
  L_d = (L < TH_L) ? 1 : 0;     // Left: black when LOW
  M_d = (M > TH_M) ? 1 : 0;     // Middle: black when HIGH
  R_d = (R < TH_R) ? 1 : 0;     // Right: black when LOW
}

void setup() {
  Serial.begin(9600);

  pinMode(MOD, OUTPUT); 
  pinMode(ENA, OUTPUT);
  pinMode(PhA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(PhB, OUTPUT);

  digitalWrite(MOD, HIGH);
  delay(500);
}

void loop() {
  readSensors();

  Serial.print("L: "); Serial.print(L);
  Serial.print(" M: "); Serial.print(M);
  Serial.print(" R: "); Serial.println(R);

  // --- NORMAL LINE FOLLOWING LOGIC ---
  if (L_d == 0 && M_d == 1 && R_d == 0) {
    move_forward(250);
  }
  else if (L_d == 1 && M_d == 1 && R_d == 1) {
    move_forward(250); 
  }
  else if (L_d == 1 && M_d == 1 && R_d == 0) {
    sharp_left(140);
  }
  else if (L_d == 0 && M_d == 1 && R_d == 1) {
    sharp_right(140);
  }
  else if (L_d == 1 && M_d == 0 && R_d == 0) {
    curve_left(130);
  }
  else if (L_d == 0 && M_d == 0 && R_d == 1) {
    curve_right(130);
  }
  else if (L_d == 0 && M_d == 0 && R_d == 0) {

    // LOST LINE LOGIC

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

    // Crawl forward
    move_forward(150);
    delay(60);
    readSensors();
  }
}


// ----- MOTOR FUNCTIONS -----

void move_forward(int carSpeed) {
  analogWrite(ENA, carSpeed);
  digitalWrite(PhA, HIGH);

  analogWrite(ENB, carSpeed);
  digitalWrite(PhB, HIGH);
}

void sharp_left(int carSpeed){
  analogWrite(ENA, carSpeed * 0.5);
  digitalWrite(PhA, HIGH);

  analogWrite(ENB, carSpeed);
  digitalWrite(PhB, HIGH);
}

void sharp_right(int carSpeed){
  analogWrite(ENA, carSpeed);
  digitalWrite(PhA, HIGH);

  analogWrite(ENB, carSpeed * 0.5);
  digitalWrite(PhB, HIGH);
}

void curve_left(int carSpeed){
  analogWrite(ENA, carSpeed * 0.85);
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
  analogWrite(ENA, 0);
  digitalWrite(PhA, LOW);

  analogWrite(ENB, 0);
  digitalWrite(PhB, LOW);
}
