#pragma once

#include <glib-object.h>

G_BEGIN_DECLS

#define TLLT_TYPE_TOASTER (tllt_toaster_get_type())
G_DECLARE_FINAL_TYPE(TlltToaster, tllt_toaster, TLLT, TOASTER, GObject)

typedef void (*TlltToasterUpdateFunc)(unsigned int minutes, unsigned int seconds, double progress,
									  gpointer user_data);

TlltToaster *tllt_toaster_new_from_file(const gchar *file_name, GError **err);
void tllt_toaster_start(TlltToaster *toaster, const TlltToasterUpdateFunc update,
						gpointer user_data);
void tllt_toaster_start_with_time(TlltToaster *toaster, const unsigned int minutes,
								  const unsigned int seconds, const unsigned int temperature,
								  const TlltToasterUpdateFunc update, gpointer user_data);
void tllt_toaster_stop(TlltToaster *toaster);

G_END_DECLS
