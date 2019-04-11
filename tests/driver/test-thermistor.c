#ifndef TLLT_WITH_WIRINGPI
#	error "This test only works if libwiringPi is found"
#endif

#include <glib-object.h>
#include <mcp3004.h>
#include <wiringPi.h>

#define SPI_CHAN 0
#define BASE_PIN 100
#define NUM_OF_PINS 8
#define FOREVER 0

int
main(G_GNUC_UNUSED int argc, G_GNUC_UNUSED char *argv[])
{
	wiringPiSetup();
	mcp3004Setup(BASE_PIN, SPI_CHAN);

	do {
		int result[NUM_OF_PINS];
		for (int i = 0; i < NUM_OF_PINS; i++) {
			result[i] = analogRead(BASE_PIN + i);
		}

		g_print("%d, %d, %d, %d, %d, %d, %d, %d\n", result[0], result[1], result[2], result[3],
				result[4], result[5], result[6], result[7]);
		delay(500);
	} while (FOREVER);

	return 0;
}
