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
                       check_For_Lines();
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
      straight_fwd(); //Just keep swimming   
    }
    case 0b10000u: 
    {
      spin_left();
    }
    case 0b01000u:
    {
      turn_left();
    }
    case 0b00010u:
    {
      turn_right();
    }
    case 0b00001u:
    {
      spin_right();
    }
  }
}

void spin_left(void)
{
  set_motor_speed(left, rev_fast, 0); 
  set_motor_speed(right, fast, 0); 
}

void turn_left(void)
{
  set_motor_speed(left, stop, 0); 
  set_motor_speed(right, fast, 0); 
}
void straight_fwd(void)
{
  set_motor_speed(left, fast, 0); 
  set_motor_speed(right, fast, 0); 
}

void straight_backwards(void)
{
  set_motor_speed(left, rev_fast, 50); 
  set_motor_speed(right, rev_fast, 0); 
}

void spin_right(void)
{
  set_motor_speed(left, fast, 0); 
  set_motor_speed(right, rev_fast, 0); 
}
void turn_right(void)
{
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

void spin_around_one_wheel_clockwise(void)
{
  set_motor_speed(left, fast, 0); 
  set_motor_speed(right, stop, 0);
}

void spin_around_one_wheel_counterclockwise(void)
{
  set_motor_speed(left, stop, 0); 
  set_motor_speed(right, fast, 0);
}

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