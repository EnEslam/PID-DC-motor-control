#include<Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <String.h>
#include "configuration.h"
#include <PulseInput.h>


LiquidCrystal_I2C  lcd(0X20 , 20 , 4);


Keypad bad(makeKeymap(characters) , rows_pins , col_pins , ROWS , COls);

///////////////////////////////////////////////////////


char pass[10] ;                  /// variables to take the distance from keybad
int pointer = 0;
int integer = 0;

///////////////////////////////////////////////////////

unsigned char start = 1;
unsigned char selector = 0;

////////////////////////////////////////////////////////

//for ultrasonic sensor

volatile unsigned int input = 0;
long duration;
long dist;

///////////////////////////////////////////////////////

String *TOPRINT = NULL ;

unsigned char FreeState =     0;
unsigned char ProductState =  0;
unsigned char PartState =     0;
unsigned char PrintingFlag  = 1;

////////////////////////////////////////////////////////

void setup() {

  //for ultrasonic sensor
  pinMode(trig, OUTPUT);
  pinMode(echo , INPUT);
  pinMode(en , OUTPUT);
  pinMode(dir , OUTPUT);
  pinMode (puls , OUTPUT);
  attachPulseInput(echo, input);  // pin 8 as INPUT_PULLUP
  lcd.init( );
  lcd.backlight();
  HomeMenuPrinting();
  pin_config();
  set_HIGH();
  lcd.setCursor(14, 2);
  lcd.print("dist");

}

