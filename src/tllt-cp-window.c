#include <stdio.h>

#include <glib/gi18n.h>
#include <gtk/gtk.h>

#include "tllt-cp-login-window.h"
#include "tllt-cp-new-recipe-window.h"
#include "tllt-cp-recipe-list-item.h"
#include "tllt-cp-window.h"
#include "tllt-cp.h"
#include "tllt-toaster.h"

struct _TlltCpWindow
{
	GtkApplicationWindow parent_instance;
};

typedef struct TlltCpWindowPrivate
{
	GtkCssProvider *css_provider;
	GtkFlowBox *user_profiles_flow_box;
	GtkRevealer *user_actions_revealer;
	GtkRevealer *recipe_revealer;
	GtkBox *toasting_status_box;
	GtkProgressBar *toasting_progress_bar;
	GtkStack *manual_stack;
	GtkLabel *timer_minutes_label;
	GtkLabel *timer_seconds_label;
	GtkSpinButton *timer_minutes_spin_button;
	GtkSpinButton *timer_seconds_spin_button;
	GtkButton *timer_reset_button;
	GtkScale *temperature_scale;
	GtkListBox *recipe_list_box;
	GtkStack *status_stack;
	GtkSpinner *preparation_spinner;
	GtkSwitch *top_heating_element_switch;
	GtkSwitch *bottom_heating_element_switch;
	GtkButton *manual_start_button;

	TlltToaster *toaster;
	GSList *logged_in_users;
	TlltCpClient *client;
	TlltCpUser *selected_user;
	TlltCpUser *toaster_user;
	TlltCpRecipe *currently_running_recipe;
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

static void
recipe_list_box_toggle_run_buttons(GtkWidget *widget, gpointer user_data)
{
	TlltCpRecipeListItem *list_item = TLLT_CP_RECIPE_LIST_ITEM(gtk_bin_get_child(GTK_BIN(widget)));
	tllt_cp_recipe_list_item_toggle_run_button(list_item, *(gboolean *) user_data);
}

static void
on_login_button_clicked(G_GNUC_UNUSED GtkButton *widget, gpointer user_data)
{
	TlltCpWindow *self		  = TLLT_CP_WINDOW(user_data);
	TlltCpWindowPrivate *priv = tllt_cp_window_get_instance_private(self);

	TlltCpLoginWindow *window = tllt_cp_login_window_new(GTK_WINDOW(self), priv->client);
	gtk_window_present(GTK_WINDOW(window));
}

static void
remove_children_from_user_profiles_flow_box(gpointer data, gpointer user_data)
{
	gtk_container_remove(GTK_CONTAINER(user_data), GTK_WIDGET(data));
}

static void
recipe_list_box_remove_all(GtkWidget *widget, gpointer data)
{
	gtk_container_remove(GTK_CONTAINER(data), widget);
}

static void
on_logout_button_clicked(G_GNUC_UNUSED GtkButton *widget, gpointer user_data)
{
	TlltCpWindow *self		  = TLLT_CP_WINDOW(user_data);
	TlltCpWindowPrivate *priv = tllt_cp_window_get_instance_private(self);

	GtkWidget *dialog = gtk_message_dialog_new(
		GTK_WINDOW(user_data), GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
		GTK_MESSAGE_WARNING, GTK_BUTTONS_YES_NO, _("Are you sure you would like to log %s out?"),
		tllt_cp_user_get_name(priv->selected_user));
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
		gtk_revealer_set_reveal_child(priv->recipe_revealer, FALSE);

		GtkContainer *container = GTK_CONTAINER(priv->recipe_list_box);
		gtk_container_foreach(container, recipe_list_box_remove_all, container);
	}
}

static void
on_user_details_button_clicked(GtkButton *widget, G_GNUC_UNUSED gpointer user_data)
{
	g_print("%s\n", gtk_button_get_label(widget));
}

static void
on_theme_state_changed(GtkCheckButton *widget, G_GNUC_UNUSED gpointer user_data)
{
	g_object_set(gtk_settings_get_default(), "gtk-application-prefer-dark-theme",
				 gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget)), NULL);
}

static void
on_user_actions_revealer_close_clicked(G_GNUC_UNUSED GtkButton *widget, gpointer user_data)
{
	TlltCpWindow *self		  = TLLT_CP_WINDOW(user_data);
	TlltCpWindowPrivate *priv = tllt_cp_window_get_instance_private(self);

	gtk_revealer_set_reveal_child(priv->user_actions_revealer, FALSE);
}

