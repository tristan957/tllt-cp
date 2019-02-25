#include <glib/gi18n.h>
#include <gtk/gtk.h>

#include "login/tllt-cp-login-dialog.h"
#include "tllt-cp-user.h"
#include "tllt-cp-window.h"

#define LOGOUT_YES -8
#define LOGOUT_NO -9
#define LOGIN_YES -3

struct _TlltCpWindow
{
	GtkApplicationWindow parent_instance;
};

typedef struct TlltCpWindowPrivate
{
	GtkButton *login_button;
	GtkButton *logout_button;
	GtkButton *user_details_button;
	GtkFlowBox *user_profiles_flow_box;
	GtkRevealer *user_actions_revealer;
	GtkRevealer *user_profile_revealer;

	GSList *logged_in_users;
	TlltCpUser *selected_user;
} TlltCpWindowPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(TlltCpWindow, tllt_cp_window, GTK_TYPE_APPLICATION_WINDOW)

static void
user_flow_box_add_user(const GtkFlowBox *self, const TlltCpUser *user)
{
	GtkWidget *lab = gtk_label_new(user->name);
	gtk_container_add(GTK_CONTAINER(self), lab);
	gtk_widget_show_all(GTK_WIDGET(self));
}

static void
on_login_button_clicked(G_GNUC_UNUSED GtkButton *widget, gpointer user_data)
{
	static int i			  = 1;
	TlltCpWindow *self		  = TLLT_CP_WINDOW(user_data);
	TlltCpWindowPrivate *priv = tllt_cp_window_get_instance_private(self);

	TlltCpLoginDialog *dialog = tllt_cp_login_dialog_new();
	gtk_window_set_transient_for(GTK_WINDOW(dialog), GTK_WINDOW(self));
	const int response = gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(GTK_WIDGET(dialog));

	if (response == LOGIN_YES) {
		const TlltCpUser *user =
			tllt_cp_user_new("Tristan Partin", "tristan.partin@your_mom.com", i++);
		priv->logged_in_users = g_slist_append(priv->logged_in_users, (gpointer) user);
		user_flow_box_add_user(priv->user_profiles_flow_box, user);
	}
}

static void
on_logout_button_clicked(G_GNUC_UNUSED GtkButton *widget, gpointer user_data)
{
	g_autoptr(GString) message = g_string_new(NULL);
	g_string_sprintf(message, _("Are you sure you would like to log %s out?"), "Tristan Partin");
	GtkWidget *dialog = gtk_message_dialog_new(
		GTK_WINDOW(user_data), GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
		GTK_MESSAGE_WARNING, GTK_BUTTONS_YES_NO, message->str);
	const int response = gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);

	if (response == LOGOUT_YES) {
	}
}

static void
on_user_details_button_clicked(GtkButton *widget, G_GNUC_UNUSED gpointer user_data)
{
	g_print("%s\n", gtk_button_get_label(widget));
}

static void
on_theme_state_changed(GtkToggleButton *widget, G_GNUC_UNUSED gpointer user_data)
{
	g_object_set(gtk_settings_get_default(), "gtk-application-prefer-dark-theme",
				 gtk_toggle_button_get_active(widget), NULL);
}

static void
on_user_profiles_flow_box_child_activated(G_GNUC_UNUSED GtkFlowBox *widget, GtkFlowBoxChild *child,
										  gpointer user_data)
{
	TlltCpWindow *self		  = TLLT_CP_WINDOW(user_data);
	TlltCpWindowPrivate *priv = tllt_cp_window_get_instance_private(self);
	const gint index		  = gtk_flow_box_child_get_index(child);
	priv->selected_user		  = TLLT_CP_USER(g_slist_nth_data(priv->logged_in_users, index));

	if (!gtk_revealer_get_child_revealed(priv->user_actions_revealer)) {
		gtk_revealer_set_reveal_child(priv->user_actions_revealer, TRUE);
	}

	if (!gtk_revealer_get_child_revealed(priv->user_profile_revealer)) {
		gtk_revealer_set_reveal_child(priv->user_profile_revealer, TRUE);
	}
}

TlltCpWindow *
tllt_cp_window_new(GApplication *app)
{
	return g_object_new(TLLT_CP_TYPE_WINDOW, "application", app, NULL);
}

static void
tllt_cp_window_finalize(GObject *object)
{
	TlltCpWindowPrivate *priv = tllt_cp_window_get_instance_private(TLLT_CP_WINDOW(object));
	g_slist_free(priv->logged_in_users);

	G_OBJECT_CLASS(tllt_cp_window_parent_class)->finalize(object);
}

static void
tllt_cp_window_class_init(TlltCpWindowClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	GtkWidgetClass *wid_class  = GTK_WIDGET_CLASS(klass);

	object_class->finalize = tllt_cp_window_finalize;

	gtk_widget_class_set_template_from_resource(wid_class,
												"/com/gitlab/tristan957/TlltCp/tllt-cp-window.ui");
	gtk_widget_class_bind_template_child_private(wid_class, TlltCpWindow, login_button);
	gtk_widget_class_bind_template_child_private(wid_class, TlltCpWindow, logout_button);
	gtk_widget_class_bind_template_child_private(wid_class, TlltCpWindow, user_details_button);
	gtk_widget_class_bind_template_child_private(wid_class, TlltCpWindow, user_profiles_flow_box);
	gtk_widget_class_bind_template_child_private(wid_class, TlltCpWindow, user_actions_revealer);
	gtk_widget_class_bind_template_child_private(wid_class, TlltCpWindow, user_profile_revealer);
	gtk_widget_class_bind_template_callback(wid_class, on_login_button_clicked);
	gtk_widget_class_bind_template_callback(wid_class, on_logout_button_clicked);
	gtk_widget_class_bind_template_callback(wid_class, on_user_details_button_clicked);
	gtk_widget_class_bind_template_callback(wid_class, on_theme_state_changed);
	gtk_widget_class_bind_template_callback(wid_class, on_user_profiles_flow_box_child_activated);
}

static void
tllt_cp_window_init(TlltCpWindow *self)
{
	TlltCpWindowPrivate *priv = tllt_cp_window_get_instance_private(self);
	priv->logged_in_users	 = g_slist_alloc();

	gtk_widget_init_template(GTK_WIDGET(self));
}
