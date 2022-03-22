#pragma  once
#define FIELD_SX 250 //Field dimensions in mm
#define FIELD_SY 250
#define OFFSETX 30	//Vector to the bottom left corner of the field in mm
#define OFFSETY 60
#define ACCEL 256
#define MICROSTEPS 8
#define STEPSTURN 400
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
