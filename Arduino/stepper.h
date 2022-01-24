
struct stepper
{
	 uint8_t dirpin;
	 uint8_t steppin;
	 uint8_t enpin;
	 uint8_t* port;
};
struct target
{
	uint8_t x;
	uint8_t y;
	uint16_t stepsx;
	uint16_t stepsy;
};
