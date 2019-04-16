#pragma once

#include <glib-object.h>

#include "tllt-sensor.h"

G_BEGIN_DECLS

struct _TlltScale
{
	TlltSensor parent_instance;

	int gpio_pin;
};

#define TLLT_TYPE_SCALE (tllt_scale_get_type())
G_DECLARE_FINAL_TYPE(TlltScale, tllt_scale, TLLT, SCALE, TlltSensor)

TlltScale *tllt_scale_new(const int gpio_pin);

G_END_DECLS
