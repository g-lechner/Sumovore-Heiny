#include <xc.h>
#include "sumovore.h"
#include "motor_control.h"

void follow_complex_curves(void);
void spin_left(void);
void turn_left(void);
void straight_fwd(void);
void straight_backwards(void);
void turn_right(void);
void spin_right(void);

void turn_around(void);

void spin_circle_clockwise(void);
void spin_circle_counterclockwise(void);
void spin_around_one_wheel_clockwise(void);
void spin_in_one_place_clockwise (void);
void spin_around_one_wheel_counterclockwise(void);
void spin_in_one_place_counterclockwise (void);

void check_For_Lines(void);

void testFunction(void);
void delay_x_seconds(int time); //time = 80 per second

signed char motor_speed_variable[] = {0,0,0,0}; //set 0 for stopped, 1 for slow, 2 for med, 3 for fast, -1 for rev slow, etc. this is left motor first, then right  
//also, last two elements are for int modifier, to make this array more unique.
//the purpose of this is so that our motors are not being set to a speed constantly, allowing it to run faster.

void motor_control(void)
{
     // very simple motor control
     switch(SeeLine.B)
     {
        case 0b00100u:
        case 0b00010u:
        case 0b01000u:
        case 0b00001u:
        case 0b10000u:
                       //no breaks all above readings end up here
                       follow_complex_curves();
                       break;
        case 0b00000u:
                       motors_brake_all();
                       break;
        default:       break;
      } 
}

void follow_complex_curves(void)
{
  switch(SeeLine.B)
  {
    case 0b00100u:
    {
      if (!motor_speed_variable[3,3,0,0])
      {
        straight_fwd(); //Just keep swimming   
      }
      break;
    }
    case 0b10000u: 
    {
       if (!motor_speed_variable[-3,3,0,0])
       {
         spin_left();
       }
       break;
    }
    case 0b01000u:
    {
       if (!motor_speed_variable[0,3,0,0])
       {
         turn_left();
       }
       break;
    }
    case 0b00010u:
    {
       if (!motor_speed_variable[3,0,0,0])
       {
         turn_right();
       }
       break;
    }
    case 0b00001u:
    {
      if (!motor_speed_variable[3,-3,0,0])
      {
        spin_right();
      }
      break;
    }
    /*
    case 0b00111u: //for sharp right turns, will turn right for some amount, then reorient itself
    {
       turn_right(); 
       delay_x_seconds(10);
       break;
    }
    case 0b11100u:
    {
        turn_left();
        delay_x_seconds(10);//see above case but for sharp lefts
        break;
    }
    case 0b00101u: //for reverse turns to the right
    {
        turn_right();
        delay_x_seconds(30);
        break;
    }
    case 0b10100u: //for reverse turns to the left
    {
        turn_left();
        delay_x_second(30);
        break;
    }
    */
  }
}

void spin_left(void)
{
    motor_speed_variable[-3,3,0,0];
    set_motor_speed(left, rev_fast, 0); 
    set_motor_speed(right, fast, 0); 
}

void turn_left(void)
{
    motor_speed_variable[0,3,0,0];
    set_motor_speed(left, stop, 0); 
    set_motor_speed(right, fast, 0); 
}
void straight_fwd(void)
{
    motor_speed_variable[3,3,0,0];
    set_motor_speed(left, fast, 0); 
    set_motor_speed(right, fast, 0); 
}

//Currently do not need
/*
void straight_backwards(void)
{
  if (!motor_speed_variable[-3,-3,50,0])
  {
    motor_speed_variable[-3,-3,50,0];
    set_motor_speed(left, rev_fast, 50); 
    set_motor_speed(right, rev_fast, 0);
  }
}
 */

void spin_right(void)
{
    motor_speed_variable[3,-3,0,0];
    set_motor_speed(left, fast, 0); 
    set_motor_speed(right, rev_fast, 0);
}
void turn_right(void)
{
    motor_speed_variable[3,0,0,0];
    set_motor_speed(left, fast, 0); 
    set_motor_speed(right, stop, 0); 
}
void testFunction(void)
{
  set_motor_speed(left, fast, 0); 
  set_motor_speed(right, fast, 0); 
  
  delay_x_seconds(240);
  
  set_motor_speed(left, medium, 0); 
  set_motor_speed(right, medium, 0); 
  
  delay_x_seconds(240);
  
  set_motor_speed(left, slow, 0); 
  set_motor_speed(right, slow, 0); 
  
  delay_x_seconds(240);
  
  set_motor_speed(left, stop, 0); 
  set_motor_speed(right, stop, 0); 
  
  delay_x_seconds(240);
  
  set_motor_speed(left, rev_slow, 0); 
  set_motor_speed(right, rev_slow, 0); 
  
  delay_x_seconds(240);
  
  set_motor_speed(left, rev_medium, 0); 
  set_motor_speed(right, rev_medium, 0); 
  
  delay_x_seconds(240);
  
  set_motor_speed(left, rev_fast, 0); 
  set_motor_speed(right, rev_fast, 0); 
  
  delay_x_seconds(240);
}

//This function is essentially a clone of turn_right!
/* 
void spin_around_one_wheel_clockwise(void)
{
  if (!motor_speed_variable[3,0,0,0])
  {
    motor_speed_variable[3,0,0,0]
    set_motor_speed(left, fast, 0); 
    set_motor_speed(right, stop, 0);
  }
}
 */

//Clone of turn_left
/*
void spin_around_one_wheel_counterclockwise(void)
{
  set_motor_speed(left, stop, 0); 
  set_motor_speed(right, fast, 0);
}
 */

void spin_in_one_place_clockwise (void)
{
  set_motor_speed(left, fast, 0); 
  set_motor_speed(right, rev_fast, 0); 
}

void spin_in_one_place_counterclockwise (void)
{
  set_motor_speed(left, rev_fast, 0); 
  set_motor_speed(right, fast, 0); 
}

void spin_circle_clockwise(void)
{
    set_motor_speed(left, fast, 0);      //To spin clockwise left must be
    set_motor_speed(right, medium, 0);   //Faster than right
}

void spin_circle_counterclockwise(void)
{
    set_motor_speed(left, medium, 0);
    set_motor_speed(right, fast, 0);
}

void check_For_Lines(void)
{
    OpenTimer0(T0_SOURCE_INT & TIMER_INT_OFF & T0_16BIT & T0_PS_1_256);
    TMR0IF = 0;
    WriteTimer0(0u);
    
    //If it sees a line, that's fine; if it doesn't, spin 180 and come 
    while(!TMR0IF)
    {
        check_sensors();
        if(SeeLine.B)
        {
            follow_complex_curves();
        }
    }
    
    turn_around();
    straight_fwd();
}

void turn_around(void)
{
    spin_in_one_place_clockwise();
    delay_x_seconds(10);  //sample time, will need to test to optimize for our bot   
}

void delay_x_seconds(int time) //time = 80 for one second @ 32MHz
{
    for(int i =0;i<time;i++)
    _delay(100000);
}    