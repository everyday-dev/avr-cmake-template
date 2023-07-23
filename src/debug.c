#include <stdio.h>
#include <avr/io.h>

static int uart_putchar(char c, FILE *stream);

static FILE mystdout = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);

void debug_init(void) {

    UCSR1B = (1 << TXEN1); // Enable transmitter
    UCSR1C = (0x03 << UCSZ10); // Set data size to 8 bits
    UBRR1L = (0x08); // Set baud rate to 115200
    UBRR1H = (0x00);

    stdout = &mystdout;		// define the output stream
}

static int uart_putchar(char c, FILE *stream) {
    if (c == '\n') {
        uart_putchar('\r', stream);
    }

    // Wait for empty transmit buffer
    while ( !( UCSR1A & (1<<UDRE1)) )
    ;

    // Put data into buffer, sends the data
    UDR1 = c;
    return 0;
}