static void
on_recipe_revealer_new_button_clicked(G_GNUC_UNUSED GtkButton *widget, gpointer user_data)
{
	TlltCpWindow *self		  = TLLT_CP_WINDOW(user_data);
	TlltCpWindowPrivate *priv = tllt_cp_window_get_instance_private(self);

	TlltCpNewRecipeWindow *recipe_window =
		tllt_cp_new_recipe_window_new(GTK_WINDOW(self), priv->client, priv->selected_user);
	gtk_window_present(GTK_WINDOW(recipe_window));
}

static void
on_recipe_revealer_close_button_clicked(G_GNUC_UNUSED GtkButton *widget, gpointer user_data)
{
	GtkRevealer *recipe_revealer = GTK_REVEALER(user_data);

	if (gtk_revealer_get_child_revealed(recipe_revealer)) {
		gtk_revealer_set_reveal_child(recipe_revealer, FALSE);
	}
}

static gboolean
show_leading_zeros(GtkSpinButton *widget, G_GNUC_UNUSED gpointer user_data)
{
	char buffer[3];
	sprintf(buffer, "%02d", gtk_spin_button_get_value_as_int(widget));
	gtk_entry_set_text(GTK_ENTRY(widget), buffer);

	return TRUE;
}

static void
update_timer(const unsigned int time, const double progress, gpointer user_data)
{
	TlltCpWindow *self		  = TLLT_CP_WINDOW(user_data);
	TlltCpWindowPrivate *priv = tllt_cp_window_get_instance_private(self);

	const unsigned int minutes = time / 60;
	const unsigned int seconds = time % 60;

	char buffer[9];
	sprintf(buffer, "%02u", minutes);
	gtk_label_set_label(priv->timer_minutes_label, buffer);
	sprintf(buffer, "%02u", seconds);
	gtk_label_set_label(priv->timer_seconds_label, buffer);
	gtk_progress_bar_set_fraction(priv->toasting_progress_bar, progress);
}

static void
on_recipe_started(G_GNUC_UNUSED TlltCpRecipeListItem *list_item, TlltCpRecipe *recipe,
				  gpointer user_data)
{
	TlltCpWindow *self		  = TLLT_CP_WINDOW(user_data);
	TlltCpWindowPrivate *priv = tllt_cp_window_get_instance_private(self);

	priv->toaster_user			   = priv->selected_user;
	priv->currently_running_recipe = recipe;

	g_autoptr(GError) err				   = NULL;
	g_autoptr(TlltCpCookingDetailsDto) dto = tllt_cp_user_get_cooking_details_for_recipe(
		priv->toaster_user, priv->client, priv->currently_running_recipe, &err);

	if (dto != NULL) {
		g_object_ref(self);
		tllt_toaster_start(priv->toaster, dto->time, dto->temperature, update_timer, self);
	} else {
		priv->toaster_user			   = NULL;
		priv->currently_running_recipe = NULL;
	}
}

static void
on_user_recipe_added(G_GNUC_UNUSED TlltCpUser *user, TlltCpRecipe *recipe, gpointer user_data)
{
	TlltCpWindow *self		  = TLLT_CP_WINDOW(user_data);
	TlltCpWindowPrivate *priv = tllt_cp_window_get_instance_private(self);

	TlltCpRecipeListItem *list_item = tllt_cp_recipe_list_item_new(recipe);
	g_object_connect(list_item, "signal::recipe-started", G_CALLBACK(on_recipe_started), self,
					 NULL);

	gtk_container_add(GTK_CONTAINER(priv->recipe_list_box), GTK_WIDGET(list_item));
}

void
tllt_cp_window_add_user(G_GNUC_UNUSED TlltCpWindow *self, TlltCpUser *user)
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
	gtk_box_pack_end(GTK_BOX(internal_box), gtk_label_new(tllt_cp_user_get_name(user)), TRUE, TRUE,
					 0);
	gtk_container_add(GTK_CONTAINER(user_tile), internal_box);
	gtk_container_add(GTK_CONTAINER(flow_box_child), user_tile);
	gtk_container_add(GTK_CONTAINER(priv->user_profiles_flow_box), flow_box_child);
	gtk_widget_show_all(GTK_WIDGET(priv->user_profiles_flow_box));

	g_object_connect(user, "signal::recipe-added", G_CALLBACK(on_user_recipe_added), self, NULL);

	priv->logged_in_users = g_slist_append(priv->logged_in_users, (gpointer) user);
}

