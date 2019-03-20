#include <glib-object.h>
#include <glib/gi18n.h>
#include <limits.h>

#include "tllt-powerable.h"
#include "tllt-readable.h"
#include "tllt-thermistor.h"

static void tllt_thermistor_powerable_interface_init(TlltPowerableInterface *iface);
static void tllt_thermistor_readable_interface_init(TlltReadableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(TlltThermistor, tllt_thermistor, G_TYPE_OBJECT,
						G_IMPLEMENT_INTERFACE(TLLT_TYPE_POWERABLE,
											  tllt_thermistor_powerable_interface_init)
							G_IMPLEMENT_INTERFACE(TLLT_TYPE_READABLE,
												  tllt_thermistor_readable_interface_init))

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
	return 0;
}

static void
tllt_thermistor_powerable_interface_init(TlltPowerableInterface *iface)
{
	iface->on  = tllt_thermistor_on;
	iface->off = tllt_thermistor_off;
}

static void
tllt_thermistor_readable_interface_init(TlltReadableInterface *iface)
{
	iface->read = tllt_thermistor_read;
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
	GObjectClass *obj_class = G_OBJECT_CLASS(klass);

	obj_class->set_property = tllt_thermistor_set_property;

	obj_properties[PROP_GPIO_PIN] =
		g_param_spec_uchar("gpio-pin", _("GPIO pin"), _("GPIO pin for the thermo"), 0, UCHAR_MAX, 0,
						   G_PARAM_READWRITE | G_PARAM_CONSTRUCT);

	g_object_class_install_properties(obj_class, N_PROPS, obj_properties);
}

static void
tllt_thermistor_init(G_GNUC_UNUSED TlltThermistor *self)
{}

TlltThermistor *
tllt_thermistor_new(const unsigned char gpio_pin)
{
	return g_object_new(TLLT_TYPE_THERMISTOR, "gpio-pin", gpio_pin, NULL);
}
