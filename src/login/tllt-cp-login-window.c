#include <glib/gi18n.h>
#include <gtk/gtk.h>

#include "tllt-cp-login-window.h"
#include "tllt-cp-user.h"
#include "tllt-cp-window.h"

struct _TlltCpLoginWindow
{
	GtkDialog parent_instance;
};

typedef struct TlltCpLoginWindowPrivate
{
	GtkHeaderBar *header_bar;
	GtkStack *login_stack;
	GtkEntry *login_email_entry;
	GtkEntry *login_password_entry;
	GtkEntry *new_user_name_entry;
	GtkEntry *new_user_email_entry;
	GtkEntry *new_user_password_entry;
	GtkEntry *new_user_re_password_entry;
} TlltCpLoginWindowPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(TlltCpLoginWindow, tllt_cp_login_window, GTK_TYPE_WINDOW)

static void
on_login_button_clicked(G_GNUC_UNUSED GtkButton *button, gpointer user_data)
{
	TlltCpLoginWindow *self = TLLT_CP_LOGIN_WINDOW(user_data);

	tllt_cp_window_add_user(TLLT_CP_WINDOW(gtk_window_get_transient_for(GTK_WINDOW(self))),
							tllt_cp_user_new("Tristan Partin", "tristan.partin@your_mom.com", 1));
	gtk_window_close(GTK_WINDOW(self));
}

static void
on_cancel_button_clicked(G_GNUC_UNUSED GtkButton *widget, gpointer user_data)
{
	gtk_window_close(GTK_WINDOW(user_data));
}

static void
on_new_user_button_clicked(GtkButton *widget, gpointer user_data)
{
	TlltCpLoginWindow *self		   = TLLT_CP_LOGIN_WINDOW(user_data);
	TlltCpLoginWindowPrivate *priv = tllt_cp_login_window_get_instance_private(self);

	gtk_header_bar_set_title(priv->header_bar, _("Sign Up"));
	gtk_widget_hide(GTK_WIDGET(widget));
	gtk_widget_set_can_focus(GTK_WIDGET(widget), FALSE);
	gtk_stack_set_visible_child_name(priv->login_stack, "new_user_page");
	gtk_widget_grab_focus(GTK_WIDGET(priv->new_user_name_entry));
}

static void
on_create_button_clicked(G_GNUC_UNUSED GtkButton *widget, gpointer user_data)
{
	TlltCpLoginWindow *self = TLLT_CP_LOGIN_WINDOW(user_data);

	tllt_cp_window_add_user(TLLT_CP_WINDOW(gtk_window_get_transient_for(GTK_WINDOW(self))),
							tllt_cp_user_new("Tristan Partin", "tristan.partin@your_mom.com", 2));
	gtk_window_close(GTK_WINDOW(self));
}

static void
on_login_password_entry_activate(G_GNUC_UNUSED GtkEntry *widget, G_GNUC_UNUSED gpointer user_data)
{}

static void
on_new_user_re_password_entry_activate(G_GNUC_UNUSED GtkEntry *widget,
									   G_GNUC_UNUSED gpointer user_data)
{}

TlltCpLoginWindow *
tllt_cp_login_window_new(const GtkWindow *parent)
{
	return g_object_new(TLLT_CP_TYPE_LOGIN_WINDOW, "transient-for", GTK_WINDOW(parent), NULL);
}

static void
tllt_cp_login_window_finalize(GObject *object)
{
	G_OBJECT_CLASS(tllt_cp_login_window_parent_class)->finalize(object);
}

static void
tllt_cp_login_window_class_init(TlltCpLoginWindowClass *klass)
{
	GObjectClass *obj_class   = G_OBJECT_CLASS(klass);
	GtkWidgetClass *wid_class = GTK_WIDGET_CLASS(klass);

	obj_class->finalize = tllt_cp_login_window_finalize;

	gtk_widget_class_set_template_from_resource(
		wid_class, "/com/gitlab/tristan957/tllt-cp/login/tllt-cp-login-window.ui");
	gtk_widget_class_bind_template_child_private(wid_class, TlltCpLoginWindow, header_bar);
	gtk_widget_class_bind_template_child_private(wid_class, TlltCpLoginWindow, login_stack);
	gtk_widget_class_bind_template_child_private(wid_class, TlltCpLoginWindow, login_email_entry);
	gtk_widget_class_bind_template_child_private(wid_class, TlltCpLoginWindow,
												 login_password_entry);
	gtk_widget_class_bind_template_child_private(wid_class, TlltCpLoginWindow, new_user_name_entry);
	gtk_widget_class_bind_template_child_private(wid_class, TlltCpLoginWindow,
												 new_user_email_entry);
	gtk_widget_class_bind_template_child_private(wid_class, TlltCpLoginWindow,
												 new_user_password_entry);
	gtk_widget_class_bind_template_child_private(wid_class, TlltCpLoginWindow,
												 new_user_re_password_entry);
	gtk_widget_class_bind_template_callback(wid_class, on_new_user_button_clicked);
	gtk_widget_class_bind_template_callback(wid_class, on_login_button_clicked);
	gtk_widget_class_bind_template_callback(wid_class, on_cancel_button_clicked);
	gtk_widget_class_bind_template_callback(wid_class, on_login_password_entry_activate);
	gtk_widget_class_bind_template_callback(wid_class, on_new_user_re_password_entry_activate);
	gtk_widget_class_bind_template_callback(wid_class, on_create_button_clicked);
}

static void
tllt_cp_login_window_init(TlltCpLoginWindow *self)
{
	gtk_widget_init_template(GTK_WIDGET(self));
}
