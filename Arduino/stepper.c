#include "stepper.h"

inline void step(steptypes s)
{
	XPort&=~(1<<XDirPin);		//set dir pins to 0
	YPort&=~(1<<YDirPin);
	XPort|=((s>>1)&1)<<(XDirPin); //if dir bit is set set dir bits in stepper
	YPort|=((s>>1)&1)<<(YDirPin);
	XPort^=((~s)&1)<<XStepPin; //if stepper bit is 0 then do step x 
	YPort^=(s&1)<<YStepPin;	//if stepper bit is 1 then do step y
}

point pointfrompos(uint8_t x,uint8_t y)
{
	uint16_t microstepsturn=STEPSTURN*MICROSTEPS;
	
	return (point){
		(uint16_t)(x/255.0f*FIELD_SX+OFFSETX)/42.25*microstepsturn,
		(uint16_t)(y/255.0f*FIELD_SY+OFFSETY)/42.25*microstepsturn};
}
void move(steptypes s,uint16_t microsteps)
{
	cli();
	TCCR1A = 0;
	TCCR1B=0b00001100;
	uint16_t donesteps=0;
	uint16_t sollsteps=0;
	uint16_t accelsteps=1200;
	uint16_t tcntrev=0;
	microsteps -= 2*accelsteps;
	float time;
	float vend;
	TCNT1=0;
	while(donesteps<=accelsteps)
	{
		time = TCNT1/15625.0;
		sollsteps=ACCEL*time*time;
		sollsteps>>=1;
		if(sollsteps>donesteps)
		{
			donesteps++;
			step(s);
		}
	}
	tcntrev=TCNT1;
	TCNT1=0;
	vend=(uint16_t)15625.0/(ACCEL*time);
	while(microsteps--)
	{
		while(TCNT1<vend);
		TCNT1=0;
		step(s);		
	}
	TCNT1=0;
	while(donesteps>0)
	{
		
		time = (tcntrev-TCNT1)/15625.0;
		sollsteps=ACCEL*time*time;
		sollsteps>>=1;
		if(sollsteps<donesteps)
		{
			donesteps--;
			step(s);
		}
	}
}
