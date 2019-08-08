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

void motor_move_steps(int16_t steps, uint16_t frequency, DirectionType direction)
{	
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
	
	if (direction == Left)
	{
		MOTOR_PORT |= (1 << step_def[7]);
	}
	
	for (uint16_t i = 0; i < steps; i ++)
	{
		OCR1A = (F_CPU / (64 * frequency)) - 1;
		
		motor_stop();
		
		if (direction == Right)
		{
			MOTOR_PORT |= step_def[(7 - (i % 8))];
		}
		
		if (direction == Left)
		{
			MOTOR_PORT |= step_def[(i % 8)];
		}
		
		while (!(TIFR1 & (1 << OCF1A)));
		TIFR1 |= (1 << OCF1A);
		TCNT1 = 0;
	}
	
	motor_stop();
}