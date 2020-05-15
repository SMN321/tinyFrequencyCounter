#include <stdio.h>
#include "../max7219/max7219_driver/max7219_driver.h"
#include <avr/interrupt.h>

static int sensor_toggle_count;
static int sensor_toggle_count_copy;

static int main() {
    interrupt_count = 0;
    PCMSK = (1 << PCINT3); // set the interrupt mask for the pin-change-interrupt of PB3
    TIMSK |= (1 << TOIE0); // set the overflow interrupt enable for counter0
    TCCR0B = (1 << CS02) | (1 << CS00); // set prescaler of counter0 to 1024
    sei() //set global interrupt enable flag
    while(1) {
        _delay_ms(0xFF);
    }

}

ISR(PCINT3_vect) {
    interrupt_count++;
}

ISR(TIM0_OVF_vect, ISR_NOBLOCK) {
    sensor_toggle_count_copy = sensor_toggle_count;
    sensor_toggle_count = 0;

    //TODO: calculate frequency in hz and display it
}
