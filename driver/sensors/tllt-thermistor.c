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

typedef struct TlltThermistorPrivate
{
	gboolean running;
} TlltThermistorPrivate;

static void tllt_thermistor_powerable_init(TlltPowerableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(TlltThermistor, tllt_thermistor, TLLT_TYPE_SENSOR,
						G_IMPLEMENT_INTERFACE(TLLT_TYPE_POWERABLE, tllt_thermistor_powerable_init)
							G_ADD_PRIVATE(TlltThermistor))

typedef enum TlltThermistorProps
{
	PROP_RUNNING = 1,
	PROP_GPIO_PIN,
	N_PROPS,
} TlltThermistorProps;

static GParamSpec *obj_properties[N_PROPS];

static void
tllt_thermistor_off(TlltThermistor *self)
{
	TlltThermistorPrivate *priv = tllt_thermistor_get_instance_private(self);

	priv->running = FALSE;

#ifdef TLLT_WITH_WIRINGPI
	digitalWrite(self->gpio_pin, LOW);
#endif
}

static void
tllt_thermistor_on(TlltThermistor *self)
{
	TlltThermistorPrivate *priv = tllt_thermistor_get_instance_private(self);

	priv->running = TRUE;

#ifdef TLLT_WITH_WIRINGPI
	digitalWrite(self->gpio_pin, HIGH);
#endif
}

static gboolean
tllt_thermistor_is_running(TlltThermistor *self)
{
	TlltThermistorPrivate *priv = tllt_thermistor_get_instance_private(self);

	return priv->running;
}

static void
tllt_thermistor_powerable_init(TlltPowerableInterface *iface)
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"
	iface->off		  = tllt_thermistor_off;
	iface->on		  = tllt_thermistor_on;
	iface->is_running = tllt_thermistor_is_running;
#pragma GCC diagnostic pop
}

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
	TlltThermistor *self		= TLLT_THERMISTOR(obj);
	TlltThermistorPrivate *priv = tllt_thermistor_get_instance_private(self);

	switch (prop_id) {
	case PROP_RUNNING:
		g_value_set_boolean(val, priv->running);
		break;
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
	TlltThermistor *self		= TLLT_THERMISTOR(obj);
	TlltThermistorPrivate *priv = tllt_thermistor_get_instance_private(self);

	switch (prop_id) {
	case PROP_RUNNING:
		priv->running = g_value_get_boolean(val);
		break;
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

	obj_properties[PROP_RUNNING] =
		g_param_spec_boolean("running", _("Running"), _("Whether the thermistor is running"), FALSE,
							 G_PARAM_PRIVATE | G_PARAM_READABLE | G_PARAM_CONSTRUCT_ONLY);
	obj_properties[PROP_GPIO_PIN] =
		g_param_spec_uchar("gpio-pin", _("GPIO pin"), _("GPIO pin for the thermo"), 0, UCHAR_MAX, 0,
						   G_PARAM_READWRITE | G_PARAM_CONSTRUCT);

	g_object_class_override_property(obj_class, PROP_RUNNING, "running");
	g_object_class_install_property(obj_class, PROP_GPIO_PIN, obj_properties[PROP_GPIO_PIN]);
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
