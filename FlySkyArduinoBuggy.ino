/*
 * Flysky / turnigy IBus Two wheel buggy
 * Requires arduino & motor shield
 * Tested with Turnigy iA6C receiver
 */

#include <FlySkyIBus.h>
#include <AFMotor.h>
#include <math.h>

AF_DCMotor left_motor(4, MOTOR12_64KHZ);  // define motor on channel 1 with 64KHz PWM
AF_DCMotor right_motor(3, MOTOR12_64KHZ);  // define motor on channel 2 with 64KHz PWM

void setup() 
{
  Serial.begin(115200);
  IBus.begin(Serial);
}

void loop() 
{
  IBus.loop();

  unsigned int ibus0 = IBus.readChannel(0); // read in the latest ibus channel values
  unsigned int ibus1 = IBus.readChannel(1);

  boolean ibus0zero = (ibus0 == 0)? true : false; // if value is zero TX generally isn't on
  boolean ibus1zero = (ibus1 == 0)? true : false;
  boolean notx = false;
  if (ibus0zero or ibus1zero) {notx = true;}

  unsigned int failibus0 = (ibus0 >> 12) & 0xF;  // if last 4 bits are 0xC channels are in failsafe
  unsigned int failibus1 = (ibus1 >> 12) & 0xF;
  boolean ibus0fail = (failibus0 == 0xC)? true : false;
  boolean ibus1fail = (failibus1 == 0xC)? true : false;
  boolean nosignal = false;
  if (ibus0fail or ibus1fail) {nosignal = true;}
  
  float x=(((float(IBus.readChannel(0))/1000)*2)-3); // scale channels from 1k/2k to -1.0/+1.0
  float y=(((float(IBus.readChannel(1))/1000)*2)-3);

  float r=sqrt((x*x)+(y*y)); // convert to polar coords
  float t=atan2(y,x);

  t=t-(3.14/4); // rotate -45 degrees

  float left=(r * cos(t)); // convert back to cartesian coords
  float right=(r * sin(t));

  left = (left * sqrt(2)); // rescale the new coords
  right = (right * sqrt(2));
  
  left = max(-1,min(left,1)); // clamp to -1.0/+1.0
  right = max(-1,min(right,1));

  left = left*255; // scale to 0/255 for motors
  right = right*255;

  if (nosignal or notx) { // if link is unhealthy stop
    
    left_motor.run(RELEASE);
    left_motor.setSpeed(0);
    right_motor.run(RELEASE);
    right_motor.setSpeed(0);
    
  }else{ 
    
    if (left > 0) {  // set motors
      left_motor.run(FORWARD);
      left_motor.setSpeed(left);
    } else if (left < 0) {
      left_motor.run(BACKWARD);
      left = (left * -1);
      left_motor.setSpeed(left);
    } else {
      left_motor.run(RELEASE);
      left_motor.setSpeed(0);
    }

    if (right > 0) {
      right_motor.run(FORWARD);
      right_motor.setSpeed(right);
    } else if (right < 0) {
      right_motor.run(BACKWARD);
      right = (right * -1);
      right_motor.setSpeed(right);
    } else {
      right_motor.run(RELEASE);
      right_motor.setSpeed(0);
    }
  }
}
