#include <limits.h>

#include <glib-object.h>
#include <glib/gi18n.h>

#include "tllt-sensor.h"
#include "tllt-thermistor.h"

G_DEFINE_TYPE(TlltThermistor, tllt_thermistor, TLLT_TYPE_SENSOR)

typedef enum TlltThermistorProps
{
	PROP_GPIO_PIN = 1,
	N_PROPS,
} TlltThermistorProps;

static GParamSpec *obj_properties[N_PROPS];

static void
tllt_thermistor_off(G_GNUC_UNUSED TlltThermistor *self)
{}

static void
tllt_thermistor_on(G_GNUC_UNUSED TlltThermistor *self)
{}

static double
tllt_thermistor_read(G_GNUC_UNUSED TlltThermistor *self)
{
	static int i = 0;
	if (i & 2 == 0) {
		return 350;
	}

	return 400;
}

static void
tllt_thermistor_get_property(GObject *obj, guint prop_id, GValue *val, GParamSpec *pspec)
{
	TlltThermistor *self = TLLT_THERMISTOR(obj);

	switch (prop_id) {
	case PROP_GPIO_PIN:
		g_value_set_uchar(val, self->gpio_pin);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
		break;
	}
}

static void
tllt_thermistor_set_property(GObject *obj, guint prop_id, const GValue *val, GParamSpec *pspec)
{
	TlltThermistor *self = TLLT_THERMISTOR(obj);

	switch (prop_id) {
	case PROP_GPIO_PIN:
		self->gpio_pin = g_value_get_uchar(val);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
		break;
	}
}

// Unfortunately the way GLib abstract class implementation is setup this is unavoidable
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"

static void
tllt_thermistor_class_init(TlltThermistorClass *klass)
{
	GObjectClass *obj_class		  = G_OBJECT_CLASS(klass);
	TlltSensorClass *sensor_class = TLLT_SENSOR_CLASS(klass);

	obj_class->get_property = tllt_thermistor_get_property;
	obj_class->set_property = tllt_thermistor_set_property;

	sensor_class->off  = tllt_thermistor_off;
	sensor_class->on   = tllt_thermistor_on;
	sensor_class->read = tllt_thermistor_read;

	obj_properties[PROP_GPIO_PIN] =
		g_param_spec_uchar("gpio-pin", _("GPIO pin"), _("GPIO pin for the thermo"), 0, UCHAR_MAX, 0,
						   G_PARAM_READWRITE | G_PARAM_CONSTRUCT);

	g_object_class_install_properties(obj_class, N_PROPS, obj_properties);
}

#pragma GCC diagnostic pop

static void
tllt_thermistor_init(G_GNUC_UNUSED TlltThermistor *self)
{}

TlltThermistor *
tllt_thermistor_new(const unsigned char gpio_pin)
{
	return g_object_new(TLLT_TYPE_THERMISTOR, "gpio-pin", gpio_pin, NULL);
}
