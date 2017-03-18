/* 
* 74HC595 library
*/

#include <Arduino.h>
#include <sreg.h>

static uint8_t ds_;
static uint8_t shcp_;
static uint8_t stcp_;
static int sreg_count_;

/* The state of the shift register */
static int shcp_reg_ = 0;

/*
* Initializes the shift register IC
* @ds - data input
* @shcp - clock
* @stcp - latch
* @init_value - number describing the initial state of the reg(s)
* @sreg_count - the number of the shift register ICs chained together
*/
void sreg_init(uint8_t ds, uint8_t shcp, uint8_t stcp, int init_value, int sreg_count) 
{
	ds_ = ds;
	shcp_ = shcp;
	stcp_ = stcp;
	sreg_count_ = sreg_count;

	pinMode(ds_, OUTPUT);
	pinMode(shcp_, OUTPUT);
	pinMode(stcp_, OUTPUT);

	sreg_write_bits(init_value);
}

/*
* Tests shift register pin value
* @pin - the pin which we want to read the value
* @returns - 0 or 1
*/
int sreg_test_bit(int pin)
{
	return !!(shcp_reg_ & 1 << pin);
}

/*
* Tests all bits stored in shcp register
* @returns - integer which is the state of the shcp register
*/
int sreg_test_all_bits() 
{
	return shcp_reg_;
}

/*
* Overwrites specific bit in the register
* @pin - the physical pin where the bit_value should be written to
* @bit_value - the value of the pin, 0 or 1. If you pass an integer 
* other than 0, it's autmatically converted to 1.
*/
void sreg_write_bit(int pin, int bit_value)
{
	shcp_reg_ = (shcp_reg_ & ~(1 << pin)) | (!!bit_value << pin);
}

/*
* Inverts (negates) specific bit in the register
* @pin - the physical pin where the bit_value should be written to
*/
void sreg_invert_bit(int pin)
{
	sreg_write_bit(pin, !sreg_test_bit(pin));
}

/*
* Sets the bits to be shifted into the sotrage register
* @bits - a number, each digit of which (in binary representation )will be sent to 
* the corresponding shif register pin, e.g. 0b00000011 will set pin0 to 1,
* pin1 to 1 and all the rest to 0
*/
void sreg_write_bits(int bits)
{
	shcp_reg_ = bits;
	sreg_latch_low();
	sreg_latch_high();
}

/* 
* Start reading values 
*/
void sreg_latch_low()
{
	digitalWrite(stcp_, 0);
	digitalWrite(ds_, 0);
	digitalWrite(shcp_, 0);
}

/* 
* Stop reading values 
*/
void sreg_latch_high()
{
	for (int i = (4 << sreg_count_) - 1; i >= 0; i--) {
		digitalWrite(shcp_, 0); /* falling edge of the clock, no data is transferred */
		digitalWrite(ds_, sreg_test_bit(i)); /* send zeroes and ones to the ds input */
		digitalWrite(shcp_, 1); /* data is shifted on the rising edge of the clock */
		digitalWrite(ds_, 0); /* prevent any change of data to bleed through*/
	}
	digitalWrite(stcp_, 1); /* data is shifter to the IC pins */
}
