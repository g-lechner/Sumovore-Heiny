/*
Final Version of Line Following Robot Code
This was the Code used for running on the track

 * 
*/




#include <xc.h>
#include "sumovore.h"
#include "motor_control.h"

void follow_complex_curves(void);
void spin_left(void);
void turn_left(void);
void curve_left(void);
void merge_left(void);

void test150(void);

void straight_fwd(void);
void straight_backwards(void);

void curve_right(void);
void turn_right(void);
void spin_right(void);
void slow_fwd(void);
void merge_right(void);

void spin_150_counterclockwise(void);
void spin_150_clockwise(void);

void turn_around(void);

char check_For_Lines(void);

void test_straight(void);
void testFunction(void);
void delay_x_seconds(int time); //time = 80 per second

char line_seen;
char parallel_lines = 0;

signed int motor_speed_variable[] = {0,0,0,0}; //set 0 for stopped, 1 for slow, 2 for med, 3 for fast, -1 for rev slow, etc. this is left motor first, then right  
//also, last two elements are for int modifier, to make this array more unique.
//the purpose of this is so that our motors are not being set to a speed constantly, allowing it to run faster.

 

void motor_control(void)
{
    OpenTimer0(T0_SOURCE_INT & TIMER_INT_OFF & T0_16BIT & T0_PS_1_256);
     // very simple motor control
     switch(SeeLine.B)
     {
        case 0b00100u: //Straight
        case 0b00010u: //Curve right
        case 0b01000u: // Curve left
        case 0b01100u: //Curve left
        case 0b00110u: //Curve Right
        case 0b00001u: //Curve sharp right
        case 0b10000u://Curve sharp left 
        case 0b00111u: // 90 degrees Turn Right
        case 0b11100u:// 90 degrees Turn Left
        case 0b11111u://Block/Crossroads
        case 0b11110u://Finetune
        case 0b01111u://Finetune
        case 0b11000u://Finetune sharp left curve
        case 0b00011u://Finetune sharp right curve
        case 0b10100u://sharp angle turn left
        case 0b00101u://Sharp angle turn right
             
                       //no breaks all above readings end up here
                       follow_complex_curves();
                       break;
        case 0b00000u:
        {
            
            
            
            TMR0IF = 0;
            WriteTimer0(48911u); //(65536-49911)*256 = 4000000 TCY = .5s ~ 15cm
            straight_fwd();
            while(!TMR0IF && !SeeLine.B)
            {
                check_sensors();
                set_leds();
                                 
            }     
            if (SeeLine.B)
            {
                if (SeeLine.B == 0b10000)
                {
                    merge_right();
                        while(!SeeLine.b.Center)
                        {
                        check_sensors();
                        set_leds(); 
                        }
                }
                
                
                else if (SeeLine.B == 0b00001)
                {
                    merge_left(); 
                    while(!SeeLine.b.Center)
                    {
                    check_sensors();
                    set_leds(); 
                    }
                }
                
                else if (SeeLine.B == 0b00011)
                {
                spin_right();
                    while(!SeeLine.b.Center)
                    {
                    check_sensors();
                    set_leds(); 
                    }
                    
                }
                    
                else if (SeeLine.B == 0b11000)
                {
                spin_left();
                    while(!SeeLine.b.Center)
                    {
                    check_sensors();
                    set_leds(); 
                    }
                      
                }    
                    
                break;
            }
            
            
            else if (TMR0IF)
            {
            turn_around();  
            break;
            }
        }                
    }          
                    
}

