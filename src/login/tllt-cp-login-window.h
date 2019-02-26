#pragma once

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define TLLT_CP_TYPE_LOGIN_WINDOW (tllt_cp_login_window_get_type())
G_DECLARE_FINAL_TYPE(TlltCpLoginWindow, tllt_cp_login_window, TLLT_CP, LOGIN_WINDOW, GtkWindow)

TlltCpLoginWindow *tllt_cp_login_window_new();
const char *tllt_cp_login_window_get_username(TlltCpLoginWindow *self);
const char *tllt_cp_login_window_get_password(TlltCpLoginWindow *self);

G_END_DECLS
