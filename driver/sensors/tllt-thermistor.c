#include <limits.h>

#include <glib-object.h>
#include <glib/gi18n.h>
#ifdef TLLT_WITH_WIRINGPI
#	include <wiringPi.h>
#endif

#include "tllt-config.h"
#include "tllt-powerable.h"
#include "tllt-sensor.h"
#include "tllt-thermistor.h"

G_DEFINE_TYPE(TlltThermistor, tllt_thermistor, TLLT_TYPE_SENSOR)

typedef enum TlltThermistorProps
{
	PROP_GPIO_PIN = 1,
	N_PROPS,
} TlltThermistorProps;

static GParamSpec *obj_properties[N_PROPS];

static double
tllt_thermistor_read(G_GNUC_UNUSED TlltThermistor *self)
{
	static int i = 0;
	if ((i & 2) == 0) {
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

static void
tllt_thermistor_class_init(TlltThermistorClass *klass)
{
	GObjectClass *obj_class		  = G_OBJECT_CLASS(klass);
	TlltSensorClass *sensor_class = TLLT_SENSOR_CLASS(klass);

	obj_class->get_property = tllt_thermistor_get_property;
	obj_class->set_property = tllt_thermistor_set_property;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"
	sensor_class->read = tllt_thermistor_read;
#pragma GCC diagnostic pop

	obj_properties[PROP_GPIO_PIN] =
		g_param_spec_uchar("gpio-pin", _("GPIO pin"), _("GPIO pin for the thermo"), 0, UCHAR_MAX, 0,
						   G_PARAM_READWRITE | G_PARAM_CONSTRUCT);

	g_object_class_install_properties(obj_class, N_PROPS, obj_properties);
}

static void
tllt_thermistor_init(TLLT_UNUSED TlltThermistor *self)
{
#ifdef TLLT_WITH_WIRINGPI
	pinMode(self->gpio_pin, OUTPUT);
#endif
}

TlltThermistor *
tllt_thermistor_new(const unsigned char gpio_pin)
{
	return g_object_new(TLLT_TYPE_THERMISTOR, "gpio-pin", gpio_pin, NULL);
}
