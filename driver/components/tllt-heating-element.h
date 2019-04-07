#pragma once

#include <glib-object.h>

G_BEGIN_DECLS

#define TLLT_TYPE_HEATING_ELEMENT (tllt_heating_element_get_type())
G_DECLARE_FINAL_TYPE(TlltHeatingElement, tllt_heating_element, TLLT, HEATING_ELEMENT, GObject)

struct _TlltHeatingElement
{
	GObject parent_instance;

	unsigned char gpio_pin;
};

TlltHeatingElement *tllt_heating_element_new(const unsigned char gpio_pin);

G_END_DECLS
