#pragma  once
#define FIELD_SX 170 //Field dimensions in mm
#define FIELD_SY 170
#define OFFSETX 80	//Vector to the bottom left corner of the field in mm
#define OFFSETY 100
#define ACCEL 1400
#define MICROSTEPS 8
#define STEPSTURN 400
#define JOYSTICK_SW 2
#define MAGNET_PIN 4
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
}target;
