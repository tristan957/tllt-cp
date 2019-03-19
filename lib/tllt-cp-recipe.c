#include <glib-object.h>
#include <glib/gi18n.h>

#include "tllt-cp-recipe.h"

struct _TlltCpRecipe
{
	GObject parent_instance;
};

typedef struct TlltCpRecipePrivate
{
	char *name;
} TlltCpRecipePrivate;

G_DEFINE_TYPE_WITH_PRIVATE(TlltCpRecipe, tllt_cp_recipe, G_TYPE_OBJECT)

typedef enum TlltCpRecipeProps
{
	PROP_NAME = 1,
	N_PROPS,
} TlltCpRecipeProps;

static GParamSpec *obj_properties[N_PROPS];

static void
tllt_cp_recipe_set_property(GObject *obj, guint prop_id, const GValue *val, GParamSpec *pspec)
{
	TlltCpRecipe *self		  = TLLT_CP_RECIPE(obj);
	TlltCpRecipePrivate *priv = tllt_cp_recipe_get_instance_private(self);

	switch (prop_id) {
	case PROP_NAME:
		g_free(priv->name);
		priv->name = g_value_dup_string(val);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
		break;
	}
}

static void
tllt_cp_recipe_get_property(GObject *obj, guint prop_id, GValue *val, GParamSpec *pspec)
{
	TlltCpRecipe *self		  = TLLT_CP_RECIPE(obj);
	TlltCpRecipePrivate *priv = tllt_cp_recipe_get_instance_private(self);

	switch (prop_id) {
	case PROP_NAME:
		g_value_set_string(val, priv->name);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
		break;
	}
}

static void
tllt_cp_recipe_finalize(GObject *obj)
{
	TlltCpRecipe *self		  = TLLT_CP_RECIPE(obj);
	TlltCpRecipePrivate *priv = tllt_cp_recipe_get_instance_private(self);

	if (priv->name != NULL) {
		g_free(priv->name);
	}

	G_OBJECT_CLASS(tllt_cp_recipe_parent_class)->finalize(obj);
}

static void
tllt_cp_recipe_class_init(TlltCpRecipeClass *klass)
{
	GObjectClass *obj_class = G_OBJECT_CLASS(klass);

	obj_class->finalize		= tllt_cp_recipe_finalize;
	obj_class->get_property = tllt_cp_recipe_get_property;
	obj_class->set_property = tllt_cp_recipe_set_property;

	obj_properties[PROP_NAME] =
		g_param_spec_string("name", _("Name"), _("Recipe name"), NULL, G_PARAM_READWRITE);

	g_object_class_install_properties(obj_class, N_PROPS, obj_properties);
}

static void
tllt_cp_recipe_init(G_GNUC_UNUSED TlltCpRecipe *self)
{}

TlltCpRecipe *
tllt_cp_recipe_new()
{
	return g_object_new(TLLT_CP_TYPE_RECIPE, NULL);
}
