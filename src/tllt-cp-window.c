#include <glib/gi18n.h>
#include <gtk/gtk.h>

#include "login/tllt-cp-login-window.h"
#include "tllt-cp-user.h"
#include "tllt-cp-window.h"

struct _TlltCpWindow
{
	GtkApplicationWindow parent_instance;
};

typedef struct TlltCpWindowPrivate
{
	GtkFlowBox *user_profiles_flow_box;
	GtkRevealer *user_actions_revealer;
	GtkRevealer *user_profile_revealer;

	GSList *logged_in_users;
	TlltCpUser *selected_user;
} TlltCpWindowPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(TlltCpWindow, tllt_cp_window, GTK_TYPE_APPLICATION_WINDOW)

static void
tllt_cp_window_user_tile_clicked(GtkButton *widget, gpointer flow_box)
{
	gtk_flow_box_select_child(flow_box,
							  GTK_FLOW_BOX_CHILD(gtk_widget_get_parent(GTK_WIDGET(widget))));
	g_signal_emit_by_name(flow_box, "child-activated",
						  GTK_FLOW_BOX_CHILD(gtk_widget_get_parent(GTK_WIDGET(widget))), NULL);
}

void
tllt_cp_window_add_user(TlltCpWindow *self, const TlltCpUser *user)
{
	TlltCpWindowPrivate *priv = tllt_cp_window_get_instance_private(self);

	GtkWidget *flow_box_child = gtk_flow_box_child_new();
	GtkWidget *user_tile	  = gtk_button_new();
	GtkWidget *internal_box   = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

	gtk_widget_set_halign(flow_box_child, GTK_ALIGN_FILL);
	gtk_widget_set_valign(flow_box_child, GTK_ALIGN_CENTER);

	g_object_connect((gpointer) user_tile, "signal::clicked",
					 G_CALLBACK(tllt_cp_window_user_tile_clicked), priv->user_profiles_flow_box,
					 NULL);

	gtk_box_pack_start(GTK_BOX(internal_box),
					   gtk_image_new_from_icon_name("user-info", GTK_ICON_SIZE_DND), TRUE, TRUE, 0);
	gtk_box_pack_end(GTK_BOX(internal_box), gtk_label_new(user->name), TRUE, TRUE, 0);
	gtk_container_add(GTK_CONTAINER(user_tile), internal_box);
	gtk_container_add(GTK_CONTAINER(flow_box_child), GTK_WIDGET(user_tile));
	gtk_container_add(GTK_CONTAINER(priv->user_profiles_flow_box), flow_box_child);
	gtk_widget_show_all(GTK_WIDGET(priv->user_profiles_flow_box));

	priv->logged_in_users = g_slist_append(priv->logged_in_users, (gpointer) user);
}

static void
on_login_button_clicked(G_GNUC_UNUSED GtkButton *widget, gpointer user_data)
{
	TlltCpWindow *self = TLLT_CP_WINDOW(user_data);

	TlltCpLoginWindow *window = tllt_cp_login_window_new(GTK_WINDOW(self));
	gtk_window_present(GTK_WINDOW(window));
}

static void
remove_children_from_user_profiles_flow_box(gpointer data, gpointer user_data)
{
	gtk_container_remove(GTK_CONTAINER(user_data), GTK_WIDGET(data));
}

static void
on_logout_button_clicked(G_GNUC_UNUSED GtkButton *widget, gpointer user_data)
{
	TlltCpWindow *self		  = TLLT_CP_WINDOW(user_data);
	TlltCpWindowPrivate *priv = tllt_cp_window_get_instance_private(self);

	GtkWidget *dialog = gtk_message_dialog_new(
		GTK_WINDOW(user_data), GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
		GTK_MESSAGE_WARNING, GTK_BUTTONS_YES_NO, _("Are you sure you would like to log %s out?"),
		priv->selected_user->name);
	GtkWidget *button	= gtk_dialog_get_widget_for_response(GTK_DIALOG(dialog), GTK_RESPONSE_YES);
	GtkStyleContext *ctx = gtk_widget_get_style_context(button);
	gtk_style_context_add_class(ctx, GTK_STYLE_CLASS_DESTRUCTIVE_ACTION);

	const int response = gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);

	if (response == GTK_RESPONSE_YES) {
		g_autoptr(GList) list = gtk_flow_box_get_selected_children(priv->user_profiles_flow_box);
		g_warn_if_fail(g_list_length(list) > 0);
		g_list_foreach(list, remove_children_from_user_profiles_flow_box,
					   (gpointer) priv->user_profiles_flow_box);
		priv->logged_in_users = g_slist_remove(priv->logged_in_users, priv->selected_user);
		priv->selected_user   = NULL;
		gtk_revealer_set_reveal_child(priv->user_actions_revealer, FALSE);
		gtk_revealer_set_reveal_child(priv->user_profile_revealer, FALSE);
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
	g_warn_if_fail(priv->selected_user != NULL);

	if (!gtk_revealer_get_child_revealed(priv->user_actions_revealer)) {
		gtk_revealer_set_reveal_child(priv->user_actions_revealer, TRUE);
	}

	if (!gtk_revealer_get_child_revealed(priv->user_profile_revealer)) {
		gtk_revealer_set_reveal_child(priv->user_profile_revealer, TRUE);
	}
}

static void
on_user_actions_revealer_close_clicked(G_GNUC_UNUSED GtkButton *widget, gpointer user_data)
{
	TlltCpWindow *self		  = TLLT_CP_WINDOW(user_data);
	TlltCpWindowPrivate *priv = tllt_cp_window_get_instance_private(self);

	gtk_revealer_set_reveal_child(priv->user_actions_revealer, FALSE);
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
	gtk_widget_class_bind_template_child_private(wid_class, TlltCpWindow, user_profiles_flow_box);
	gtk_widget_class_bind_template_child_private(wid_class, TlltCpWindow, user_actions_revealer);
	gtk_widget_class_bind_template_child_private(wid_class, TlltCpWindow, user_profile_revealer);
	gtk_widget_class_bind_template_callback(wid_class, on_login_button_clicked);
	gtk_widget_class_bind_template_callback(wid_class, on_logout_button_clicked);
	gtk_widget_class_bind_template_callback(wid_class, on_user_details_button_clicked);
	gtk_widget_class_bind_template_callback(wid_class, on_theme_state_changed);
	gtk_widget_class_bind_template_callback(wid_class, on_user_profiles_flow_box_child_activated);
	gtk_widget_class_bind_template_callback(wid_class, on_user_actions_revealer_close_clicked);
}

static void
tllt_cp_window_init(TlltCpWindow *self)
{
	gtk_widget_init_template(GTK_WIDGET(self));
}
