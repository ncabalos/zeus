#include <stdint.h>
#include <HardwareConfig.h>
#include <servo.h>

typedef struct servo_module_s {
    uint16_t servo_pulse_widths[SERVO_CHANNEL_COUNT];
} SERVO_MODULE;

static SERVO_MODULE servoModule;

void ServoInit(void)
{
    uint16_t i;

    for(i = 0; i < SERVO_CHANNEL_COUNT; i++) {
        servoModule.servo_pulse_widths[i] = 0;
    }
}

uint16_t ServoSetPulseWidth(uint16_t channel, uint16_t pulse_width)
{
    if(channel >= SERVO_CHANNEL_COUNT) {
        return -1;
    }

    if(pulse_width > 100) {
        return -1;
    }

    servoModule.servo_pulse_widths[channel] = pulse_width;
    return 0;
}

uint16_t ServoGetPulseWidth(uint16_t channel)
{
    if(channel >= SERVO_CHANNEL_COUNT) {
        return -1;
    }

    return servoModule.servo_pulse_widths[channel];
}
