#include <glib-object.h>
#include <glib/gi18n.h>

#include "tllt-powerable.h"
#include "tllt-sensor.h"

static void tllt_sensor_powerable_init(TlltPowerableInterface *iface);

G_DEFINE_ABSTRACT_TYPE_WITH_CODE(TlltSensor, tllt_sensor, G_TYPE_OBJECT,
								 G_IMPLEMENT_INTERFACE(TLLT_TYPE_POWERABLE,
													   tllt_sensor_powerable_init))

static void
tllt_sensor_powerable_init(TlltPowerableInterface *iface)
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"
	iface->off = tllt_sensor_off;
	iface->on  = tllt_sensor_on;
#pragma GCC diagnostic pop
}

static void
tllt_sensor_class_init(G_GNUC_UNUSED TlltSensorClass *klass)
{}

static void
tllt_sensor_init(G_GNUC_UNUSED TlltSensor *self)
{}

void
tllt_sensor_off(TlltSensor *self)
{
	TlltSensorClass *klass = TLLT_SENSOR_GET_CLASS(self);
	klass->off(self);
}

void
tllt_sensor_on(TlltSensor *self)
{
	TlltSensorClass *klass = TLLT_SENSOR_GET_CLASS(self);
	klass->on(self);
}

double
tllt_sensor_read(TlltSensor *self)
{
	TlltSensorClass *klass = TLLT_SENSOR_GET_CLASS(self);

	return klass->read(self);
}