static void
on_user_profiles_flow_box_child_activated(G_GNUC_UNUSED GtkFlowBox *widget, GtkFlowBoxChild *child,
										  gpointer user_data)
{
	TlltCpWindow *self		  = TLLT_CP_WINDOW(user_data);
	TlltCpWindowPrivate *priv = tllt_cp_window_get_instance_private(self);

	const gint index		  = gtk_flow_box_child_get_index(child);
	TlltCpUser *prev_selected = priv->selected_user;
	priv->selected_user		  = TLLT_CP_USER(g_slist_nth_data(priv->logged_in_users, index));
	g_return_if_fail(priv->selected_user != NULL);

	// Block the refresh for performance reasons if you click the same person
	if (prev_selected == NULL ||
		tllt_cp_user_get_id(prev_selected) != tllt_cp_user_get_id(priv->selected_user)) {
		GList *recipes = tllt_cp_user_get_recipes(priv->selected_user);

		GtkContainer *container = GTK_CONTAINER(priv->recipe_list_box);
		gtk_container_foreach(container, recipe_list_box_remove_all, container);

		for (guint i = 0; i < g_list_length(recipes); i++) {
			TlltCpRecipeListItem *list_item =
				tllt_cp_recipe_list_item_new(TLLT_CP_RECIPE(g_list_nth(recipes, i)->data));
			if (tllt_toaster_is_running(priv->toaster)) {
				tllt_cp_recipe_list_item_toggle_run_button(list_item, FALSE);
			}

			g_object_connect(list_item, "signal::recipe-started", G_CALLBACK(on_recipe_started),
							 self, NULL);
			gtk_container_add(container, GTK_WIDGET(list_item));
		}
	}

	if (!gtk_revealer_get_child_revealed(priv->user_actions_revealer)) {
		gtk_revealer_set_reveal_child(priv->user_actions_revealer, TRUE);
	}

	if (!gtk_revealer_get_child_revealed(priv->recipe_revealer)) {
		gtk_revealer_set_reveal_child(priv->recipe_revealer, TRUE);
	}
}

static gboolean
on_heating_element_swtich_state_set(G_GNUC_UNUSED GtkSwitch *widget, G_GNUC_UNUSED gboolean state,
									gpointer user_data)
{
	TlltCpWindow *self		  = TLLT_CP_WINDOW(user_data);
	TlltCpWindowPrivate *priv = tllt_cp_window_get_instance_private(self);

	if (!gtk_switch_get_active(priv->top_heating_element_switch) &&
		!gtk_switch_get_active(priv->bottom_heating_element_switch)) {
		gtk_widget_set_sensitive(GTK_WIDGET(priv->manual_start_button), FALSE);
	} else {
		if (!gtk_widget_get_sensitive(GTK_WIDGET(priv->manual_start_button))) {
			gtk_widget_set_sensitive(GTK_WIDGET(priv->manual_start_button), TRUE);
		}
	}

	return FALSE;
}

static void
on_timer_start_button_clicked(G_GNUC_UNUSED GtkButton *widget, gpointer user_data)
{
	TlltCpWindow *self		  = TLLT_CP_WINDOW(user_data);
	TlltCpWindowPrivate *priv = tllt_cp_window_get_instance_private(self);

	// Reset switches for next run
	if (!gtk_switch_get_active(priv->top_heating_element_switch)) {
		gtk_switch_set_active(priv->top_heating_element_switch, TRUE);
	}

	if (!gtk_switch_get_active(priv->bottom_heating_element_switch)) {
		gtk_switch_set_active(priv->top_heating_element_switch, TRUE);
	}

	const int minutes = gtk_spin_button_get_value_as_int(priv->timer_minutes_spin_button);
	const int seconds = gtk_spin_button_get_value_as_int(priv->timer_seconds_spin_button);

	g_object_ref(self);
	tllt_toaster_start(priv->toaster, minutes * 60 + seconds,
					   gtk_range_get_value(GTK_RANGE(priv->temperature_scale)), update_timer, self);
}

static void
on_timer_reset_button_clicked(G_GNUC_UNUSED GtkButton *widget, gpointer user_data)
{
	TlltCpWindow *self		  = TLLT_CP_WINDOW(user_data);
	TlltCpWindowPrivate *priv = tllt_cp_window_get_instance_private(self);

	tllt_toaster_stop(priv->toaster);
}

