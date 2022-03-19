typedef struct
{
	uint16_t x;
	uint16_t y;
} point;

typedef struct 
{
	uint8_t x;
	uint8_t y;
	point pos;
	uint8_t completed;
}target;

typedef struct
{
	volatile uint8_t* Port;
	volatile uint8_t* Ddr;
	uint8_t StepPin;
	uint8_t DirPin;
	uint8_t EnPin;
	point pos;
}stepper;

stepper stepperX = {.Port=&PORTB,.Ddr=&DDRB,.StepPin=PORTB4,.DirPin=PORTB3,.EnPin=PORTB2};
void stepperInit(stepper* s)
{
	*(s->Ddr)&=~((1<<s->DirPin)|(1<<s->EnPin)|(1<<s->StepPin));
}
void step(stepper* s, uint8_t dir)
{
	*(s->Port)&=~1<<(uint8_t)s->DirPin;
	*(s->Port)|=dir<<(uint8_t)s->DirPin;
	*(s->Port)^=(1<<(uint8_t)s->StepPin);
}



