#ifndef _28BYJ_48MOTOR_H_
#define _28BYJ_48MOTOR_H_

#define F_CPU 8000000UL

#define IN1 0
#define IN2 1
#define IN3 2
#define IN4 3

#define MOTOR_PORT PORTB
#define MOTOR_DDR  DDRB

#define SET(pin) MOTOR_PORT |=  (1 << pin)
#define CLR(pin) MOTOR_PORT &= ~(1 << pin)

#define MAX_STEPS 1024
	
void motor_init();
void motor_stop();
void motor_move_steps(int16_t steps, uint16_t frequency);
void motor_set(uint8_t mask, uint16_t frequency);

#endif /* _28BYJ_48MOTOR_H_ */