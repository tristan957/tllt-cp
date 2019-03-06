#include <glib/gi18n.h>
#include <gtk/gtk.h>

#include "tllt-cp-recipe-sidebar.h"

struct _TlltCpRecipeSidebar
{
	GtkBox parent_instance;
};

typedef struct TlltCpRecipeSidebarPrivate
{
	GtkListBox *recipe_list_box;

	TlltCpUser *user;
} TlltCpRecipeSidebarPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(TlltCpRecipeSidebar, tllt_cp_recipe_sidebar, GTK_TYPE_BOX)

typedef enum TlltCpRecipeSidebarProps
{
	PROP_USER = 1,
	N_PROPS
} TlltCpRecipeSidebarProps;

static GParamSpec *obj_properties[N_PROPS];

static void
tllt_cp_recipe_sidebar_get_property(GObject *obj, guint prop_id, GValue *val, GParamSpec *pspec)
{
	TlltCpRecipeSidebar *self		 = TLLT_CP_RECIPE_SIDEBAR(obj);
	TlltCpRecipeSidebarPrivate *priv = tllt_cp_recipe_sidebar_get_instance_private(self);

	switch (prop_id) {
	case PROP_USER:
		g_value_set_object(val, priv->user);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
		break;
	}
}

static void
tllt_cp_recipe_sidebar_set_property(GObject *obj, guint prop_id, const GValue *val,
									GParamSpec *pspec)
{
	TlltCpRecipeSidebar *self		 = TLLT_CP_RECIPE_SIDEBAR(obj);
	TlltCpRecipeSidebarPrivate *priv = tllt_cp_recipe_sidebar_get_instance_private(self);

	switch (prop_id) {
	case PROP_USER:
		/**
		 * Use the same user pointer that exists at the window level. Increment ref count of new
		 * object and decrement ref count of old object.
		 */
		g_object_unref(priv->user);
		priv->user = g_object_ref(g_value_get_pointer(val));
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
		break;
	}
}

static void
tllt_cp_recipe_sidebar_finalize(GObject *obj)
{
	G_OBJECT_CLASS(tllt_cp_recipe_sidebar_parent_class)->finalize(obj);
}

static void
tllt_cp_recipe_sidebar_class_init(TlltCpRecipeSidebarClass *klass)
{
	GObjectClass *obj_class   = G_OBJECT_CLASS(klass);
	GtkWidgetClass *wid_class = GTK_WIDGET_CLASS(klass);

	obj_class->finalize		= tllt_cp_recipe_sidebar_finalize;
	obj_class->get_property = tllt_cp_recipe_sidebar_get_property;
	obj_class->set_property = tllt_cp_recipe_sidebar_set_property;

	obj_properties[PROP_USER] = g_param_spec_object(
		"user", _("User"), _("The user to show reciped of"), TLLT_CP_TYPE_USER, G_PARAM_READWRITE);

	g_object_class_install_properties(obj_class, N_PROPS, obj_properties);

	gtk_widget_class_set_template_from_resource(
		wid_class, "/com/gitlab/tristan957/tllt-cp/tllt-cp-recipe-sidebar.ui");
}

static void
tllt_cp_recipe_sidebar_init(TlltCpRecipeSidebar *self)
{
	gtk_widget_init_template(GTK_WIDGET(self));
}

void
tllt_cp_recipe_sidebar_clear(G_GNUC_UNUSED TlltCpRecipeSidebar *self)
{}

void
tllt_cp_recipe_sidebar_add_recipe(G_GNUC_UNUSED TlltCpRecipeSidebar *self)
{}

void
tllt_cp_recipe_sidebar_set_user(TlltCpRecipeSidebar *self, TlltCpUser *user)
{
	TlltCpRecipeSidebarPrivate *priv = tllt_cp_recipe_sidebar_get_instance_private(self);

	priv->user = user;
}

TlltCpRecipeSidebar *
tllt_cp_recipe_sidebar_new(TlltCpUser *user)
{
	return g_object_new(TLLT_CP_TYPE_RECIPE_SIDEBAR, "user", user, NULL);
}
