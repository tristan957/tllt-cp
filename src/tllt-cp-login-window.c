#include <string.h>

#include <glib/gi18n.h>
#include <gtk/gtk.h>

#include "tllt-cp-login-window.h"
#include "tllt-cp-window.h"
#include "tllt-cp.h"

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
	GtkRevealer *login_info_revealer;
	GtkRevealer *new_user_info_revealer;
	GtkLabel *new_user_info_label;

	TlltCpClient *client;
} TlltCpLoginWindowPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(TlltCpLoginWindow, tllt_cp_login_window, GTK_TYPE_WINDOW)

typedef enum TlltCpLoginWindowProps
{
	PROP_CLIENT = 1,
	N_PROPS,
} TlltCpLoginWindowProps;

static GParamSpec *obj_properties[N_PROPS];

static void
login_user(TlltCpLoginWindow *self)
{
	TlltCpLoginWindowPrivate *priv = tllt_cp_login_window_get_instance_private(self);

	if (gtk_revealer_get_child_revealed(priv->login_info_revealer)) {
		gtk_revealer_set_reveal_child(priv->login_info_revealer, FALSE);
	}

	const char *email	= gtk_entry_get_text(priv->login_email_entry);
	const char *password = gtk_entry_get_text(priv->login_password_entry);

	g_autoptr(GError) err = NULL;
	TlltCpUser *user	  = tllt_cp_user_authenticate(priv->client, email, password, &err);
	if (err != NULL) {
		if (!gtk_revealer_get_child_revealed(priv->login_info_revealer)) {
			gtk_revealer_set_reveal_child(priv->login_info_revealer, TRUE);
		}

		return;
	}

	tllt_cp_window_add_user(TLLT_CP_WINDOW(gtk_window_get_transient_for(GTK_WINDOW(self))), user);
	gtk_window_close(GTK_WINDOW(self));
}

static void
create_user(TlltCpLoginWindow *self)
{
	TlltCpLoginWindowPrivate *priv = tllt_cp_login_window_get_instance_private(self);

	if (gtk_revealer_get_child_revealed(priv->new_user_info_revealer)) {
		gtk_revealer_set_reveal_child(priv->new_user_info_revealer, FALSE);
	}

	/**
	 * GtkEntry expects its internal string to not be modified, so we duplicate it and strip the
	 * whitespace
	 */
	g_autofree char *name   = g_strstrip(g_strdup(gtk_entry_get_text(priv->new_user_name_entry)));
	g_autofree char *email  = g_strstrip(g_strdup(gtk_entry_get_text(priv->new_user_email_entry)));
	const char *password	= gtk_entry_get_text(priv->new_user_password_entry);
	const char *re_password = gtk_entry_get_text(priv->new_user_re_password_entry);

	if (strlen(name) == 0) {
		gtk_label_set_label(priv->new_user_info_label, "Name cannot be empty");
		if (!gtk_revealer_get_child_revealed(priv->new_user_info_revealer)) {
			gtk_revealer_set_reveal_child(priv->new_user_info_revealer, TRUE);
		}

		return;
	}

	if (strlen(email) == 0) {
		// TODO: regex check for valid email?
		gtk_label_set_label(priv->new_user_info_label, "Email cannot be empty");
		if (!gtk_revealer_get_child_revealed(priv->new_user_info_revealer)) {
			gtk_revealer_set_reveal_child(priv->new_user_info_revealer, TRUE);
		}

		return;
	}

	if (g_strcmp0(password, re_password) != 0) {
		gtk_label_set_label(priv->new_user_info_label, "Passwords do not match");
		if (!gtk_revealer_get_child_revealed(priv->new_user_info_revealer)) {
			gtk_revealer_set_reveal_child(priv->new_user_info_revealer, TRUE);
		}

		return;
	}

	g_autoptr(GError) err = NULL;
	TlltCpUser *user	  = tllt_cp_user_create(priv->client, name, email, password, &err);
	if (err != NULL) {
		g_autoptr(GString) message = g_string_new(NULL);
		g_string_printf(message, "Error creating new user: %s", err->message);
		gtk_label_set_label(priv->new_user_info_label, message->str);
		if (!gtk_revealer_get_child_revealed(priv->new_user_info_revealer)) {
			gtk_revealer_set_reveal_child(priv->new_user_info_revealer, TRUE);
		}

		return;
	}

	tllt_cp_window_add_user(TLLT_CP_WINDOW(gtk_window_get_transient_for(GTK_WINDOW(self))), user);
	gtk_window_close(GTK_WINDOW(self));
}

