#include <glib-object.h>
#include <glib/gi18n.h>
#include <gtk/gtk.h>

#include "tllt-cp-new-recipe-window.h"
#include "tllt-cp.h"

struct _TlltCpNewRecipeWindow
{
	GtkWindow parent_instance;
};

typedef struct TlltCpNewRecipeWindowPrivate
{
	GtkEntry *recipe_name_entry;
	GtkComboBoxText *recipe_type_combo;

	TlltCpClient *client;
	TlltCpUser *author;
} TlltCpNewRecipeWindowPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(TlltCpNewRecipeWindow, tllt_cp_new_recipe_window, GTK_TYPE_WINDOW)

typedef enum TlltCpNewRecipeWindowProps
{
	PROP_CLIENT = 1,
	PROP_AUTHOR,
	N_PROPS,
} TlltCpNewRecipeWindowProps;

static GParamSpec *obj_properties[N_PROPS];

static void
tllt_cp_new_recipe_window_finalize(GObject *obj)
{
	TlltCpNewRecipeWindow *self		   = TLLT_CP_NEW_RECIPE_WINDOW(obj);
	TlltCpNewRecipeWindowPrivate *priv = tllt_cp_new_recipe_window_get_instance_private(self);

	g_object_unref(priv->author);
	g_object_unref(priv->client);

	G_OBJECT_CLASS(tllt_cp_new_recipe_window_parent_class)->finalize(obj);
}

static void
tllt_cp_new_recipe_window_get_property(GObject *obj, guint prop_id, GValue *val, GParamSpec *pspec)
{
	TlltCpNewRecipeWindow *self		   = TLLT_CP_NEW_RECIPE_WINDOW(obj);
	TlltCpNewRecipeWindowPrivate *priv = tllt_cp_new_recipe_window_get_instance_private(self);

	switch (prop_id) {
	case PROP_CLIENT:
		g_value_set_object(val, priv->client);
		break;
	case PROP_AUTHOR:
		g_value_set_object(val, priv->author);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
	}
}

static void
tllt_cp_new_recipe_window_set_property(GObject *obj, guint prop_id, const GValue *val,
									   GParamSpec *pspec)
{
	TlltCpNewRecipeWindow *self		   = TLLT_CP_NEW_RECIPE_WINDOW(obj);
	TlltCpNewRecipeWindowPrivate *priv = tllt_cp_new_recipe_window_get_instance_private(self);

	switch (prop_id) {
	case PROP_CLIENT:
		priv->client = g_value_dup_object(val);
		break;
	case PROP_AUTHOR:
		priv->author = g_value_dup_object(val);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
	}
}

static void
on_cancel_button_clicked(G_GNUC_UNUSED GtkButton *widget, gpointer user_data)
{
	TlltCpNewRecipeWindow *self = TLLT_CP_NEW_RECIPE_WINDOW(user_data);

	gtk_window_close(GTK_WINDOW(self));
}

static void
on_create_recipe_button_clicked(G_GNUC_UNUSED GtkButton *widget, gpointer user_data)
{
	TlltCpNewRecipeWindow *self		   = TLLT_CP_NEW_RECIPE_WINDOW(user_data);
	TlltCpNewRecipeWindowPrivate *priv = tllt_cp_new_recipe_window_get_instance_private(self);

	g_autoptr(GError) err = NULL;
	TlltCpRecipe *recipe  = tllt_cp_user_add_recipe(
		 priv->author, priv->client, gtk_entry_get_text(priv->recipe_name_entry),
		 gtk_combo_box_get_active(GTK_COMBO_BOX(priv->recipe_type_combo)), &err);

	// TODO: do something meaningful with error
	if (err != NULL) {
		g_printerr("%s\n", err->message);
	}
}

static void
tllt_cp_new_recipe_window_class_init(TlltCpNewRecipeWindowClass *klass)
{
	GObjectClass *obj_class   = G_OBJECT_CLASS(klass);
	GtkWidgetClass *wid_class = GTK_WIDGET_CLASS(klass);

	obj_class->finalize		= tllt_cp_new_recipe_window_finalize;
	obj_class->get_property = tllt_cp_new_recipe_window_get_property;
	obj_class->set_property = tllt_cp_new_recipe_window_set_property;

	obj_properties[PROP_CLIENT] = g_param_spec_object(
		"client", _("Client"), _("Client to make requests with"), TLLT_CP_TYPE_CLIENT,
		G_PARAM_PRIVATE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY);
	obj_properties[PROP_AUTHOR] =
		g_param_spec_object("author", _("Author"), _("Author of the recipe"), TLLT_CP_TYPE_USER,
							G_PARAM_PRIVATE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY);

	g_object_class_install_properties(obj_class, N_PROPS, obj_properties);

	gtk_widget_class_set_template_from_resource(
		wid_class, "/ht/sr/git/tristan957/tllt-cp/ui/tllt-cp-new-recipe-window.ui");
	gtk_widget_class_bind_template_child_private(wid_class, TlltCpNewRecipeWindow,
												 recipe_name_entry);
	gtk_widget_class_bind_template_child_private(wid_class, TlltCpNewRecipeWindow,
												 recipe_type_combo);
	gtk_widget_class_bind_template_callback(wid_class, on_cancel_button_clicked);
	gtk_widget_class_bind_template_callback(wid_class, on_create_recipe_button_clicked);
}

static void
tllt_cp_new_recipe_window_init(TlltCpNewRecipeWindow *self)
{
	gtk_widget_init_template(GTK_WIDGET(self));
}

TlltCpNewRecipeWindow *
tllt_cp_new_recipe_window_new(GtkWindow *parent, TlltCpClient *client, TlltCpUser *author)
{
	return g_object_new(TLLT_CP_TYPE_NEW_RECIPE_WINDOW, "transient-for", parent, "client", client,
						"author", author, NULL);
}
