#include <glib-object.h>

#include "sensors/tllt-sensor.h"
#include "sensors/tllt-thermistor.h"

int
main(G_GNUC_UNUSED int argc, G_GNUC_UNUSED char *argv[])
{
	g_autoptr(TlltThermistor) thermistor = tllt_thermistor_new(0, 100, 4);

	tllt_sensor_read(TLLT_SENSOR(thermistor));

	return 0;
}
