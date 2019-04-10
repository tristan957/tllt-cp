#pragma once

#include <gtk/gtk.h>

#include "tllt-cp.h"

G_BEGIN_DECLS

#define TLLT_CP_TYPE_WINDOW (tllt_cp_window_get_type())
G_DECLARE_FINAL_TYPE(TlltCpWindow, tllt_cp_window, TLLT_CP, WINDOW, GtkApplicationWindow)

TlltCpWindow *tllt_cp_window_new(GApplication *app);
void tllt_cp_window_add_user(TlltCpWindow *self, TlltCpUser *user);

G_END_DECLS
