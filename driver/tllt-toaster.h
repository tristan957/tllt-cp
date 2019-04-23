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

typedef void (*TlltToasterUpdateFunc)(unsigned int time, double progress, gpointer user_data);

TlltToaster *tllt_toaster_new_from_config_file(const char *file_path, GError **err);
void tllt_toaster_start(TlltToaster *self, const unsigned int time, const int temperature,
						const TlltToasterUpdateFunc update, gpointer user_data);
void tllt_toaster_stop(TlltToaster *self);
gboolean tllt_toaster_is_running(TlltToaster *self);

G_END_DECLS
