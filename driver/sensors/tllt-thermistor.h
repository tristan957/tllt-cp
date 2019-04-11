#pragma once

#include <glib-object.h>

#include "tllt-sensor.h"

G_BEGIN_DECLS

struct _TlltThermistor
{
	TlltSensor parent_instance;

	unsigned int spi_chan;
	unsigned int base_pin;
	unsigned int num_pins;
};

#define TLLT_TYPE_THERMISTOR (tllt_thermistor_get_type())
G_DECLARE_FINAL_TYPE(TlltThermistor, tllt_thermistor, TLLT, THERMISTOR, TlltSensor)

TlltThermistor *tllt_thermistor_new(const unsigned int spi_chan, const unsigned int base_pin,
									const unsigned int num_pins);

G_END_DECLS
