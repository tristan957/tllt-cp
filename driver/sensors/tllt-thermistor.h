#pragma once

#include <glib-object.h>

#include "tllt-sensor.h"

G_BEGIN_DECLS

struct _TlltThermistor
{
	unsigned char gpio_pin;
};

#define TLLT_TYPE_THERMISTOR (tllt_thermistor_get_type())
G_DECLARE_FINAL_TYPE(TlltThermistor, tllt_thermistor, TLLT, THERMISTOR, TlltSensor)

TlltThermistor *tllt_thermistor_new(const unsigned char gpio_pin);

G_END_DECLS
