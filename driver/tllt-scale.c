#include <limits.h>

#include <glib-object.h>
#include <glib/gi18n.h>

#include "tllt-powerable.h"
#include "tllt-readable.h"
#include "tllt-scale.h"

static void tllt_scale_readable_interface_init(TlltReadableInterface *iface);
static void tllt_scale_powerable_interface_init(TlltPowerableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(TlltScale, tllt_scale, G_TYPE_OBJECT,
						G_IMPLEMENT_INTERFACE(TLLT_TYPE_POWERABLE,
											  tllt_scale_powerable_interface_init)
							G_IMPLEMENT_INTERFACE(TLLT_TYPE_READABLE,
												  tllt_scale_readable_interface_init))

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
tllt_scale_powerable_interface_init(TlltPowerableInterface *iface)
{
	iface->on  = tllt_scale_on;
	iface->off = tllt_scale_off;
}

static void
tllt_scale_readable_interface_init(TlltReadableInterface *iface)
{
	iface->read = tllt_scale_read;
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

static void
tllt_scale_class_init(TlltScaleClass *klass)
{
	GObjectClass *obj_class = G_OBJECT_CLASS(klass);

	obj_class->set_property = tllt_scale_set_property;

	obj_properties[PROP_GPIO_PIN] =
		g_param_spec_uchar("gpio-pin", _("GPIO pin"), _("GPIO pin for the scale"), 0, UCHAR_MAX, 0,
						   G_PARAM_READWRITE | G_PARAM_CONSTRUCT);

	g_object_class_install_properties(obj_class, N_PROPS, obj_properties);
}

static void
tllt_scale_init(G_GNUC_UNUSED TlltScale *self)
{}

TlltScale *
tllt_scale_new(const unsigned char gpio_pin)
{
	return g_object_new(TLLT_TYPE_SCALE, "gpio-pin", gpio_pin, NULL);
}
