#include <xc.h>
#include "/Users/gabriellechner-ling/Documents/School/SPRING 2016/APSC 1299 PROJECTS/Common/sumovore.h"
#include "/Users/gabriellechner-ling/Documents/School/SPRING 2016/APSC 1299 PROJECTS/Common/motor_control.h"
//hello
void follow_simple_curves(void);
void spin_left(void);
void turn_left(void);
void straight_fwd(void);
void straight_backwards(void);
void turn_right(void);
void spin_right(void);

void spin_circle_clockwise(void);
void spin_circle_counterclockwise(void);
void spin_around_one_wheel_clockwise(void);
void spin_in_one_place_clockwise (void);
void spin_around_one_wheel_counterclockwise(void);
void spin_in_one_place_counterclockwise (void);
void testFunction(void);
void delay_3_seconds(void);
void motor_control(void)
{
     // very simple motor control
     /*switch(SeeLine.B)
     {
        case 0b00100u:
        case 0b00010u:
        case 0b01000u:
        case 0b00001u:
        case 0b10000u:
                       //no breaks all above readings end up here
                       follow_simple_curves();
                       break;
        case 0b00000u:
                     //  motors_brake_all();
            testFunction();
                       break;
        default:       break;
      } */
   delay_3_seconds();
   spin_circle_clockwise();
   delay_3_seconds();
   spin_around_one_wheel_clockwise();
   delay_3_seconds();
   spin_in_one_place_clockwise ();
   delay_3_seconds();
   
}

void follow_simple_curves(void)
{
    if ( SeeLine.b.Center ) straight_fwd();
    else if (SeeLine.b.Left) spin_left();
    else if (SeeLine.b.CntLeft) turn_left();
    else if (SeeLine.b.CntRight) turn_right();
    else if (SeeLine.b.Right) spin_right();
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
  
  delay_3_seconds();
  
  set_motor_speed(left, medium, 0); 
  set_motor_speed(right, medium, 0); 
  
  delay_3_seconds();
  
  set_motor_speed(left, slow, 0); 
  set_motor_speed(right, slow, 0); 
  
  delay_3_seconds();
  
  set_motor_speed(left, stop, 0); 
  set_motor_speed(right, stop, 0); 
  
  delay_3_seconds();
  
  set_motor_speed(left, rev_slow, 0); 
  set_motor_speed(right, rev_slow, 0); 
  
  delay_3_seconds();
  
  set_motor_speed(left, rev_medium, 0); 
  set_motor_speed(right, rev_medium, 0); 
  
  delay_3_seconds();
  
  set_motor_speed(left, rev_fast, 0); 
  set_motor_speed(right, rev_fast, 0); 
  
    delay_3_seconds();
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


void delay_3_seconds(void)
{
    for(int i =0;i<150;i++)
    _delay(179200);
}    
