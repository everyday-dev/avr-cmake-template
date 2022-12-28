/*! @file main.c
 * @brief Main source for the tiny-oled firmware
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <avr/io.h>

#define LED_STAT_DDR        (DDRD)
#define LED_STAT_PORT       (PORTD)
#define LED_STAT_PIN        (4)

/*!
 * @brief Main function and entry point for the firmware
 *
 * @param[in] void
 *
 * @return Returns void
 */
int main(void) {
    LED_STAT_DDR |= (1 << LED_STAT_PIN);
    LED_STAT_PORT |= (1 << LED_STAT_PIN);
    while(1) {
    }
}
