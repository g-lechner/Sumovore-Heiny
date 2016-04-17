/*
 * Version X - Final Commented/detailed and organized file. Removed all excess outdated functions/variables
 * 
 * Nizar El Hoss
 * Gabriel Lechner-Ling
 * Jake Osborne
 * 
 * March-April 2016
 * 
 * Revisions of code up to and including this version can be found at:
 * https://github.com/g-lechner/Sumovore-Heiny
 */

#include <xc.h>
#include "sumovore.h"
#include "motor_control.h"

/* Prototypes */

//The main loop of the program deciding what the bot should do.
void follow_complex_curves(void);

//Prototypes for our movement functions

void spin_left(void);
void turn_left(void);

void spin_right(void);
void turn_right(void);

void straight_fwd(void);

//Prototypes for functions dealing with gaps
void merge_left(void);
void merge_right(void);
void turn_around(void);

void delay_x_seconds(int time); //time = 80 per second to allow delays of larger amounts (mostly for testing purposes)

/* Global Variables*/

signed int motor_speed_variable[] = {0,0,0,0}; //set 0 for stopped, 1 for slow, 2 for med, 3 for fast, -1 for rev slow, etc. this is left motor first, then right  
//also, last two elements are for int modifier, to make this array more unique. This is why the variable is an int; chars wouldn't be able to handle the modifiers.
//the purpose of this is so that our motors are not being set to a speed constantly, allowing it to run faster.

 

