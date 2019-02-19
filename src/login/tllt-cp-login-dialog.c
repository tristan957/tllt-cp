#include <gtk/gtk.h>

#include "tllt-cp-login-dialog.h"

struct _TlltCpLoginDialog {
	GtkDialog parent_instance;
};

typedef struct TlltCpLoginDialogPrivate {
	GtkEntry *username_entry;
	GtkEntry *password_entry;
} TlltCpLoginDialogPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(TlltCpLoginDialog, tllt_cp_login_dialog, GTK_TYPE_DIALOG)

static void
on_login_button_clicked(G_GNUC_UNUSED GtkButton *button, gpointer user_data)
{
	gtk_window_close(GTK_WINDOW(user_data));
}

static void
on_cancel_button_clicked(G_GNUC_UNUSED GtkButton *button, gpointer user_data)
{
	gtk_window_close(GTK_WINDOW(user_data));
}

const char *
tllt_cp_login_dialog_get_username(TlltCpLoginDialog *self)
{
	TlltCpLoginDialogPrivate *priv = tllt_cp_login_dialog_get_instance_private(self);
	return gtk_entry_get_text(priv->username_entry);
}

const char *
tllt_cp_login_dialog_get_password(TlltCpLoginDialog *self)
{
	TlltCpLoginDialogPrivate *priv = tllt_cp_login_dialog_get_instance_private(self);
	return gtk_entry_get_text(priv->password_entry);
}

TlltCpLoginDialog *
tllt_cp_login_dialog_new()
{
	return g_object_new(TLLT_CP_TYPE_LOGIN_DIALOG, "use-header-bar", FALSE, NULL);
}

static void
tllt_cp_login_dialog_finalize(GObject *object)
{
	G_OBJECT_CLASS(tllt_cp_login_dialog_parent_class)->finalize(object);
}

static void
tllt_cp_login_dialog_class_init(TlltCpLoginDialogClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	GtkWidgetClass *wid_class  = GTK_WIDGET_CLASS(klass);

	object_class->finalize = tllt_cp_login_dialog_finalize;

	gtk_widget_class_set_template_from_resource(
		wid_class, "/com/gitlab/tristan957/TlltCp/login/tllt-cp-login-dialog.ui");
	gtk_widget_class_bind_template_child_private(wid_class, TlltCpLoginDialog, username_entry);
	gtk_widget_class_bind_template_child_private(wid_class, TlltCpLoginDialog, password_entry);
	gtk_widget_class_bind_template_callback(wid_class, on_login_button_clicked);
	gtk_widget_class_bind_template_callback(wid_class, on_cancel_button_clicked);
}

static void
tllt_cp_login_dialog_init(TlltCpLoginDialog *self)
{
	gtk_widget_init_template(GTK_WIDGET(self));
}