void loop() {
  //for ultraconic sensor reading distance function calling


  //////////////////////////////////////////////////////////
  if (PrintingFlag == 1) {
    HomeMenuPrinting();
    PrintingFlag = 0;
  }

  //selector application
  char pres = bad.getKey();


  if (pres) {

    switch (pres) {
      case '2':                   // case of automatic operation
        start = 1;
        FreeState =     0;
        ProductState =  1;
        PartState =     0;
        number_of_element = num_of_products ;
        lcd.clear();
        lcd.setCursor(5 , 0);
        lcd.print("SELECT PRODUCT");
        Printing_arr(ProductsElements);

        while (FreeState == 0 && ProductState == 1 && PartState == 0) {
          pres = bad.getKey();
          if (pres ) {
            switch (pres) {
              case '1':                                               //case of full high
                start = 1;
                ProductState = 2;
                number_of_element = N_FH_P ;
                lcd.clear();
                lcd.setCursor(5 , 0);
                lcd.print("SELECT PART");
                Printing_arr(FHP);

                while (FreeState == 0 && ProductState == 2 && PartState == 0) {
                  pres = bad.getKey();
                  if (pres && pres > 48 && pres <= 48 + N_FH_P) {
                    switch (pres) {
                      case '1':                                          //case of cover
                        PartState =  1;
                        lcd.clear();
                        lcd.setCursor(5 , 0);
                        lcd.print("COVER");
                        break;
                      case '2':                                          //case of body
                        PartState =  2;
                        lcd.clear();
                        lcd.setCursor(5 , 0);
                        lcd.print("BODY");
                        break;
                    }
                  }
                }
                break;

              case '2':                                               //case of tripde
                ProductState = 3;
                number_of_element = N_T_P ;
                lcd.clear();
                lcd.setCursor(5 , 0);
                lcd.print("SELECT PART");
                start = 1;
                Printing_arr(TP);

                while (FreeState == 0 && ProductState == 3 && PartState == 0) {
                  pres = bad.getKey();
                  if (pres) {
                    switch (pres) {
                      case '1':                                          //main leg
                        PartState =  1;
                        lcd.clear();
                        lcd.setCursor(5 , 0);
                        lcd.print("MAIN LEG");
                        break;
                      case '2':                                          //door leg
                        PartState =  2;
                        lcd.clear();
                        lcd.setCursor(5 , 0);
                        lcd.print("DOOR LEG");
                        break;

                      case '3':
                        PartState = 3;
                        lcd.clear();
                        lcd.setCursor(5, 0);
                        lcd.print("DOOR COVER");
                        break;

                      case '4':
                        PartState = 4;
                        lcd.clear();
                        lcd.setCursor(5, 0);
                        lcd.print("INCL BODY ");
                        break;

                      case '5':
                        PartState = 5;
                        lcd.clear();
                        lcd.setCursor(5, 0);
                        lcd.print("FIXED BODY");
                        break;

                      case '6':
                        PartState = 6;
                        lcd.clear();
                        lcd.setCursor(5, 0);
                        lcd.print("COVER");
                        break;

                      case 'B' :
                        if (start < number_of_element - 2) {
                          start++;
                          Printing_arr(TP);
                        }

                        break;

                      default :
                        break;

                      case 'A' :
                        if (start > 1) {
                          start--;
                          Printing_arr(ProductsElements);

                        }
                        break ;

                    }
                  }
                }
                break;

              case '3':                                               //case of barrier
                start = 1;
                ProductState = 4;
                number_of_element = N_B_P ;
                lcd.clear();
                lcd.setCursor(5 , 0);
                lcd.print("SELECT PART");
                Printing_arr(BP);

                while (FreeState == 0 && ProductState == 4 && PartState == 0) {
                  pres = bad.getKey();
                  if (pres) {
                    switch (pres) {
                      case '1':                                          //case of main body
                        PartState =  1;
                        lcd.clear();
                        lcd.setCursor(5 , 0);
                        lcd.print("MAIN BODY");
                        break;
                      case '2':                                          //case of large cover
                        PartState =  2;
                        lcd.clear();
                        lcd.setCursor(5 , 0);
                        lcd.print("LARGE COVER");
                        break;
                      case '3':
                        PartState =  3;
                        lcd.clear();
                        lcd.setCursor(5 , 0);
                        lcd.print("SMALL COVER");                      //case of small cover
                    }
                  }
                }


              case 'B' :
                if (start < number_of_element - 2) {
                  start++;
                  Printing_arr(ProductsElements);
                }

                break;



              case 'A' :
                if (start > 1) {
                  start--;
                  Printing_arr(ProductsElements);

                }
                break ;

              default :
                break;
            }


          }



        }

        break;

      case '1':                                                    //case free operation
        FreeState =     1;
        PrintingFlag =  1;
        while (FreeState == 1 && ProductState == 0 && PartState == 0) {
          if (PrintingFlag == 1) {
            start = 1;
            lcd.clear();
            lcd.setCursor(3 , 0);
            lcd.print("ENTER THE DIST");

            lcd.setCursor(0 , 2);
            lcd.print("DIST mm:");
            PrintingFlag = 0;
          }
          pres = bad.getKey();
          if (pres && pres >= '0' && pres <= '9' ) {
            pass[pointer] = pres;
            pointer++;
            lcd.setCursor(8 + pointer, 2);
            lcd.write(pres);
            lcd.leftToRight();

          }

          else if (pres && pres == '#' && pointer < 10 && pointer > 0)
          {
            unsigned int dist_int = chartoint();


            FreeState = 2;
            PrintingFlag = 1;
            /*lcd.setCursor(5 , 2);
              lcd.print(pulsate_the_motor( dist_int, 1));
              //Printing_arr(pulsate_the_motor( dist_int, 1));

              //pulsate_the_motor( dist_int, 1);*/

            while (FreeState == 2) {
              update_pointer();
              if (PrintingFlag == 1) {
                pointer = 0;
                number_of_element = num_of_free ;
                lcd.clear();
                lcd.setCursor(5 , 0);
                lcd.print("SELECT ANGLE");
                start = 1;
                Printing_arr(FreeElements);
                PrintingFlag = 0;
              }
              pres = bad.getKey();
              switch (pres) {

                default :
                  if (pres >= '1' && pres <= '1' + num_of_free )
                  {
                    FreeState =     3;
                    selector = pres - '0';

                    //////////////////////////////////////////////
                    lcd.clear();
                    lcd.setCursor(2 , 0);
                    lcd.print("SELECTED OPTIONS");
                    lcd.setCursor(0 , 1);
                    lcd.print("DIST mm:");
                    lcd.setCursor(8 , 1);
                    lcd.print(dist_int);
                    //////////////////////////////////////////////
                    lcd.setCursor(0 , 2);
                    lcd.print("ANGLE:");
                    lcd.setCursor(6 , 2);
                    lcd.print(FreeElements[pres - '1']);
                    ////////////////////////////////////////////
                    lcd.setCursor(2 , 3);
                    lcd.print("PRESS OK ");
                    ////////////////////////////////////////////
                    while (FreeState == 3) {
                      pres = bad.getKey();
                      switch (pres) {
                        case '#':
                          update_pointer();
                          lcd.clear();
                          lcd.setCursor(9 , 1);
                          lcd.print("OK");
                          pulsate_the_motor( dist_int, 1);
                          break;

                        case 'C':
                          FreeState = 2;
                          PrintingFlag = 1;
                          selector = 0;
                          set_HIGH();
                          break;

                        case '*':
                          FreeState = 0;
                          PrintingFlag = 1;
                          selector = 0;
                          set_HIGH();

                      }

                    }


                  }
                  break;

                case 'B' :
                  if (start < number_of_element - 2) {
                    start++;
                    Printing_arr(FreeElements);

                  }

                  break;



                case 'A' :
                  if (start > 1) {
                    start--;
                    Printing_arr(FreeElements);

                  }
                  break ;

                case 'C' :
                  FreeState = 1;
                  PrintingFlag = 1;
                  break;

                case '*' :
                  FreeState = 0;
                  PrintingFlag = 1;
                  break ;

              }
            }

          }

          else if (pres == 'C') {

            FreeState = 0;
            PrintingFlag = 1;

          }

          else if (pres == '*') {
            FreeState = 0;
            PrintingFlag  = 1;

          }



        }

        break;


      default :
        break;

        /*
          if(pres<('1'+number_of_element) && pres >= '1' && pres <start+51  ){
          lcd.clear();
          /*if(update_flag == false){
          selector=(pres-48);

          }
          pressed_show(pres);                          // to show pressed element
          }

          else if(pres== '*'){
          lcd.clear();
          set_HIGH();
          start=1;
          selector = 0;
          lcd.setCursor(3 , 0);
          lcd.print("SELECT OPTION");
          Printing_arr(FreeElements);
          //state_flag=false;
          // update_flag = false ;
          }

          else if(pres=='0' && start<number_of_element-2 && state_flag==false)
          {
          start++;
          Printing_arr(FreeElements);
          }

          else if(pres=='8' && start>1 /*&& /*state_flag==false*//*){
  start--;
  Printing_arr(FreeElements);
  }

  else if (pres == '#' /*&& update_flag== true*//*){
  lcd.setCursor(14,2);
  lcd.print("dist");
  lcd.setCursor(16,3);
  if(duration<500)
  {lcd.print(dist);}

  else
  {lcd.print("0");}

  update_pointer();

  }*/

    }


  }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////

void HomeMenuPrinting(void) {
  lcd.clear();
  lcd.setCursor(5 , 0);
  lcd.print("SELECT MODE");
  lcd.setCursor(0 , 1);
  lcd.print("1-FREE SEQ");
  lcd.setCursor(0 , 2);
  lcd.print("2-AUTO SEQ");
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////


void Printing_arr(String *ptr) {
  for (int i = start ; i <= (start + 2) && i <= number_of_element ; i++) {
    lcd.setCursor(0 , 1 + (i - start));
    lcd.print(ptr[i - 1]);
  }

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////


void pin_config() {
  for (int j = 0 ; j < 4 ; j++) {
    pinMode(trigger[j] , OUTPUT);
  }
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void update_pointer() {
  if ( selector != 0 && selector != 4) {
    digitalWrite(trigger[selector - 1] , LOW);
    for (int p = 0 ; p < 4 ; p++) {
      if (p != (selector - 1))
      { digitalWrite(trigger[p] , HIGH);

      }

    }
    digitalWrite(trigger[3] , LOW);
  }


}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/* void  pressed_show(char p){

  if (state_flag==false){
        lcd.setCursor(2 , 0);
        lcd.print("SELECTED OPTION");

        lcd.setCursor(0 , 2);
        lcd.print(stringArray[p-49]);

        lcd.setCursor(0 , 3);
        //lcd.print(selector);
        state_flag = true;
        //update_flag = true;
   }
   else{
    lcd.setCursor(0 , 2);
    lcd.print("please enter * ");
    set_HIGH();
     //update_flag = false;
    }

  }*/


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void set_HIGH() {
  for (int set = 0 ; set < 4 ; set++) {

    digitalWrite(trigger[set] , HIGH);

  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void ReadDis() {

  digitalWrite(trig , LOW);
  delayMicroseconds(2);
  digitalWrite(trig , HIGH);
  delayMicroseconds(15);
  digitalWrite(trig , LOW);
  //read the duration
  duration = input;
  dist = duration * (0.034 / 2) * 10;
  lcd.setCursor(14, 2);
  lcd.print("dist");
  delay(1);
  lcd.setCursor(16, 3);
  if (duration < 500)
    lcd.print(dist);

  else
    lcd.print("0");

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

unsigned long int chartoint (void) {


  unsigned long int  result = 0;
  if (pointer > 0) {

    for (int count = 0 ; count < pointer ; count ++ ) {

      // Serial.println(buff[count]);
      unsigned long int var = power(10 , (pointer - count - 1));
      //Serial.println(var);
      result = result + ((pass[count] - '0') * (var ));

    }

  }
  return result;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

unsigned long  int  power(int base , int po) {
  unsigned long int   ret =  (unsigned long int )(base);
  if (base > 0 && po > 1) {
    for (int st = 0 ; st < (po - 1); st++) {
      ret = ret * base;
    }
    return ret;
  }

  else if (po == 1) {
    return ret;
  }

  else if (po == 0)
  {
    return 1;
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int pulsate_the_motor(unsigned long int distance , int   motion_direction) {

  unsigned long int  num = (distance * pulses) / pitch ;
  //int num = (x/360);

  digitalWrite(en , LOW);

  // control_direction
  if (motion_direction == 1) {
    digitalWrite(dir , LOW);
  }
  else if (motion_direction == 2) {

    digitalWrite(dir , HIGH);
  }

  return num ;

  if (num > 0)
  {
    // pulse the stepper
    for (unsigned int  i = 0 ; i < num ; i++) {

      digitalWrite(puls, HIGH);
      delay(2);
      digitalWrite(puls, LOW);
      delay(2);
    }
  }
}
