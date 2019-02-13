#pragma once

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define TLLT_CP_TYPE_APPLICATION (tllt_cp_application_get_type())
G_DECLARE_FINAL_TYPE(TlltCpApplication, tllt_cp_application, TLLT_CP, APPLICATION, GtkApplication)

TlltCpApplication *tllt_cp_application_new(const char *id);

G_END_DECLS
