#include <gtk/gtk.h>

#include "tllt-cp-login-window.h"

struct _TlltCpLoginWindow
{
	GtkDialog parent_instance;
};

typedef struct TlltCpLoginWindowPrivate
{
	GtkStack *login_stack;
	GtkEntry *login_email_entry;
	GtkEntry *login_password_entry;
} TlltCpLoginWindowPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(TlltCpLoginWindow, tllt_cp_login_window, GTK_TYPE_WINDOW)

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

static void
on_new_user_button_clicked(GtkButton *widget, gpointer user_data)
{
	TlltCpLoginWindow *self		   = TLLT_CP_LOGIN_WINDOW(user_data);
	TlltCpLoginWindowPrivate *priv = tllt_cp_login_window_get_instance_private(self);
	gtk_widget_hide(GTK_WIDGET(widget));
	gtk_stack_set_visible_child_name(priv->login_stack, "new_user_page");
}

const char *
tllt_cp_login_window_get_email(TlltCpLoginWindow *self)
{
	TlltCpLoginWindowPrivate *priv = tllt_cp_login_window_get_instance_private(self);
	return gtk_entry_get_text(priv->login_email_entry);
}

const char *
tllt_cp_login_window_get_password(TlltCpLoginWindow *self)
{
	TlltCpLoginWindowPrivate *priv = tllt_cp_login_window_get_instance_private(self);
	return gtk_entry_get_text(priv->login_password_entry);
}

TlltCpLoginWindow *
tllt_cp_login_window_new()
{
	return g_object_new(TLLT_CP_TYPE_LOGIN_WINDOW, NULL);
}

static void
tllt_cp_login_window_finalize(GObject *object)
{
	G_OBJECT_CLASS(tllt_cp_login_window_parent_class)->finalize(object);
}

static void
tllt_cp_login_window_class_init(TlltCpLoginWindowClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	GtkWidgetClass *wid_class  = GTK_WIDGET_CLASS(klass);

	object_class->finalize = tllt_cp_login_window_finalize;

	gtk_widget_class_set_template_from_resource(
		wid_class, "/com/gitlab/tristan957/TlltCp/login/tllt-cp-login-window.ui");
	gtk_widget_class_bind_template_child_private(wid_class, TlltCpLoginWindow, login_stack);
	gtk_widget_class_bind_template_child_private(wid_class, TlltCpLoginWindow, login_email_entry);
	gtk_widget_class_bind_template_child_private(wid_class, TlltCpLoginWindow,
												 login_password_entry);
	gtk_widget_class_bind_template_callback(wid_class, on_new_user_button_clicked);
	gtk_widget_class_bind_template_callback(wid_class, on_login_button_clicked);
	gtk_widget_class_bind_template_callback(wid_class, on_cancel_button_clicked);
}

static void
tllt_cp_login_window_init(TlltCpLoginWindow *self)
{
	gtk_widget_init_template(GTK_WIDGET(self));
}
