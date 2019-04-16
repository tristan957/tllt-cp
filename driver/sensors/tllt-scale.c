#include <limits.h>

#include <glib-object.h>
#include <glib/gi18n.h>
#ifdef TLLT_WITH_WIRINGPI
#	include <wiringPi.h>
#endif

#include "tllt-config.h"
#include "tllt-powerable.h"
#include "tllt-scale.h"

typedef struct TlltScalePrivate
{
	gboolean running;
} TlltScalePrivate;

static void tllt_scale_powerable_init(TlltPowerableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(TlltScale, tllt_scale, TLLT_TYPE_SENSOR,
						G_IMPLEMENT_INTERFACE(TLLT_TYPE_POWERABLE, tllt_scale_powerable_init)
							G_ADD_PRIVATE(TlltScale))

typedef enum TlltScaleProps
{
	PROP_RUNNING = 1,
	PROP_GPIO_PIN,
	N_PROPS,
} TlltScaleProps;

static GParamSpec *obj_properties[N_PROPS];

static void
tllt_scale_off(TlltScale *self)
{
	TlltScalePrivate *priv = tllt_scale_get_instance_private(self);

	priv->running = FALSE;

#ifdef TLLT_WITH_WIRINGPI
	digitalWrite(self->gpio_pin, LOW);
#endif
}

static void
tllt_scale_on(TlltScale *self)
{
	TlltScalePrivate *priv = tllt_scale_get_instance_private(self);

	priv->running = TRUE;

#ifdef TLLT_WITH_WIRINGPI
	digitalWrite(self->gpio_pin, HIGH);
#endif
}

static gboolean
tllt_scale_is_running(TlltScale *self)
{
	TlltScalePrivate *priv = tllt_scale_get_instance_private(self);

	return priv->running;
}

static void
tllt_scale_powerable_init(TlltPowerableInterface *iface)
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"
	iface->off		  = tllt_scale_off;
	iface->on		  = tllt_scale_on;
	iface->is_running = tllt_scale_is_running;
#pragma GCC diagnostic pop
}

static double
tllt_scale_read(G_GNUC_UNUSED TlltScale *self)
{
	return 0;
}

static void
tllt_scale_get_property(GObject *obj, guint prop_id, GValue *val, GParamSpec *pspec)
{
	TlltScale *self		   = TLLT_SCALE(obj);
	TlltScalePrivate *priv = tllt_scale_get_instance_private(self);

	switch (prop_id) {
	case PROP_RUNNING:
		g_value_set_boolean(val, priv->running);
		break;
	case PROP_GPIO_PIN:
		g_value_set_int(val, self->gpio_pin);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
		break;
	}
}

static void
tllt_scale_set_property(GObject *obj, guint prop_id, const GValue *val, GParamSpec *pspec)
{
	TlltScale *self		   = TLLT_SCALE(obj);
	TlltScalePrivate *priv = tllt_scale_get_instance_private(self);

	switch (prop_id) {
	case PROP_RUNNING:
		priv->running = g_value_get_boolean(val);
		break;
	case PROP_GPIO_PIN:
		self->gpio_pin = g_value_get_int(val);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
		break;
	}
}

static void
tllt_scale_class_init(TlltScaleClass *klass)
{
	GObjectClass *obj_class		  = G_OBJECT_CLASS(klass);
	TlltSensorClass *sensor_class = TLLT_SENSOR_CLASS(klass);

	obj_class->get_property = tllt_scale_get_property;
	obj_class->set_property = tllt_scale_set_property;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"
	sensor_class->read = tllt_scale_read;
#pragma GCC diagnostic pop

	obj_properties[PROP_RUNNING] =
		g_param_spec_boolean("running", _("Running"), _("Whether the scale is running"), FALSE,
							 G_PARAM_PRIVATE | G_PARAM_READABLE | G_PARAM_CONSTRUCT_ONLY);
	obj_properties[PROP_GPIO_PIN] =
		g_param_spec_int("gpio-pin", _("GPIO pin"), _("GPIO pin for the scale"), 0, INT_MAX, 0,
						 G_PARAM_READWRITE | G_PARAM_CONSTRUCT);

	g_object_class_override_property(obj_class, PROP_RUNNING, "running");
	g_object_class_install_property(obj_class, PROP_GPIO_PIN, obj_properties[PROP_GPIO_PIN]);
}

static void
tllt_scale_init(TLLT_UNUSED TlltScale *self)
{
#ifdef TLLT_WITH_WIRINGPI
	pinMode(self->gpio_pin, OUTPUT);
#endif
}

TlltScale *
tllt_scale_new(const int gpio_pin)
{
	return g_object_new(TLLT_TYPE_SCALE, "gpio-pin", gpio_pin, NULL);
}
