#include <limits.h>

#include <glib-object.h>
#include <glib/gi18n.h>

#include "tllt-scale.h"

G_DEFINE_TYPE(TlltScale, tllt_scale, TLLT_TYPE_SENSOR)

typedef enum TlltScaleProps
{
	PROP_GPIO_PIN = 1,
	N_PROPS,
} TlltScaleProps;

static GParamSpec *obj_properties[N_PROPS];

static void
tllt_scale_off(G_GNUC_UNUSED TlltScale *self)
{}

static void
tllt_scale_on(G_GNUC_UNUSED TlltScale *self)
{}

static double
tllt_scale_read(G_GNUC_UNUSED TlltScale *self)
{
	return 0;
}

static void
tllt_scale_set_property(GObject *obj, guint prop_id, const GValue *val, GParamSpec *pspec)
{
	TlltScale *self = TLLT_SCALE(obj);

	switch (prop_id) {
	case PROP_GPIO_PIN:
		self->gpio_pin = g_value_get_uchar(val);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
		break;
	}
}

// Unfortunately the way GLib interface implementation is setup this is unavoidable
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"

static void
tllt_scale_class_init(TlltScaleClass *klass)
{
	GObjectClass *obj_class		  = G_OBJECT_CLASS(klass);
	TlltSensorClass *sensor_class = TLLT_SENSOR_CLASS(klass);

	obj_class->set_property = tllt_scale_set_property;

	sensor_class->off  = tllt_scale_off;
	sensor_class->on   = tllt_scale_on;
	sensor_class->read = tllt_scale_read;

	obj_properties[PROP_GPIO_PIN] =
		g_param_spec_uchar("gpio-pin", _("GPIO pin"), _("GPIO pin for the scale"), 0, UCHAR_MAX, 0,
						   G_PARAM_READWRITE | G_PARAM_CONSTRUCT);

	g_object_class_install_properties(obj_class, N_PROPS, obj_properties);
}

#pragma GCC diagnostic pop

static void
tllt_scale_init(G_GNUC_UNUSED TlltScale *self)
{}

TlltScale *
tllt_scale_new(const unsigned char gpio_pin)
{
	return g_object_new(TLLT_TYPE_SCALE, "gpio-pin", gpio_pin, NULL);
}