//Essentially the switchboard of the program: Go into our main decision making function if our bot sees anything that we coded for 
//otherwise do a different set of checks to determine if it should turn around or merge.
//Default case to possibly tell us if our bot is seeing something that we haven't accounted for in the code.
void motor_control(void)
{
    OpenTimer0(T0_SOURCE_INT & TIMER_INT_OFF & T0_16BIT & T0_PS_1_256); //So that we have a timer opened. Probably a better choice to have in our initialization but it worked.
    
     switch(SeeLine.B)
     {
        case 0b00100u: //Straight
        case 0b00010u: //Curve right
        case 0b01000u: //Curve left
        case 0b01100u: //Curve left
        case 0b00110u: //Curve Right
        case 0b00001u: //Curve sharp right
        case 0b10000u: //Curve sharp left 
        case 0b00111u: //90 degrees Turn Right
        case 0b11100u: //90 degrees Turn Left
        case 0b11111u: //Block/Crossroads
        case 0b11110u: //Finetune
        case 0b01111u: //Finetune
        case 0b11000u: //Finetune sharp left curve
        case 0b00011u: //Finetune sharp right curve
        case 0b10100u: //sharp angle turn left
        case 0b00101u: //Sharp angle turn right
            
                       follow_complex_curves();
                       break;
        case 0b00000u:
        {   
            TMR0IF = 0;
            WriteTimer0(48911u); //(65536-49911)*256 = 4000000 TCY = .5s ~ 15cm
            straight_fwd();
            
            //Trapped in this while loop until it sees something again (reorientate), or our timer runs out (turn around)
            while(!TMR0IF && !SeeLine.B)
            {
                check_sensors();
                set_leds();
                                 
            }
            
            //If the bot sees something to its left first, that means it will want to merge towards the right, opposite applies to seeing right first.
            if (SeeLine.B)
            {
                if (SeeLine.B == 0b10000)
                {
                    merge_right();
                        while(!SeeLine.b.Center)//gently merge into the line until centered
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
                
                //If it sees two sensors light up at once, chances are that it is a gap followed by a 90 degree turn (Just not coming in completely straight). 
                //In this case, we will want it to spin itself to the line, rather than the gradual transition above
                //If it sees three sensors, it will break out of this anyways and do what the complex curves says to do.
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
            
            //If the timer goes off before it sees anything, it must be a dead end so let's turn around
            else if (TMR0IF)
            {
                turn_around();  
                break;
            }
        }
        
         default: 
         {
             //For debugging purposes we could set specific LED's and add a delay here to signal something wrong
             break;
         }
    }          
                    
}

//A complicated switch statement that tells our bot what to do based on the sensor input.
void follow_complex_curves(void)
{
  switch(SeeLine.B)
  {
    //If a 90 degree turn to the right is seen
    //Go forward a little bit, and then spin in place (clockwise) until the center line is seen
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
    
    //Same as above but to spin in place (counter-clockwise)
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
    
    //To cover our bases, these next two are made in case it comes at a 90 degree turn at an odd angle
    //And needs to reorientate a little bit in order to get the 3 sensor readings above
    case 0b01111u:
    {
      spin_right(); 
      break;
    }
    case 0b11110u:
    {
      spin_left();     
      break;
    }
    
    //Slight motion towards the left until it is centered again.
    case 0b11000u:
    {
        turn_left();
        while(!SeeLine.b.Center)
        {
            check_sensors();
            set_leds(); 
        } 
        
      break;
    }
    
    //Same as above case but towards the right slightly.
    case 0b00011u:
    {
        turn_right();
        while(!SeeLine.b.Center)
        {
            check_sensors();
            set_leds(); 
        }
      break;
    }
    
    //Keep on going forward until this isn't the case anymore (no pun intended)
    //Ideally our bot should follow a straight line forever with only being set once, but both motors are not perfect and equal.
    case 0b00100u:
    {
        if (!motor_speed_variable[1,1,0,0])
        {
            straight_fwd();    
            break;
        }
    }
    
    //If our bot sees something to the far left of its vision, correct itself greatly by going left until a new case comes up
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
    
    //Likewise for the far right
    case 0b00001u:
    {
      if(!SeeLine.b.Center)
      {
        if (!motor_speed_variable[2,-2,0,0])
          spin_right(); 
      }
      break;
    }
    
    //This and the next two cases after this set are for when it is slightly off a path, to correct itself
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
    
    //For reverse turns towards the right
    //The general idea of this one is to start a timer as soon as the array of sensors below are triggered
    //During this timer, if it sees all white, it will spin around instantly because it knows that it must be a reverse turn
    //If it doesn't see all white in this short timer, carry on following the curves afterwards
    case 0b00101u:
    {
        TMR0IF=0;
        WriteTimer0(0u);
        while( ReadTimer0() < 14286) //Tweakable timer to determine how long to keep going until we go back to following curves
        {
            check_sensors();
            set_leds();
            
            //To combat the parallel lines, we trap it to going one direction while it stays parallel
            if(SeeLine.B == 0b00101u)
            {
               while(SeeLine.B == 0b00101)
               {
                   check_sensors();
                   set_leds();
               }
               delay_x_seconds(5); 
               //this small delay is because sometimes it would see all white after it got off parallel lines
               //the fraction of time seeing all white triggered the bot to turn around until it was centered, so this was an experimental delay
               //it did seem to help a bit though.
            }
            
            check_sensors();
            set_leds();
            
            //Kind of redundant, but if it doesn't see parallel lines and sees all white, turn around.
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
    
    case 0b10100u: //for reverse turns to the left with similar logic to above case
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
    
    //If all sensors come on, it is either at the finish or a crossroad.
    //So we have a timer which lasts ~.25s and the bot will stop if:
    //if it sees all 5 sensors still.
    
    //If it sees anything else, then it breaks out and does its thing
    //For that quarter second, it is stuck going forward ignoring everything else.
    case 0b11111u:
    {
        TMR0IF = 0;
        WriteTimer0(57724u); //(65536-57724)*256 = .25s

        while(!TMR0IF) straight_fwd();
    
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


//Spinning in place counter-clockwise slowly.
void spin_left(void) 
{
    motor_speed_variable[-1,1,0,0];
    set_motor_speed(left, rev_slow, 0);
    set_motor_speed(right, slow, 0);
}

//Spinning in place clockwise slowly.
void spin_right(void)
{
    motor_speed_variable[1,-1,0,0];
    set_motor_speed(left, slow, 0);
    set_motor_speed(right, rev_slow, 0);
}

//Wide circular arc of motion to the left; can use for regular curves
void turn_left(void) //[0,3,0,0]
{
    motor_speed_variable[-1,3,0,0];
    set_motor_speed(left, rev_slow, 0); 
    set_motor_speed(right, fast, 0); 
}

//Wide circular arc of motion to the right; can use for regular curves
void turn_right(void) //[3,0,0,0]
{
     motor_speed_variable[3,-1,0,0];
    set_motor_speed(left, fast, 0); 
    set_motor_speed(right, rev_slow, 0);    
}

//Both wheels going forward slowly; for straight paths
void straight_fwd(void)
{
    motor_speed_variable[1,1,0,0];
    set_motor_speed(left, slow, 0);
    set_motor_speed(right, slow, 0); 
}

//Slight bend towards the right for smooth merging
void merge_right(void)
{
    motor_speed_variable[2,1,0,0];
    set_motor_speed(left, medium, 0); 
    set_motor_speed(right, slow, 0); 
}

//Slight bend towards the left for smooth merging
void merge_left(void) //[1,2,0,0]
{
     motor_speed_variable[1,2,0,20]; 
    set_motor_speed(left, slow, 0);
    set_motor_speed(right, medium, 20);
}

//Spin in place clockwise for about .75 seconds (Experimental/Tweakable variable), then goes straight ahead until it finds something
void turn_around(void)
{
    spin_right();
    delay_x_seconds(63); 
    
    straight_fwd();
    while(SeeLine.B == 0b00000)
    {
        check_sensors();
        set_leds();
    }
}


void delay_x_seconds(int time) //time = 80 for one second with 32MHz processor chip
{
    for(int i =0;i<time;i++)
    _delay(100000);
}    
