#include <stdio.h>
#include "../max7219/max7219_driver/max7219_driver.h"
#include <avr/interrupt.h>

static int interrupt_count;

static int main() {
    interrupt_count = 0;
    PCMSK = 1 << 3; // set the interrupt mask for the pin-change-interrupt of PB3 (PCINT3)

    sei() //set global interrupt flag
}

ISR(PCINT3_vect) {
    interrupt_count++;
}
