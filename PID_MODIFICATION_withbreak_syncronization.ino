#define pitch      5.0
#define pitch_S    5.0
#define max_vel    10.0
#define max_accel  5.0
#define ConsStep   0.5
#include <math.h>




float CurrentVelocity  = 0.0 ;

float accel_step = 0.0 ;
float accel_step_sample_1 = 0.0 ;

volatile long count = 0 ;
long count_prev = 0 ;
//////////////////////////////////////////
//for motor 1
float KP = 4.0 ;
float KI = 0.09 ;
float KD = 0.050;
/////////////////////////////////////////
//for motor 2
float KP_S = 4.0 ;
float KI_S = 0.09 ;
float KD_S = 0.050;
////////////////////////////////////////////
//for motor 1
volatile float EncoderCount = 0.0 ;
volatile long EncoderCount_prev = 0.0 ;
const float Pulses = 600.0;
//////////////////////////////////////////////
//for motor 2
volatile float EncoderCount_S = 0.0 ;
volatile long EncoderCount_prev_S = 0.0 ;
const float Pulses_S = 600.0;
///////////////////////////////////////////////
//for motor 1
const byte interruptpinA =  2 ;
const byte interruptpinB =  4 ;
///////////////////////////////////////////////
//for motor 2
const byte interruptpinA_S =  3 ;
const byte interruptpinB_S =  5 ;
//////////////////////////////////////////////
//for motor 1
const byte PWMpin =        9;
const byte DIRpin =        7 ;
//////////////////////////////////////////////
// for motor 2
const byte PWMpin_S =        10;
const byte DIRpin_S =        8 ;
///////////////////////////////////////////////
const float Vmax =              12.0 ;

////////////////////////////////////////////
//for motor 1 , 2
float X , X_S =                      0.0 ;
float X_prev , X_prev_S =           -1.0 ;
///////////////////////////////////////////

float er, er_S , er_prev, er_prev_S , inter , inter_S , inter_prev, inter_prev_S = 0.0;

long t ;
long t_prev = 0;
long t_start = 0 ;


void setup() {
  Serial.begin(9600);

  pinMode(interruptpinA , INPUT_PULLUP);
  pinMode(interruptpinB , INPUT_PULLUP);

  pinMode(interruptpinA_S , INPUT_PULLUP);
  pinMode(interruptpinB_S , INPUT_PULLUP);

  pinMode(DIRpin , OUTPUT);
  pinMode(DIRpin_S , OUTPUT);

  attachInterrupt(digitalPinToInterrupt(interruptpinA)    , ISR_interruptA , RISING);
  attachInterrupt(digitalPinToInterrupt(interruptpinA_S) , ISR_interruptA_S , RISING);

  cli();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  OCR1A = 12499;  // every 50 ms
  TCCR1B |= (1 << WGM12) ;
  TCCR1B |= (1 << CS11 | 1 << CS10) ;
  TIMSK1  |= (1 << OCIE1A) ;
  sei();


}

void loop() {

  static float X_T = 30.0;
  t = millis();

  Init();


  Move(X_T);

  X = ((float)((EncoderCount) / Pulses)) * pitch; // position = revolutions * pitch
  //Serial.println(X);
  // delay(2000);
  X = ((float)((EncoderCount) / Pulses)) * pitch; // position = revolutions * pitch
  //Serial.println(X);


}




////////////////////////////////////////////////


