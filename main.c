//written by Simon Wilhelmstaetter on 16.05.2020
#include "../max7219/max7219_driver/max7219_driver.h"
#include <avr/interrupt.h>
#include <util/delay.h>

volatile int sensor_toggle_count = 0;

int main() {
    GIMSK = (1 << PCIE); //enable pin change interrupts
    PCMSK = (1 << PCINT3); // set the interrupt mask for the pin-change-interrupt of PB3
    TIMSK |= (1 << TOIE0); // set the overflow interrupt enable for counter0
    TCCR0B = (1 << CS02); // set prescaler of counter0 to 256 so that an overflow occurs every 0.512s+
    max7219_init();
    max7219_send_command(MAX7219_SCAN_LIMIT, MAX7219_SCAN_LIMIT_3); //only the first four digits are set
    max7219_send_command(MAX7219_DECODE_MODE, 0x0F); //Code B decode for the first four digits
    sei(); //set global interrupt enable flag
    while(1) {
        _delay_ms(0xFFFF);
    }
}

ISR(PCINT0_vect) {
    sensor_toggle_count++;
}

ISR(TIM0_OVF_vect, ISR_NOBLOCK) { //no blocking ISR to not miss any toggle interrupts
    //because the toggle interrupt counts both edges (low->high and high->low), it counts with the doubled frequency,
    //however the interrupt occurs nearly every half second which means that the number of toggles over a half
    //second equals the number of detections over one second which is the frequency in Hz
    //in conclusion: display the number of toggle interrupts every half second, no further processing needed
    int remainder = sensor_toggle_count;
    sensor_toggle_count = 0;
    remainder *= 60; //Hz to rpm
    for (unsigned char i = 1; i < 5; i++) { //only the last 4 digits are displayed
        if (remainder || i == 1) { //no leading zeros
            max7219_send_command(i, remainder % 10);
        } else {
            max7219_send_command(i, 0xF); //blank digit
        }
        remainder /= 10;
    }
}
