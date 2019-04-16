#pragma once

#include <glib-object.h>

#include "tllt-sensor.h"

G_BEGIN_DECLS

typedef enum TlltThermistorState
{
	STATE_READ,
	STATE_NO_READ,
	STATE_WAIT,
} TlltThermistorState;

struct _TlltThermistor
{
	TlltSensor parent_instance;

	int spi_chan;
	int base_pin;
	unsigned int num_pins;
};

#define TLLT_TYPE_THERMISTOR (tllt_thermistor_get_type())
G_DECLARE_FINAL_TYPE(TlltThermistor, tllt_thermistor, TLLT, THERMISTOR, TlltSensor)

TlltThermistor *tllt_thermistor_new(const int spi_chan, const int base_pin,
									const unsigned int num_pins);
double tllt_thermistor_reading_to_farenheit(const double reading);

G_END_DECLS
