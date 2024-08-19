#include <String.h>

void pin_config(void);
void Printing_arr(String *ptr);
void update_pointer(void);
void pressed_show(char );
void set_HIGH(void);
void ReadDis(void);
void HomeMenuPrinting(void);
unsigned long int  power(int base , int po);
unsigned long int chartoint (void);
int pulsate_the_motor(unsigned long int dist , int  motion_direction);

//define number of steps to complete one revolution
#define    steps    200

//define control pins
#define    en        0
#define    dir       A2
#define    puls      A3



//to add a new option please add a string element and change the number of array element
#define  num_of_free        4                 //num_of_angles_list
#define  num_of_products    4                 //num_of_products_elements
#define  N_FH_P             2                 //num_of_full high parts
#define  N_T_P              6                 //number of tripode parts
#define  N_B_P              3                 //number of barrier gate parts
#define  N_HB_P             3                 //number of hydraullic barrier gate parts
#define  RIGHT              1                 //right direction of stepper motors 
#define  LEFT               2                 //left direction of the stepper motor 

/////////////////////////////////////////////////////////////////////////////////////////////////

#define   pulses           200               //number of pulses for 1 rev
#define   pitch            5                 //pitch of power screw in mm 

String FreeElements[num_of_free] =         { "1-304 1.5 135",
                                             "2-304 1.5 120",
                                             "3-304 1.5 150",
                                             "4-NO LIMMIT  "
                                           };



String FHP[N_FH_P] =                        {"1-COVER      ",
                                             "2-BODY       "
                                            };

String ProductsElements[num_of_products] = {"1-FULL HIGH   ",
                                            "2-TRIPODE     ",
                                            "3-BARRIER     ",
                                            "4-HYDRAULLIC B"
                                           };

String TP[N_T_P] =                            {"1-MAIN LEG    ",
                                               "2-DOOR LEG    ",
                                               "3-DOOR COVER  ",
                                               "4-INCL BODY   ",
                                               "5-FIXED BODY  ",
                                               "6-COVER       "
                                              };

String BP[N_B_P] =                            {"1-MAIN BODY   ",
                                               "2-LARGE COVER ",
                                               "3-SMALL COVER "
                                              };

String BHP[N_HB_P] =                          {"1-MAIN BODY   ",
                                               "2-LARGE COVER ",
                                               "3-SMALL COVER "
                                              };




unsigned char  number_of_element = 0 ;

// array of triggering pins
unsigned char trigger[num_of_free] = { 13, 12 , 11, 10};

const byte ROWS  = 4;
const byte COls  = 4;

const int trig = A0;
const int echo = 2;

char characters[ROWS][COls] = {
  {'1' , '2' , '3' , 'A'},
  {'4' , '5' , '6' , 'B'},
  {'7' , '8' , '9' , 'C'},
  {'*' , '0' , '#' , 'D'}
};

byte rows_pins[ROWS] = {3, 4, 5 , 6};
byte col_pins[COls] = {7 , 8 , 9 , A1};