void follow_complex_curves(void)
{
  switch(SeeLine.B)
  {
    
    
    case 0b00111u:
    {
       _delay(3000); //So the robot goes past the turn before actually turning
        spin_right();
        while(!SeeLine.b.Center)
        {
            check_sensors();
            set_leds(); 
        }
        break;  
    }
    
    
    case 0b11100u:
    {
        
        _delay(3000); //So the robot goes past the turn before actually turning
        spin_left();
        while(!SeeLine.b.Center)
        {
            check_sensors();
            set_leds(); 
        }
        
        break;  
    }

    case 0b01111u:
    {
      spin_right(); 
      break;
    }
    //This too?
    case 0b11110u:
    {
      spin_left();     
      break;
    }
    
    case 0b11000u:
    {
      //if (SeeLine.B == 0b11100 ||SeeLine.B == 0b11000 ||SeeLine.B ==0b10000 )
      //{
        
        turn_left(); //Just keep swimming 
        while(!SeeLine.b.Center)
        {
            check_sensors();
            set_leds(); 
       // }
      } 
        
      break;
    }
    case 0b00011u:
    {
      //if (SeeLine.B == 0b00111 ||SeeLine.B == 0b00011 ||SeeLine.B == 0b00001 )
      //{
        turn_right(); //Just keep swimming 
        while(!SeeLine.b.Center)
        {
            check_sensors();
            set_leds(); 
        }
      //} 
      break;
    }
    
      case 0b00100u:
      {
          straight_fwd();    
          break;
      }
    case 0b10000u: 
    {
         if(!SeeLine.b.Center)
      {
       if (!motor_speed_variable[-2,2,0,0])
       {
         spin_left();
       }
       break;
      }
    }
    case 0b01000u:
    case 0b01100u:
    {
       if (!motor_speed_variable[1,2,0,0])
       {
         turn_left();
       }
       break;
    }
    case 0b00110u:
    case 0b00010u:
    {
       if (!motor_speed_variable[2,1,0,0])
       {
         turn_right();
       }
       break;
    }
    case 0b00001u:
    {
      if(!SeeLine.b.Center)
      {
        if (!motor_speed_variable[2,-2,0,0])
          spin_right(); 
      }
      break;
    }
    
    
    case 0b00101u: //for reverse turns to the right
   {
        TMR0IF=0;
        WriteTimer0(0u);
        while( ReadTimer0() < 14286)
        {
            check_sensors();
            set_leds();
            
            if(SeeLine.B == 0b00101u)
            {
               while(SeeLine.B == 0b00101)
               {
                   check_sensors();
                   set_leds();
               }
               delay_x_seconds(5);
            }
            
            check_sensors();
            set_leds();
            
            if(SeeLine.B != 0b00101u)
            {
   
            if(SeeLine.B == 0b00000)
            {
               spin_right();
               while(SeeLine.B != 0b00100)
               {
                   check_sensors();
                   set_leds();
               }
            
               break;
              
            }
            }
        }
        break;
    }
    case 0b10100u: //for reverse turns to the left
    {
        TMR0IF=0;
        WriteTimer0(0u);
        while( ReadTimer0() < 14286)
        {
            check_sensors();
            set_leds();
            
            if(SeeLine.B == 0b10100u)
            {
               while(SeeLine.B == 0b10100u)
               {
                   check_sensors();
                   set_leds();
               } 
               delay_x_seconds(5);
            }
            
            check_sensors();
            set_leds();
            
            if(SeeLine.B != 0b10100u)
            {
   
            if(SeeLine.B == 0b00000)
            {
               spin_left();
               while(SeeLine.B != 0b00100)
               {
                   check_sensors();
                   set_leds();
               }
            
               break;
              
            }
            }
        }
        break;
    }
    case 0b11111u:
    {
    TMR0IF = 0;
    WriteTimer0(57724u); //(65536-57724)*256 = .25s

    while(!TMR0IF) // To continue forward until timer runs out
        {
        straight_fwd();
        }
        check_sensors();
        set_leds();
    if (SeeLine.B == 0b11111) // if still 0b11111 after timer runs out, stop
    {
        motors_brake_all();
    while(1);
    }
    break;
    }
  }
}



void spin_left(void) //[-2,2,0,0]
{
    motor_speed_variable[-1,1,0,0];
    set_motor_speed(left, rev_slow, 0);//slow 
    set_motor_speed(right, slow, 0); //slow
}

void turn_left(void) //[0,3,0,0]
{
    motor_speed_variable[-1,3,0,0];
    set_motor_speed(left, rev_slow, 0); 
    set_motor_speed(right, fast, 0); 
}

void straight_fwd(void) //[2,2,0,30]
{
    motor_speed_variable[2,2,0,50];
    set_motor_speed(left, slow, 0); //slow for both
    set_motor_speed(right, slow, 0); 
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

void spin_right(void) //[2,-2,0,0]
{
    motor_speed_variable[1,-1,0,0];
    set_motor_speed(left, slow, 0); //slow
    set_motor_speed(right, rev_slow, 0);//slow
}

void turn_right(void) //[3,0,0,0]
{
     motor_speed_variable[3,-1,0,0];
    set_motor_speed(left, fast, 0); 
    set_motor_speed(right, rev_slow, 0);    
}

void merge_right(void) //[2,1,0,0]
{
    motor_speed_variable[2,1,0,0];
    set_motor_speed(left, medium, 0); 
    set_motor_speed(right, slow, 0); 
}


void merge_left(void) //[1,2,0,0]
{
     motor_speed_variable[1,2,0,0]; 
    set_motor_speed(left, slow, 0);
    set_motor_speed(right, medium, 20);
}

void curve_right(void) //[2,1,0,0]
{
    motor_speed_variable[3,2,0,0];
    set_motor_speed(left, fast, 0); 
    set_motor_speed(right, medium, 0); 
}

void curve_left(void) //[2,1,0,0]
{
    motor_speed_variable[2,3,0,0];
    set_motor_speed(left, medium, 0); 
    set_motor_speed(right, fast, 0); 
}

char check_For_Lines(void)
{

    //If it sees a line, that's fine; if it doesn't, spin 180 and come 
        check_sensors();
        set_leds();
        if(SeeLine.b.Center)
        {
            return 1;
        }
    return 0;
     
    //turn_around();
    //straight_fwd();
}

void slow_fwd(void)
{
  motor_speed_variable[1,1,0,60];
    set_motor_speed(left, slow, 0); 
    set_motor_speed(right, slow, 60);    
}


void turn_around(void)
{
    spin_right();
    delay_x_seconds(63); 
    straight_fwd();
}

void test_straight(void)
{
    straight_fwd();
    delay_x_seconds(80);
}

void testFunction(void)
{
    straight_fwd();
  
  delay_x_seconds(240);
  
  slow_fwd();
  
  delay_x_seconds(240);
  
  curve_right(); 
  
  delay_x_seconds(240);
  
  curve_left();
  
  delay_x_seconds(240);
  
  straight_fwd();
  
  delay_x_seconds(240);
  
  turn_around();
  
}

void test150(void)
{
    straight_fwd();
  
  delay_x_seconds(240);
    
  spin_150_clockwise();
  
    delay_x_seconds(240);
    
    spin_150_counterclockwise();
    
    

    
}

void delay_x_seconds(int time) //time = 80 for one second @ 32MHz
{
    for(int i =0;i<time;i++)
    _delay(100000);
}    

