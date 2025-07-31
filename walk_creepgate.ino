#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#define trigPin 8
#define echoPin 9

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERVOMIN 230  //0 deg 150:600
#define SERVOMAX 520  //180 deg -> 375 = 90 deg, 263 = 45 deg
#define N 250  //2000
#define M 1000

#define UP 105
#define DOWN 45

float Duration = 0;
float Distance = 0;

// サーボ番号（例：偶数=hip、奇数=knee）
int FL_HIP = 0, FL_KNEE = 1;
int FR_HIP = 2, FR_KNEE = 3;
int BL_HIP = 4, BL_KNEE = 5;
int BR_HIP = 6, BR_KNEE = 7;

int count = 0;

void servo_write(int n, int ang) {
  ang = constrain(ang, 0, 180);
  int pulse = map(ang, 0, 180, SERVOMIN, SERVOMAX);
  pwm.setPWM(n, 0, pulse);
}

void setup() {
  Serial.begin(9600);
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
  pwm.begin();
  pwm.setPWMFreq(50);

  for (int i = 0; i < 8; i += 2) {
    servo_write(i, 90);  // 初期中央
  }
  for (int i = 1; i < 8; i += 2) {
    servo_write(i, DOWN);
  }
  delay(N);  
  servo_write(2, 135);
  servo_write(6, 135);
  delay(N);
}
/**/


void measure_distance() {
  digitalWrite(trigPin, LOW); delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  Duration = pulseIn(echoPin, HIGH) / 2;
  Distance = Duration * 340 * 100 / 1000000;

  Serial.println(String("距離: ") + Distance + " cm");
}

// 足を前に出す（ステップ動作）
void step_R(int hip, int knee) {
  servo_write(knee, UP);       // 持ち上げる
  delay(N);
  servo_write(hip, 45);        // 前に出す（+90°）
  
  delay(N);
  servo_write(knee, DOWN);        // 下ろす
  delay(N);
}


void step_L(int hip, int knee) {
  servo_write(knee, UP);       // 持ち上げる
  delay(N);
  servo_write(hip, 135);        // 前に出す（+90°）
  delay(N);
  servo_write(knee, DOWN);        // 下ろす
  delay(N);
}



void body_shift_forward_1() {
  servo_write(FR_HIP, 90);
  servo_write(BR_HIP, 90);
  servo_write(FL_HIP, 45);
  servo_write(BL_HIP, 45);
  delay(N);
}

void body_shift_forward_2() {
  servo_write(FR_HIP, 135);
  servo_write(BR_HIP, 135);
  servo_write(FL_HIP, 90);
  servo_write(BL_HIP, 90);
  delay(N);
}


void creep_cycle() {
  step_R(BR_HIP, BR_KNEE);
  step_R(FR_HIP, FR_KNEE);
  body_shift_forward_1();
  step_L(BL_HIP, BL_KNEE);
  step_L(FL_HIP, FL_KNEE);
  body_shift_forward_2();
}

void turn_right_90_degrees(){
  // サーボ番号（例：偶数=hip、奇数=knee）
  FL_HIP = 2, FL_KNEE = 3;
  FR_HIP = 6, FR_KNEE = 7;
  BL_HIP = 0, BL_KNEE = 1;
  BR_HIP = 4, BR_KNEE = 5;

  servo_write(FL_HIP, 90);   
  delay(N);
  //BR
  servo_write(BR_KNEE, UP);       // 持ち上げる
  delay(N);
  servo_write(BR_HIP, 135);        // 前に出す（+45°）
  delay(N);
  servo_write(BR_KNEE, DOWN);        // 下ろす
  delay(N);

}

void reset(){
  servo_write(FL_HIP, 135); 
  delay(N);
  //BL
  servo_write(BR_KNEE, UP);       // 持ち上げる
  delay(N);
  servo_write(BR_HIP, 90);        // 前に出す（+45°）
  delay(N);
  servo_write(BR_KNEE, DOWN);        // 下ろす
  delay(N);

  FL_HIP = 0, FL_KNEE = 1;
  FR_HIP = 2, FR_KNEE = 3;
  BL_HIP = 4, BL_KNEE = 5;
  BR_HIP = 6, BR_KNEE = 7;
}


void loop() {
  measure_distance();
  if (Distance >= 30) {
    creep_cycle();
    if (count == 1){
      for(int i = 0; i < 2; i++ ){
        creep_cycle();
      }
      reset();
      count = 0;
    }
  } else {
    turn_right_90_degrees();
    Serial.println("座標入れ替え");
    delay(N);
    count = 1;
    measure_distance();
    while(Distance <= 30){
      creep_cycle();
      measure_distance();
    }
  } 
 
}
