#ifndef TLLT_WITH_WIRINGPI
#	error "This test only works if libwiringPi is found"
#endif

#include <glib-object.h>

#include "components/tllt-heating-element.h"
#include "tllt-driver.h"
#include "tllt-powerable.h"

#define BOTTOM_ELEMENT_PIN 2
#define TOP_ELEMENT_PIN 3

int
main(G_GNUC_UNUSED int argc, G_GNUC_UNUSED const char *argv[])
{
	tllt_driver_init();

	g_autoptr(TlltHeatingElement) bottom_element = tllt_heating_element_new(BOTTOM_ELEMENT_PIN);
	g_autoptr(TlltHeatingElement) top_element	= tllt_heating_element_new(TOP_ELEMENT_PIN);

	if (tllt_powerable_is_running(TLLT_POWERABLE(bottom_element)) &&
		tllt_powerable_is_running(TLLT_POWERABLE(top_element))) {
		tllt_powerable_off(TLLT_POWERABLE(bottom_element));
		tllt_powerable_off(TLLT_POWERABLE(top_element));
	} else {
		tllt_powerable_on(TLLT_POWERABLE(bottom_element));
		tllt_powerable_on(TLLT_POWERABLE(top_element));
	}

	return 0;
}
