#ifndef Sreg_h
#define Sreg_h

void sreg_init(uint8_t ds, uint8_t shcp, uint8_t stcp, int sreg_init, int sreg_count);
int sreg_test_bit(int pin);
int sreg_test_all_bits();
void sreg_write_bit(int pin, int bit_value);
void sreg_invert_bit(int pin);
void sreg_write_bits(int bits);
void sreg_latch_low();
void sreg_latch_high();

#endif /* Sreg_h */