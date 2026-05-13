

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

/* -------- Pin Definitions -------- */

// PIR Sensor Input
#define PIR_SENSOR   PD2

// Outputs
#define STATUS_LED   PB0
#define ALERT_LED    PB1
#define BUZZER       PB2

/* -------- Function Prototypes -------- */

void system_init(void);
void status_led_blink(void);
void alarm_on(void);
void alarm_off(void);

void tone(uint16_t frequency);
void noTone(void);

/* -------- Main Program -------- */

int main(void)
{
    system_init();

    while(1)
    {
        // System running indication
        status_led_blink();

        // Check PIR sensor
        if(PIND & (1 << PIR_SENSOR))
        {
            // Motion detected
            alarm_on();

            // Generate buzzer tone (1000 Hz)
            tone(1000);

            // Alarm duration = 5 seconds
            for(uint8_t i = 0; i < 5; i++)
            {
                _delay_ms(1000);
            }

            // Stop buzzer
            noTone();

            // Turn OFF alarm LED
            alarm_off();
        }
    }
}

/* -------- System Initialization -------- */

void system_init(void)
{
    // Set LEDs and buzzer as OUTPUT
    DDRB |= (1 << STATUS_LED);
    DDRB |= (1 << ALERT_LED);
    DDRB |= (1 << BUZZER);

    // Set PIR sensor as INPUT
    DDRD &= ~(1 << PIR_SENSOR);

    // Enable internal pull-up resistor
    PORTD |= (1 << PIR_SENSOR);

    // Initially OFF
    PORTB &= ~(1 << STATUS_LED);
    PORTB &= ~(1 << ALERT_LED);
    PORTB &= ~(1 << BUZZER);
}

/* -------- Status LED Blink -------- */

void status_led_blink(void)
{
    PORTB ^= (1 << STATUS_LED);
    _delay_ms(1000);
}

/* -------- Alarm ON -------- */

void alarm_on(void)
{
    PORTB |= (1 << ALERT_LED);
}

/* -------- Alarm OFF -------- */

void alarm_off(void)
{
    PORTB &= ~(1 << ALERT_LED);
}

/* -------- tone() Function -------- */
/*
   Simple square wave generator
   Frequency in Hz
*/

void tone(uint16_t frequency)
{
    uint16_t delay_time;

    delay_time = 1000000UL / (frequency * 2);

    // Generate tone for short duration
    for(uint16_t i = 0; i < 1000; i++)
    {
        PORTB ^= (1 << BUZZER);

        for(uint16_t j = 0; j < delay_time; j++)
        {
            _delay_us(1);
        }
    }
}

/* -------- noTone() Function -------- */

void noTone(void)
{
    PORTB &= ~(1 << BUZZER);
}