#ifndef TLLT_WITH_WIRINGPI
#	error "This test only works if libwiringPi is found"
#endif

#include <glib-object.h>
#include <mcp3004.h>
#include <wiringPi.h>

#define SPI_CHAN 0
#define BASE_PIN 100
#define NUM_OF_PINS 8
#define DELAY 500

gboolean
all_zero(int *result)
{
	return result[1] == 0 && result[2] == 0 && result[3] == 0 && result[4] == 0 && result[5] == 0 &&
		   result[6] == 0 && result[7] == 0;
}

int
main(G_GNUC_UNUSED int argc, G_GNUC_UNUSED char *argv[])
{
	wiringPiSetupGpio();
	mcp3004Setup(BASE_PIN, SPI_CHAN);

	int result[NUM_OF_PINS];
	do {
		double avg	 = 0;
		int iterations = 0;

		do {
			for (int i = 0; i < NUM_OF_PINS; i++) {
				result[i] = analogRead(BASE_PIN + i);
			}
			g_print("%4d %4d %4d %4d %4d %4d %4d %4d\n", result[0], result[1], result[2], result[3],
					result[4], result[5], result[6], result[7]);
			avg += result[0];
			iterations++;
		} while (!all_zero(result));

		// delay(DELAY);
	} while (1);

	return 0;
}
