#include <glib-object.h>

#include "sensors/tllt-sensor.h"
#include "sensors/tllt-thermistor.h"
#include "tllt-toaster.h"

int
main(G_GNUC_UNUSED int argc, G_GNUC_UNUSED char *argv[])
{
	g_autoptr(GError) err = NULL;
	g_autoptr(TlltToaster) toaster =
		tllt_toaster_new_from_config_file("./tests/driver/test.config.json", &err);
	if (err != NULL) {
		g_printerr("%s\n", err->message);

		return 1;
	}

	do {
		g_print("%f\n", tllt_sensor_read(TLLT_SENSOR(toaster->thermistor)));
	} while (1);

	return 0;
}
