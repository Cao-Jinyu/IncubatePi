#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>

#define PIN 18

int main (void) {

	if (wiringPiSetup() == -1) {
		perror("Wiring Pi setup failed");
		exit(1);
	}

	pwmSetMode(PWM_MODE_MS);
	pwmSetRange(32);
	pwmSetClock(24);
	pinMode(PIN, OUTPUT);

	const int min_pwm = 0;
	const int max_pwm = 31;

	while(1) {
		int pwm_value;

		printf("Type a value between %d and %d: ", min_pwm, max_pwm);
		scanf("%d", &pwm_value);

		if (pwm_value == -1) exit(0);
		else if ((pwm_value < min_pwm) || (pwm_value > max_pwm)) {
			printf("Value out of range\n");
			continue;
		}

		pwmWrite(PIN, 0);
		delay(100);
	}

	return 0;
}
