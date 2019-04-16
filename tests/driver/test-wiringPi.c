#ifndef TLLT_WITH_WIRINGPI
#	error "This test only works if libwiringPi is found"
#endif

#include <glib-object.h>
#include <wiringPi.h>

#include "tllt-driver.h"

int
main(G_GNUC_UNUSED int argc, G_GNUC_UNUSED const char *argv[])
{
	tllt_driver_init();

	pinMode(2, INPUT);
	digitalRead(2);
	pinMode(2, OUTPUT);
	digitalWrite(2, HIGH);
	digitalWrite(2, LOW);
	pinMode(2, INPUT);

	return 0;
}
