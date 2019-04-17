#pragma once

#include <glib-object.h>

#include "components/tllt-heating-element.h"
#include "sensors/tllt-thermistor.h"

G_BEGIN_DECLS

struct _TlltToaster
{
	GObject parent_instance;

	TlltHeatingElement *top_heating_element;
	TlltHeatingElement *bottom_heating_element;
	TlltThermistor *thermistor;
};

#define TLLT_TYPE_TOASTER (tllt_toaster_get_type())
G_DECLARE_FINAL_TYPE(TlltToaster, tllt_toaster, TLLT, TOASTER, GObject)

typedef void (*TlltToasterUpdateFunc)(unsigned int minutes, unsigned int seconds, double progress,
									  gpointer user_data);

TlltToaster *tllt_toaster_new_from_config_file(const char *file_path, GError **err);
void tllt_toaster_start(TlltToaster *toaster, const unsigned int minutes,
						const unsigned int seconds, const int temperature,
						const TlltToasterUpdateFunc update, gpointer user_data);
void tllt_toaster_stop(TlltToaster *toaster);

G_END_DECLS
