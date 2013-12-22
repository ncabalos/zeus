/******************************************************************************
 * Copyright (C) Nathaniel Abalos - All Rights Reserved                       *
 * Contents of this file is proprietary and confidential. Unauthorized        *
 * copying of this file, via any medium is strictly prohibited.               *
 *                                                                            *
 * Contact information: nathaniel_abalos@hotmail.com                          *
 ******************************************************************************/

/*
 * File:   pwm.h
 * Author: ncabalos
 *
 * Created on December 4, 2013, 11:23 PM
 */

#ifndef SERVO_H
#define	SERVO_H

typedef struct servo_info_s {
    uint16_t duty_cycle;
} SERVO_INFO;

extern void ServoInit(void);
extern uint16_t ServoSetPulseWidth(uint16_t channel, uint16_t pulse_width);
extern uint16_t ServoGetPulseWidth(uint16_t channel);


#endif	/* PWM_H */