void Init() {


  count = 1 ;
  count_prev = 0 ;
  X_prev = -1.0 ;
  X_prev_S = -1.0 ;

  er = 0.0 ;
  er_prev  = 0.0;

  er_S = 0.0 ;
  er_prev_S  = 0.0;

  inter = 0.0 ;
  inter_prev = 0.0;

  inter_S = 0.0 ;
  inter_prev_S = 0.0;

  X = 0.0   ;
  t_prev = t;

  EncoderCount = 0;
  EncoderCount_S = 0;

  accel_step_sample_1 = 0.0 ;
  accel_step = 0.0;
}
//////////////////////////////////////////////
void Move(float X_T) {

  static float X_D = 0.0 ;

  X = (float(EncoderCount) / Pulses) * pitch; // position = revolutions * pitch
  X_S = (float(EncoderCount_S) / Pulses_S) * pitch_S; // position = revolutions * pitch
  er_S = X  - X_S ;
  while (fabs(X - X_T) >= 0.05  ||  fabs(er_S) >= 0.05 )  {
    X = ((float)((EncoderCount) / Pulses)) * pitch;
    X_S = (float(EncoderCount_S) / Pulses_S) * pitch_S; // position = revolutions * pitch
    er_S = X  - X_S ;
    while (fabs(X - X_T) >= 0.05  ||  fabs(er_S) >= 0.05  )   {
      X = ((float)((EncoderCount) / Pulses)) * pitch;
      X_S = (float(EncoderCount_S) / Pulses_S) * pitch_S; // position = revolutions * pitch
      er_S = X  - X_S ;
      while (fabs(X - X_T) >= 0.05  ||  fabs(er_S) >= 0.05) {



        if (count > count_prev) {


          X = ((float)((EncoderCount) / Pulses)) * pitch; // position = revolutions * pitch
          X_prev = X ;
          X_S = ((float)((EncoderCount_S) / Pulses_S)) * pitch_S; // position = revolutions * pitch
          X_prev_S = X_S ;

          //CurrentVelocity = ((X - X_prev) / (t - t_prev)) / 1000.0;

          if (X <= 10.0 ) {

            X_D = X + accel_step  ;
            accel_step += 0.015;
            accel_step_sample_1 = accel_step ;
          }


          else if (X > 10.0 && (X < (X_T - 10.0) )  ) {

            X_D = (X + accel_step_sample_1)  ;
            //Serial.print( X);
            // Serial.print( " ");
            /* Serial.println( count-count_sample_1);
              Serial.print( " ");*/

          }

          else if ((X >= X_T - 10.0) && (X_D <= X_T) ) {

            X_D = X + 0.3 ;
            if ( accel_step_sample_1 > 0) {
              accel_step_sample_1 -= 0.015 ;
            }

            if (accel_step_sample_1 < 0) {

              accel_step_sample_1 = 0.01;

            }
            X_D = X + accel_step_sample_1 ;

          }


          if (X >= X_T ) {
            X_D = X_T ;
          }

          // Serial.print(X_D);
          //Serial.print(" ");
          PID_Calculate(X_D);

          if (count > 10000  ) {

            break;
          }
          count_prev = count ;
        }

      }



      MotorDriver(0 , 0, Vmax);

      delay(100) ;
      MotorDriver(0 , 0, Vmax);

      if (count > 10000  ) {

        break;

      }

    }

    MotorDriver(0 , 0, Vmax);
    delay(100) ;
    MotorDriver(0 , 0, Vmax);
    if (count > 10000  ) {
      break;
    }

    if (fabs(X - X_T) <= 0.05) {
      MotorDriver(0 , 0, Vmax);
    }
  }
  MotorDriver(0 , 0, Vmax);
}

///////////////////////////////////////////////////

void PID_Calculate(float X_D) {

  t = millis();
  float dt = (t - t_prev) ;
  t_prev = t ;
  er = (X_D - X);
  er_S = X - X_S ;
  //inter = inter + (dt * er_prev) ;
  inter = inter_prev + ((float)(dt) * (er - er_prev) / 2.0);
  inter_S = inter_prev_S + ((float)(dt) * (er_S - er_prev_S) / 2.0);

  float V = KP * er + KI * inter + (KD * (er - er_prev) / dt) ;
  float V_S = KP_S * er_S + KI_S * inter_S + (KD_S * (er_S - er_prev_S) / dt) ;


  inter_prev = inter ;
  inter_prev_S = inter_S ;

  er_prev = er;
  er_prev_S = er_S;

  if (V > Vmax ) {
    V = Vmax;
    inter = inter_prev;

  }

  if (V_S > Vmax ) {
    V_S = Vmax;
    inter_S = inter_prev_S;

  }

  if (V < -Vmax) {
    V = -Vmax ;
    inter = inter_prev;
  }

  if (V_S < -Vmax) {
    V_S = -Vmax ;
    inter_S = inter_prev_S;
  }

  t_prev = t ;
  MotorDriver (V , V_S ,  Vmax);




}







/////////////////////////////////////////////////////////////////////////////////
void MotorDriver (float V , float V_S , float Vmax) {

  static int PWMval = int(255 * (fabs(V) / Vmax));
  static int PWMval_S = int(255 * (fabs(V_S) / Vmax));

  if (PWMval > 255) {
    PWMval = 255;

  }

  if (PWMval_S > 255) {
    PWMval_S = 255;

  }


  /* if (PWMval < 30 && er != 0.0 && V != 0.0) {
     PWMval = 30;

    }*/

  if (V > 0) {

    digitalWrite(DIRpin , HIGH);

  }

  else if (V < 0) {
    digitalWrite(DIRpin , LOW);

  }

  else {
    analogWrite(PWMpin , 0);

  }


  if (V_S > 0) {

    digitalWrite(DIRpin_S , HIGH);

  }
  else if (V_S < 0) {
    digitalWrite(DIRpin_S , LOW);

  }

  else {
    analogWrite(PWMpin_S , 0);

  }


  analogWrite(PWMpin , PWMval);

  analogWrite(PWMpin_S , PWMval_S);


  // Serial.print(X);
  //Serial.println(" ");

}


/////////////////////////////////////////////////////////////////////////////////////
ISR(TIMER1_COMPA_vect) {

  count++;

}



//////////////////////////////////////////////////////////////////////////////////////

void ISR_interruptA() {

  bool pinB = digitalRead(interruptpinB);
  bool pinA = digitalRead(interruptpinA);

  if (pinB == LOW) {

    EncoderCount++ ;

  }

  else if (pinB == HIGH) {
    EncoderCount--;
  }

}

void ISR_interruptA_S() {

  bool pinB_S = digitalRead(interruptpinB_S);
  bool pinA_S = digitalRead(interruptpinA_S);

  if (pinB_S == LOW) {

    EncoderCount++ ;

  }

  else if (pinB_S == HIGH) {
    EncoderCount--;
  }

}
