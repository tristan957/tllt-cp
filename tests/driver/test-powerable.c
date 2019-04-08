#include <glib-object.h>

#include "components/tllt-heating-element.h"
#include "tllt-powerable.h"

int
main(G_GNUC_UNUSED int argc, G_GNUC_UNUSED char *argv[])
{
	g_autoptr(TlltHeatingElement) elem = tllt_heating_element_new(0);

	tllt_powerable_on(TLLT_POWERABLE(elem));
	tllt_powerable_off(TLLT_POWERABLE(elem));

	return 0;
}
