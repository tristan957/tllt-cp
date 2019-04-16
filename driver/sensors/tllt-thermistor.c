#include <limits.h>
#include <math.h>

#include <glib-object.h>
#include <glib/gi18n.h>
#ifdef TLLT_WITH_WIRINGPI
#	include <wiringPi.h>
#	include <mcp3004.h>
#endif

#include "tllt-config.h"
#include "tllt-powerable.h"
#include "tllt-sensor.h"
#include "tllt-thermistor.h"
#include "tllt-util.h"

// Correlate temperatures with known readings of the thermistor
static const int readings[10][2] = {
	{175, 520}, {200, 690}, {225, 740}, {250, 800},  {275, 830},
	{300, 880}, {325, 940}, {375, 985}, {400, 1000}, {425, 1010},
};

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-const-variable"
// Correlate temperatures with the amount of time it takes in seconds to heat up to that temp
static const int heat_up_timings[7][2] = {
	{300, 120}, {325, 155}, {350, 170}, {375, 185}, {400, 210}, {425, 230}, {450, 245},
};
#pragma GCC diagnostic pop

G_DEFINE_TYPE(TlltThermistor, tllt_thermistor, TLLT_TYPE_SENSOR)

typedef enum TlltThermistorProps
{
	PROP_SPI_CHAN = 1,
	PROP_BASE_PIN,
	PROP_NUMBER_OF_PINS,
	N_PROPS,
} TlltThermistorProps;

static GParamSpec *obj_properties[N_PROPS];

static double
tllt_thermistor_read(TLLT_UNUSED TlltThermistor *self)
{
	double value = 0;
#ifdef TLLT_WITH_WIRINGPI
	g_autofree int *result = g_malloc(self->num_pins * sizeof(int));
	for (unsigned int i = 0; i < self->num_pins; i++) {
		result[i] = analogRead(self->base_pin + i);
	}

	value = result[0];
#else
	// Note: this is 325 deg
	value = 940;
#endif

	return value;
}

static void
tllt_thermistor_get_property(GObject *obj, guint prop_id, GValue *val, GParamSpec *pspec)
{
	TlltThermistor *self = TLLT_THERMISTOR(obj);

	switch (prop_id) {
	case PROP_SPI_CHAN:
		g_value_set_int(val, self->spi_chan);
		break;
	case PROP_BASE_PIN:
		g_value_set_int(val, self->base_pin);
		break;
	case PROP_NUMBER_OF_PINS:
		g_value_set_uint(val, self->num_pins);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
		break;
	}
}

static void
tllt_thermistor_set_property(GObject *obj, guint prop_id, const GValue *val, GParamSpec *pspec)
{
	TlltThermistor *self = TLLT_THERMISTOR(obj);

	switch (prop_id) {
	case PROP_SPI_CHAN:
		self->spi_chan = g_value_get_int(val);
		break;
	case PROP_BASE_PIN:
		self->base_pin = g_value_get_int(val);
		break;
	case PROP_NUMBER_OF_PINS:
		self->num_pins = g_value_get_uint(val);
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

	obj_properties[PROP_SPI_CHAN] =
		g_param_spec_int("spi-chan", _("SPI Channel"), _("SPI channel for the thermistor"), 0, 1, 0,
						 G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY);
	obj_properties[PROP_BASE_PIN] =
		g_param_spec_int("base-pin", _("Base Pin"),
						 _("Base pin for the thermistor, keeping in mind this is a virtual pin"), 0,
						 UINT_MAX - 8, 100, G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY);
	obj_properties[PROP_NUMBER_OF_PINS] = g_param_spec_uint(
		"number-of-pins", _("Number of Pins"), _("Number of virtual pins used by the thermistor"),
		0, UINT_MAX, 8, G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY);

	g_object_class_install_properties(obj_class, N_PROPS, obj_properties);
}

static void
tllt_thermistor_init(TLLT_UNUSED TlltThermistor *self)
{
#ifdef TLLT_WITH_WIRINGPI
	mcp3004Setup(self->base_pin, self->spi_chan);
#endif
}

TlltThermistor *
tllt_thermistor_new(const int spi_chan, const int base_pin, const unsigned int num_pins)
{
	return g_object_new(TLLT_TYPE_THERMISTOR, "spi-chan", spi_chan, "base-pin", base_pin,
						"number-of-pins", num_pins, NULL);
}

double
tllt_thermistor_reading_to_farenheit(const double reading)
{
	int temperature   = 0;
	double relativity = 0;
	for (int i = 0; i < (int) ARR_SIZE(readings); i++) {
		double r = reading / readings[i][1];
		if (fabs(1 - r) < fabs(1 - relativity)) {
			relativity  = r;
			temperature = readings[i][0];
		}
	}

	return temperature;
}
