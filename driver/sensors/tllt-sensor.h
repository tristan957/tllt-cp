#pragma once

#include <glib-object.h>

G_BEGIN_DECLS

#define TLLT_TYPE_SENSOR (tllt_sensor_get_type())
G_DECLARE_DERIVABLE_TYPE(TlltSensor, tllt_sensor, TLLT, SENSOR, GObject)

struct _TlltSensorClass
{
	GObjectClass parent_class;

	double (*read)(TlltSensor *self);
};

double tllt_sensor_read(TlltSensor *self);

G_END_DECLS
