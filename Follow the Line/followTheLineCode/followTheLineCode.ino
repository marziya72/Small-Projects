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

const int carSpeed = 190;  // Forward speed - declared normally, as we may want to modify speed. 0 - 255.
int memory;


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

  //Delay in milliseconds
  delay(500);
}

void loop() {

  // Read sensors
  L = analogRead(L_S);
  M = analogRead(M_S);
  R = analogRead(R_S);

  // Print values (optional but useful) - this will display on the serial monitor
  Serial.print("L: "); Serial.print(L);
  Serial.print("  M: "); Serial.print(M);
  Serial.print("  R: "); Serial.println(R);

  /*
    - Condition: If L_S is false (hence 0), M_S is true (it detects something) and R_S is false (0) then it will move forward.
    - We use the digitalRead function to READ the value at each sensor - it basically asks each sensor "Do you detect anything? (Yes/No)"
    - If Yes then it would be == 1 otherwise if No then it would be == 0. Then we just call the function inside of the conditional statement
      to be executed.
  */

  if ((digitalRead(L_S)) == 0 && (digitalRead(M_S)) == 1 && (digitalRead(R_S)) == 0) {
    move_forward();
    memory = 0;
  } else if ((digitalRead(L_S)) == 1 && (digitalRead(M_S)) == 1 && (digitalRead(R_S)) == 0){
    sharp_left();
    memory = 1;
  } else if ((digitalRead(L_S)) == 0 && (digitalRead(M_S)) == 1 && (digitalRead(R_S)) == 1){
    sharp_right();
    memory = 2;
  } else if ((digitalRead(L_S)) == 1 && (digitalRead(M_S)) == 1 && (digitalRead(R_S)) == 1){
    move_forward();
    memory = 3;
  } else if ((digitalRead(L_S)) == 1 && (digitalRead(M_S)) == 0 && (digitalRead(R_S)) == 0){
    curve_left();
    memory = 4;
  } else if ((digitalRead(L_S)) == 0 && (digitalRead(M_S)) == 0 && (digitalRead(R_S)) == 1){
    curve_right();
    memory = 5;
  }
  else {
    stop_car(); // We use the else statement to finish the condition so that it tells the robot what to do if the condition above is not satisfied.
  }
}

void move_forward() {
  // LEFT motor
  analogWrite(ENA, carSpeed); // ENA controls the speed of the left motors - we use PWM to assign the amount of speed we want to apply to it. 
  digitalWrite(PhA, HIGH); // PhA controls the actual direction of the left motor - HIGH means Forward. 

  // RIGHT motor
  analogWrite(ENB, carSpeed); // ENB controls the speed of the right motors - we use PWM to assign the amount of speed we want to apply to it. 
  digitalWrite(PhB, HIGH); // PhB controls the actual direction of the right motors - HIGH means Forward. 
}
void sharp_left(){
  //LEFT motor
  analogWrite(ENA, carSpeed);
  digitalWrite(PhA, LOW);
  //RIGHT motor
  analogWrite(ENB, carSpeed);
  digitalWrite(PhB, HIGH);
 
}
void sharp_right(){
  //LEFT motor
  analogWrite(ENA, carSpeed);
  digitalWrite(PhA, HIGH);
  //RIGHT motor
  analogWrite(ENB, carSpeed);
  digitalWrite(PhB, LOW);

}
void curve_left(){
  //LEFT MOTOR
  analogWrite(ENA, carSpeed);
  digitalWrite(PhA, HIGH);
  //RIGHT MOTOR
  analogWrite(ENB, carSpeed/2);
  digitalWrite(PhB, HIGH);
 
}
void curve_right(){
  //LEFT MOTOR
  analogWrite(ENA, carSpeed);
  digitalWrite(PhA, HIGH);
  //RIGHT MOTOR
  analogWrite(ENB, carSpeed/2);
  digitalWrite(PhB, HIGH);
  memory = M
}
void stop_car() {
  analogWrite(ENA, 0); // We set the carSpeed for left motors to 0 - this will simply depower the left motors. 
  digitalWrite(PhA, LOW); // Left Motor Direction has been set to LOW - LOW means Backward.
  analogWrite(ENB, 0); // We set the carSpeed for right motors to 0 - this will simply depower the right motors. 
  digitalWrite(PhB, LOW); // Right Motor Direction has been set to LOW - LOW means backward. 
  if memory == 0 {
    
  }

  }
}