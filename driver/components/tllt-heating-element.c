#include <limits.h>

#include <glib-object.h>
#include <glib/gi18n.h>
#ifdef TLLT_WITH_WIRINGPI
#	include <wiringPi.h>
#endif

#include "tllt-config.h"
#include "tllt-heating-element.h"
#include "tllt-powerable.h"

static void tllt_heating_element_powerable_init(TlltPowerableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(TlltHeatingElement, tllt_heating_element, G_TYPE_OBJECT,
						G_IMPLEMENT_INTERFACE(TLLT_TYPE_POWERABLE,
											  tllt_heating_element_powerable_init))

typedef enum TlltHeatingElementProps
{
	PROP_GPIO_PIN = 1,
	N_PROPS,
} TlltHeatingElementProps;

static GParamSpec *obj_properties[N_PROPS];

static void
tllt_heating_element_off(TLLT_UNUSED TlltHeatingElement *self)
{
#ifdef TLLT_WITH_WIRINGPI
	digitalWrite(self->gpio_pin, LOW);
#endif
}

static void
tllt_heating_element_on(TLLT_UNUSED TlltHeatingElement *self)
{
#ifdef TLLT_WITH_WIRINGPI
	digitalWrite(self->gpio_pin, HIGH);
#endif
}

static void
tllt_heating_element_powerable_init(TlltPowerableInterface *iface)
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"
	iface->off = tllt_heating_element_off;
	iface->on  = tllt_heating_element_on;
#pragma GCC diagnostic pop
}

static void
tllt_heating_element_get_property(GObject *obj, guint prop_id, GValue *val, GParamSpec *pspec)
{
	TlltHeatingElement *self = TLLT_HEATING_ELEMENT(obj);

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
tllt_heating_element_set_property(GObject *obj, guint prop_id, const GValue *val, GParamSpec *pspec)
{
	TlltHeatingElement *self = TLLT_HEATING_ELEMENT(obj);

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
tllt_heating_element_class_init(G_GNUC_UNUSED TlltHeatingElementClass *klass)
{
	GObjectClass *obj_class = G_OBJECT_CLASS(klass);

	obj_class->get_property = tllt_heating_element_get_property;
	obj_class->set_property = tllt_heating_element_set_property;

	obj_properties[PROP_GPIO_PIN] =
		g_param_spec_uchar("gpio-pin", _("GPIO pin"), _("GPIO pin for the scale"), 0, UCHAR_MAX, 0,
						   G_PARAM_READWRITE | G_PARAM_CONSTRUCT);

	g_object_class_install_properties(obj_class, N_PROPS, obj_properties);
}

static void
tllt_heating_element_init(TLLT_UNUSED TlltHeatingElement *self)
{
#ifdef TLLT_WITH_WIRINGPI
	pinMode(self->gpio_pin, OUTPUT);
#endif
}

TlltHeatingElement *
tllt_heating_element_new(const unsigned char gpio_pin)
{
	return g_object_new(TLLT_TYPE_HEATING_ELEMENT, "gpio-pin", gpio_pin, NULL);
}
