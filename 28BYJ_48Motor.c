#include <stdlib.h>
#include <avr/io.h>
#include "28BYJ_48Motor.h"

void motor_init()
{
	MOTOR_DDR |= (1 << IN1) | (1 << IN2) | (1 << IN3) | (1 << IN4);
	TCCR1B    |= (1 << CS11) | (1 << CS10) | (1 << WGM12);
}

void motor_stop()
{
	MOTOR_PORT &= ~((1 << IN1) | (1 << IN2) | (1 << IN3) | (1 << IN4));
}

void motor_set(uint8_t mask, uint16_t frequency)
{
	OCR1A = (F_CPU / (64 * frequency)) - 1;
	
	motor_stop();
	
	MOTOR_PORT |= mask;
	
	while (!(TIFR1 & (1 << OCF1A)));
	TIFR1 |= (1 << OCF1A);
	TCNT1 = 0;
}

void motor_move_steps(int16_t steps, uint16_t frequency)
{
	if (frequency < FRQ_MIN)
	{
		frequency = FRQ_MIN;
	}
	
	if (frequency > FRQ_MAX)
	{
		frequency = FRQ_MAX;
	}
	
	uint8_t step_def[8] =
	{
		(1 << IN1),
		(1 << IN1) | (1 << IN2),
		(1 << IN2),
		(1 << IN2) | (1 << IN3),
		(1 << IN3),
		(1 << IN3) | (1 << IN4),
		(1 << IN4),
		(1 << IN1) | (1 <<IN4),
	};
	
	if (steps > 0)
	{
		motor_set(step_def[0], frequency);
		for (uint16_t i = 0; i < steps; i++)
		{
			motor_set(step_def[7 - (i % 8)], frequency);
		}
		motor_stop();
	}
	else
	{
		motor_set(step_def[7], frequency);
		for (uint16_t i = 1; i < abs(steps) + 1; i++)
		{
			motor_set(step_def[i % 8], frequency);
		}
		motor_stop();
	}
}