#include <glib-object.h>
#include <glib/gi18n.h>

#include "tllt-powerable.h"
#include "tllt-sensor.h"

G_DEFINE_ABSTRACT_TYPE(TlltSensor, tllt_sensor, G_TYPE_OBJECT)

static void
tllt_sensor_class_init(G_GNUC_UNUSED TlltSensorClass *klass)
{}

static void
tllt_sensor_init(G_GNUC_UNUSED TlltSensor *self)
{}

double
tllt_sensor_read(TlltSensor *self)
{
	TlltSensorClass *klass = TLLT_SENSOR_GET_CLASS(self);

	g_return_val_if_fail(klass->read != NULL, 0);

	return klass->read(self);
}