static void
on_toaster_stop_button_clicked(G_GNUC_UNUSED GtkButton *widget, gpointer user_data)
{
	TlltCpWindow *self		  = TLLT_CP_WINDOW(user_data);
	TlltCpWindowPrivate *priv = tllt_cp_window_get_instance_private(self);

	tllt_toaster_stop(priv->toaster);
}

static void
on_toaster_stopped(G_GNUC_UNUSED TlltToaster *toaster, gpointer user_data)
{
	TlltCpWindow *self		  = TLLT_CP_WINDOW(user_data);
	TlltCpWindowPrivate *priv = tllt_cp_window_get_instance_private(self);

	priv->toaster_user			   = NULL;
	priv->currently_running_recipe = NULL;

	if (priv->toaster_user != NULL && priv->currently_running_recipe != NULL) {
		g_print("Present user opinion window\n");
	}

	g_object_unref(self);	// Refers to ref when toaster is started

	gtk_widget_set_visible(GTK_WIDGET(priv->toasting_status_box), FALSE);
	gtk_stack_set_visible_child_name(priv->manual_stack, "edit-page");

	gboolean state = GTK_TREE_SORTABLE_UNSORTED_SORT_COLUMN_ID;
	gtk_container_foreach(GTK_CONTAINER(priv->recipe_list_box), recipe_list_box_toggle_run_buttons,
						  &state);
}

static gchar *
on_temperature_scale_format_value(G_GNUC_UNUSED GtkScale *scale, gdouble value,
								  G_GNUC_UNUSED gpointer user_data)
{
	return g_strdup_printf("%.0f °F", value);
}

static void
on_toaster_started(G_GNUC_UNUSED TlltToaster *toaster, gpointer user_data)
{
	TlltCpWindow *self		  = TLLT_CP_WINDOW(user_data);
	TlltCpWindowPrivate *priv = tllt_cp_window_get_instance_private(self);

	gtk_spinner_stop(priv->preparation_spinner);
	gtk_widget_set_sensitive(GTK_WIDGET(priv->timer_reset_button), TRUE);
	gtk_stack_set_visible_child_name(priv->status_stack, "progress-page");
}

static void
on_toaster_preparing(G_GNUC_UNUSED TlltToaster *toaster, gpointer user_data)
{
	TlltCpWindow *self		  = TLLT_CP_WINDOW(user_data);
	TlltCpWindowPrivate *priv = tllt_cp_window_get_instance_private(self);

	char buffer[3];
	sprintf(buffer, "%02d", gtk_spin_button_get_value_as_int(priv->timer_minutes_spin_button));
	gtk_label_set_label(priv->timer_minutes_label, buffer);
	sprintf(buffer, "%02d", gtk_spin_button_get_value_as_int(priv->timer_seconds_spin_button));
	gtk_label_set_label(priv->timer_seconds_label, buffer);

	gtk_widget_set_visible(GTK_WIDGET(priv->toasting_status_box), TRUE);
	gtk_widget_set_sensitive(GTK_WIDGET(priv->timer_reset_button), FALSE);

	gtk_spinner_start(priv->preparation_spinner);

	gtk_stack_set_visible_child_name(priv->status_stack, "preparation-page");
	gtk_stack_set_visible_child_name(priv->manual_stack, "display-page");

	gboolean state = FALSE;
	gtk_container_foreach(GTK_CONTAINER(priv->recipe_list_box), recipe_list_box_toggle_run_buttons,
						  &state);
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
	g_object_unref(priv->css_provider);

	G_OBJECT_CLASS(tllt_cp_window_parent_class)->finalize(object);
}