static void
on_login_button_clicked(G_GNUC_UNUSED GtkButton *button, gpointer user_data)
{
	login_user(TLLT_CP_LOGIN_WINDOW(user_data));
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
	create_user(TLLT_CP_LOGIN_WINDOW(user_data));
}

static void
on_login_password_entry_activate(G_GNUC_UNUSED GtkEntry *widget, gpointer user_data)
{
	login_user(TLLT_CP_LOGIN_WINDOW(user_data));
}

static void
on_new_user_re_password_entry_activate(G_GNUC_UNUSED GtkEntry *widget, gpointer user_data)
{
	create_user(TLLT_CP_LOGIN_WINDOW(user_data));
}

TlltCpLoginWindow *
tllt_cp_login_window_new(const GtkWindow *parent, TlltCpClient *client)
{
	return g_object_new(TLLT_CP_TYPE_LOGIN_WINDOW, "transient-for", GTK_WINDOW(parent), "client",
						client, NULL);
}

static void
tllt_cp_login_window_get_property(GObject *obj, guint prop_id, GValue *val, GParamSpec *pspec)
{
	TlltCpLoginWindow *self		   = TLLT_CP_LOGIN_WINDOW(obj);
	TlltCpLoginWindowPrivate *priv = tllt_cp_login_window_get_instance_private(self);

	switch (prop_id) {
	case PROP_CLIENT:
		g_value_set_object(val, priv->client);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
	}
}

static void
tllt_cp_login_window_set_property(GObject *obj, guint prop_id, const GValue *val, GParamSpec *pspec)
{
	TlltCpLoginWindow *self		   = TLLT_CP_LOGIN_WINDOW(obj);
	TlltCpLoginWindowPrivate *priv = tllt_cp_login_window_get_instance_private(self);

	switch (prop_id) {
	case PROP_CLIENT:
		priv->client = g_value_get_object(val);
		g_object_ref(priv->client);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
	}
}

static void
tllt_cp_login_window_finalize(GObject *obj)
{
	TlltCpLoginWindow *self		   = TLLT_CP_LOGIN_WINDOW(obj);
	TlltCpLoginWindowPrivate *priv = tllt_cp_login_window_get_instance_private(self);

	g_object_unref(priv->client);

	G_OBJECT_CLASS(tllt_cp_login_window_parent_class)->finalize(obj);
}

static void
tllt_cp_login_window_class_init(TlltCpLoginWindowClass *klass)
{
	GObjectClass *obj_class   = G_OBJECT_CLASS(klass);
	GtkWidgetClass *wid_class = GTK_WIDGET_CLASS(klass);

	obj_class->finalize		= tllt_cp_login_window_finalize;
	obj_class->get_property = tllt_cp_login_window_get_property;
	obj_class->set_property = tllt_cp_login_window_set_property;

	obj_properties[PROP_CLIENT] = g_param_spec_object(
		"client", _("Client"), _("Client to connect to server"), TLLT_CP_TYPE_CLIENT,
		G_PARAM_CONSTRUCT_ONLY | G_PARAM_PRIVATE | G_PARAM_READWRITE);

	g_object_class_install_properties(obj_class, N_PROPS, obj_properties);

	gtk_widget_class_set_template_from_resource(
		wid_class, "/ht/sr/git/tristan957/tllt-cp/ui/tllt-cp-login-window.ui");
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
	gtk_widget_class_bind_template_child_private(wid_class, TlltCpLoginWindow, login_info_revealer);
	gtk_widget_class_bind_template_child_private(wid_class, TlltCpLoginWindow,
												 new_user_info_revealer);
	gtk_widget_class_bind_template_child_private(wid_class, TlltCpLoginWindow, new_user_info_label);
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
