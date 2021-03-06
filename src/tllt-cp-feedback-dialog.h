#pragma once

#include <glib-object.h>
#include <gtk/gtk.h>

#include "tllt-cp.h"

G_BEGIN_DECLS

#define TLLT_CP_TYPE_FEEDBACK_DIALOG (tllt_cp_feedback_dialog_get_type())
G_DECLARE_FINAL_TYPE(TlltCpFeedbackDialog, tllt_cp_feedback_dialog, TLLT_CP, FEEDBACK_DIALOG,
					 GtkDialog)

TlltCpFeedbackDialog *tllt_cp_feedback_dialog_new(GtkWindow *parent, TlltCpUser *user,
												  TlltCpClient *client);

G_END_DECLS