static void
tllt_cp_window_class_init(TlltCpWindowClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	GtkWidgetClass *wid_class  = GTK_WIDGET_CLASS(klass);

	object_class->finalize = tllt_cp_window_finalize;

	gtk_widget_class_set_template_from_resource(
		wid_class, "/ht/sr/git/tristan957/tllt-cp/ui/tllt-cp-window.ui");
	gtk_widget_class_bind_template_child_private(wid_class, TlltCpWindow, user_profiles_flow_box);
	gtk_widget_class_bind_template_child_private(wid_class, TlltCpWindow, user_actions_revealer);
	gtk_widget_class_bind_template_child_private(wid_class, TlltCpWindow, recipe_revealer);
	gtk_widget_class_bind_template_child_private(wid_class, TlltCpWindow, toasting_status_box);
	gtk_widget_class_bind_template_child_private(wid_class, TlltCpWindow, toasting_progress_bar);
	gtk_widget_class_bind_template_child_private(wid_class, TlltCpWindow, manual_stack);
	gtk_widget_class_bind_template_child_private(wid_class, TlltCpWindow, timer_minutes_label);
	gtk_widget_class_bind_template_child_private(wid_class, TlltCpWindow, timer_seconds_label);
	gtk_widget_class_bind_template_child_private(wid_class, TlltCpWindow, timer_reset_button);
	gtk_widget_class_bind_template_child_private(wid_class, TlltCpWindow,
												 timer_minutes_spin_button);
	gtk_widget_class_bind_template_child_private(wid_class, TlltCpWindow,
												 timer_seconds_spin_button);
	gtk_widget_class_bind_template_child_private(wid_class, TlltCpWindow, temperature_scale);
	gtk_widget_class_bind_template_child_private(wid_class, TlltCpWindow, recipe_list_box);
	gtk_widget_class_bind_template_child_private(wid_class, TlltCpWindow, preparation_spinner);
	gtk_widget_class_bind_template_child_private(wid_class, TlltCpWindow, status_stack);
	gtk_widget_class_bind_template_child_private(wid_class, TlltCpWindow,
												 top_heating_element_switch);
	gtk_widget_class_bind_template_child_private(wid_class, TlltCpWindow,
												 bottom_heating_element_switch);
	gtk_widget_class_bind_template_child_private(wid_class, TlltCpWindow, manual_start_button);
	gtk_widget_class_bind_template_callback(wid_class, on_login_button_clicked);
	gtk_widget_class_bind_template_callback(wid_class, on_logout_button_clicked);
	gtk_widget_class_bind_template_callback(wid_class, on_user_details_button_clicked);
	gtk_widget_class_bind_template_callback(wid_class, on_theme_state_changed);
	gtk_widget_class_bind_template_callback(wid_class, on_user_profiles_flow_box_child_activated);
	gtk_widget_class_bind_template_callback(wid_class, on_user_actions_revealer_close_clicked);
	gtk_widget_class_bind_template_callback(wid_class, on_recipe_revealer_close_button_clicked);
	gtk_widget_class_bind_template_callback(wid_class, on_recipe_revealer_new_button_clicked);
	gtk_widget_class_bind_template_callback(wid_class, show_leading_zeros);
	gtk_widget_class_bind_template_callback(wid_class, on_timer_start_button_clicked);
	gtk_widget_class_bind_template_callback(wid_class, on_timer_reset_button_clicked);
	gtk_widget_class_bind_template_callback(wid_class, on_toaster_stop_button_clicked);
	gtk_widget_class_bind_template_callback(wid_class, on_temperature_scale_format_value);
	gtk_widget_class_bind_template_callback(wid_class, on_heating_element_swtich_state_set);
}

static void
tllt_cp_window_init(TlltCpWindow *self)
{
	TlltCpWindowPrivate *priv = tllt_cp_window_get_instance_private(self);

	gtk_widget_init_template(GTK_WIDGET(self));

	priv->css_provider = gtk_css_provider_new();
	gtk_css_provider_load_from_resource(priv->css_provider,
										"/ht/sr/git/tristan957/tllt-cp/css/style.css");
	gtk_style_context_add_provider_for_screen(gtk_window_get_screen(GTK_WINDOW(self)),
											  GTK_STYLE_PROVIDER(priv->css_provider),
											  GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

	priv->client = tllt_cp_client_new_from_environment();

	const char *file_path = g_getenv("TLLT_TOASTER_CONFIG_FILE_PATH");
	g_autoptr(GError) err = NULL;
	priv->toaster		  = tllt_toaster_new_from_config_file(file_path, &err);
	if (err != NULL) {
		g_error("%s", err->message);
	}

	g_object_connect(priv->toaster, "signal::preparing", G_CALLBACK(on_toaster_preparing), self,
					 NULL);
	g_object_connect(priv->toaster, "signal::started", G_CALLBACK(on_toaster_started), self, NULL);
	g_object_connect(priv->toaster, "signal::stopped", G_CALLBACK(on_toaster_stopped), self, NULL);

	for (int i = 275; i < 450; i += 25) {
		gtk_scale_add_mark(priv->temperature_scale, i, GTK_POS_TOP, NULL);
	}
}
