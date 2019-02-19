#pragma once

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define TLLT_CP_TYPE_LOGIN_DIALOG (tllt_cp_login_dialog_get_type())
G_DECLARE_FINAL_TYPE(TlltCpLoginDialog, tllt_cp_login_dialog, TLLT_CP, LOGIN_DIALOG, GtkDialog)

TlltCpLoginDialog *tllt_cp_login_dialog_new();
const char *tllt_cp_login_dialog_get_username(TlltCpLoginDialog *self);
const char *tllt_cp_login_dialog_get_password(TlltCpLoginDialog *self);

G_END_DECLS
