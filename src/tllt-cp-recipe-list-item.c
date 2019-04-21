#include <glib-object.h>
#include <glib/gi18n.h>
#include <gtk/gtk.h>

#include "tllt-cp-recipe-list-item.h"
#include "tllt-cp.h"

struct _TlltCpRecipeListItem
{
	GtkBox parent_instance;
};

typedef struct TlltCpRecipeListItemPrivate
{
	GtkLabel *recipe_name_label;
	GtkLabel *recipe_type_label;
	GtkButton *recipe_run_button;
	GtkPopover *recipe_info_popover;
	GtkEntry *recipe_name_entry;

	TlltCpRecipe *recipe;
} TlltCpRecipeListItemPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(TlltCpRecipeListItem, tllt_cp_recipe_list_item, GTK_TYPE_BOX)

typedef enum TlltCpRecipeListItemProps
{
	PROP_RECIPE = 1,
	N_PROPS,
} TlltCpRecipeListItemProps;

static GParamSpec *obj_properties[N_PROPS];

typedef enum TlltCpRecipeListItemSignals
{
	SIGNAL_RECIPE_DELETED,
	N_SIGNALS,
} TlltCpRecipeListItemSignals;

static guint obj_signals[N_SIGNALS];

static void
on_recipe_delete_button_clicked(G_GNUC_UNUSED GtkButton *widget, gpointer user_data)
{
	TlltCpRecipeListItem *self = TLLT_CP_RECIPE_LIST_ITEM(user_data);

	// g_object_unref(self)? probs do in listbox

	g_signal_emit(self, obj_signals[SIGNAL_RECIPE_DELETED], 0);
}

static void
on_recipe_save_button_clicked(G_GNUC_UNUSED GtkButton *widget, gpointer user_data)
{
	TlltCpRecipeListItem *self		  = TLLT_CP_RECIPE_LIST_ITEM(user_data);
	TlltCpRecipeListItemPrivate *priv = tllt_cp_recipe_list_item_get_instance_private(self);

	const char *name = gtk_entry_buffer_get_text(GTK_ENTRY_BUFFER(priv->recipe_name_entry));
	tllt_cp_recipe_set_name(priv->recipe, name);
}

static void
tllt_cp_recipe_list_item_finalize(GObject *obj)
{
	TlltCpRecipeListItem *self		  = TLLT_CP_RECIPE_LIST_ITEM(obj);
	TlltCpRecipeListItemPrivate *priv = tllt_cp_recipe_list_item_get_instance_private(self);

	if (priv->recipe != NULL) {
		g_object_unref(priv->recipe);
	}

	G_OBJECT_CLASS(tllt_cp_recipe_list_item_parent_class)->finalize(obj);
}

static void
tllt_cp_recipe_list_item_get_property(GObject *obj, guint prop_id, GValue *val, GParamSpec *pspec)
{
	TlltCpRecipeListItem *self		  = TLLT_CP_RECIPE_LIST_ITEM(obj);
	TlltCpRecipeListItemPrivate *priv = tllt_cp_recipe_list_item_get_instance_private(self);

	switch (prop_id) {
	case PROP_RECIPE:
		g_value_set_object(val, priv->recipe);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
	}
}

static void
tllt_cp_recipe_list_item_set_property(GObject *obj, guint prop_id, const GValue *val,
									  GParamSpec *pspec)
{
	TlltCpRecipeListItem *self		  = TLLT_CP_RECIPE_LIST_ITEM(obj);
	TlltCpRecipeListItemPrivate *priv = tllt_cp_recipe_list_item_get_instance_private(self);

	switch (prop_id) {
	case PROP_RECIPE:
		if (priv->recipe != NULL) {
			g_object_unref(priv->recipe);
		}
		priv->recipe = g_value_dup_object(val);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
	}
}

static void
tllt_cp_recipe_list_item_constructed(GObject *obj)
{
	TlltCpRecipeListItem *self		  = TLLT_CP_RECIPE_LIST_ITEM(obj);
	TlltCpRecipeListItemPrivate *priv = tllt_cp_recipe_list_item_get_instance_private(self);

	gtk_label_set_label(priv->recipe_name_label, tllt_cp_recipe_get_name(priv->recipe));

	switch (tllt_cp_recipe_get_rtype(priv->recipe)) {
	case TOAST:
		gtk_label_set_label(priv->recipe_type_label, "Toast");
		break;
	case BAGEL:
		gtk_label_set_label(priv->recipe_type_label, "Bagel");
		break;
	case PIZZA:
		gtk_label_set_label(priv->recipe_type_label, "Pizza");
		break;
	case POPTART:
		gtk_label_set_label(priv->recipe_type_label, "Poptart");
		break;
	default:
		g_warn_if_reached();
	}
}

static void
tllt_cp_recipe_list_item_class_init(TlltCpRecipeListItemClass *klass)
{
	GObjectClass *obj_class   = G_OBJECT_CLASS(klass);
	GtkWidgetClass *wid_class = GTK_WIDGET_CLASS(klass);

	obj_class->constructed  = tllt_cp_recipe_list_item_constructed;
	obj_class->finalize		= tllt_cp_recipe_list_item_finalize;
	obj_class->get_property = tllt_cp_recipe_list_item_get_property;
	obj_class->set_property = tllt_cp_recipe_list_item_set_property;

	obj_properties[PROP_RECIPE] =
		g_param_spec_object("recipe", _("Recipe"), _("Recipe"), TLLT_CP_TYPE_RECIPE,
							G_PARAM_PRIVATE | G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);

	g_object_class_install_properties(obj_class, N_PROPS, obj_properties);

	obj_signals[SIGNAL_RECIPE_DELETED] =
		g_signal_new("recipe-deleted", G_TYPE_FROM_CLASS(klass), G_SIGNAL_RUN_LAST, 0, NULL, NULL,
					 NULL, G_TYPE_NONE, 0);

	gtk_widget_class_set_template_from_resource(
		wid_class, "/ht/sr/git/tristan957/tllt-cp/ui/tllt-cp-recipe-list-item.ui");
	gtk_widget_class_bind_template_child_private(wid_class, TlltCpRecipeListItem,
												 recipe_name_label);
	gtk_widget_class_bind_template_child_private(wid_class, TlltCpRecipeListItem,
												 recipe_type_label);
	gtk_widget_class_bind_template_child_private(wid_class, TlltCpRecipeListItem,
												 recipe_info_popover);
	gtk_widget_class_bind_template_child_private(wid_class, TlltCpRecipeListItem,
												 recipe_run_button);
	gtk_widget_class_bind_template_child_private(wid_class, TlltCpRecipeListItem,
												 recipe_name_entry);
	gtk_widget_class_bind_template_callback(wid_class, on_recipe_delete_button_clicked);
	gtk_widget_class_bind_template_callback(wid_class, on_recipe_save_button_clicked);
}

static void
tllt_cp_recipe_list_item_init(TlltCpRecipeListItem *self)
{
	gtk_widget_init_template(GTK_WIDGET(self));
}

TlltCpRecipeListItem *
tllt_cp_recipe_list_item_new(TlltCpRecipe *recipe)
{
	return g_object_new(TLLT_CP_TYPE_RECIPE_LIST_ITEM, "recipe", recipe, NULL);
}
