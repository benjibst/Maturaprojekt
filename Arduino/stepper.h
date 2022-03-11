
typedef struct target
{
	uint8_t x;
	uint8_t y;
	uint16_t stepsx;
	uint16_t stepsy;
	uint8_t completed;
	
}target;
enum stageCompleted
{
	toX=0,
	toY=1,
	returnX=2,
	returnY=3
};
