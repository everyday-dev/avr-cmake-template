#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <avr/io.h>

#define LED_STAT_DDR        (DDRD)
#define LED_STAT_PORT       (PORTD)
#define LED_STAT_PIN        (4)

int main(void) {
    // Set our LED port as an output
    LED_STAT_DDR |= (1 << LED_STAT_PIN);
    // Set the LED pin HIGH
    LED_STAT_PORT |= (1 << LED_STAT_PIN);

    while(1) {
        // wait here.. forever
    }
}
