#include <glib-object.h>
#include <glib/gi18n.h>

#include "dto/tllt-cp-create-recipe-dto.h"
#include "tllt-cp-client.h"
#include "tllt-cp-recipe.h"
#include "tllt-cp-user.h"

struct _TlltCpRecipe
{
	GObject parent_instance;
};

typedef struct TlltCpRecipePrivate
{
	unsigned int id;
	char *name;
	TlltCpRecipeType type;
} TlltCpRecipePrivate;

G_DEFINE_TYPE_WITH_PRIVATE(TlltCpRecipe, tllt_cp_recipe, G_TYPE_OBJECT)

typedef enum TlltCpRecipeProps
{
	PROP_ID = 1,
	PROP_NAME,
	PROP_TYPE,
	N_PROPS,
} TlltCpRecipeProps;

static GParamSpec *obj_properties[N_PROPS];

static void
tllt_cp_recipe_set_property(GObject *obj, guint prop_id, const GValue *val, GParamSpec *pspec)
{
	TlltCpRecipe *self		  = TLLT_CP_RECIPE(obj);
	TlltCpRecipePrivate *priv = tllt_cp_recipe_get_instance_private(self);

	switch (prop_id) {
	case PROP_ID:
		priv->id = g_value_get_uint(val);
		break;
	case PROP_NAME:
		g_free(priv->name);
		priv->name = g_value_dup_string(val);
		break;
	case PROP_TYPE:
		priv->type = g_value_get_uint(val);
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
	case PROP_ID:
		g_value_set_uint(val, priv->id);
		break;
	case PROP_NAME:
		g_value_set_string(val, priv->name);
		break;
	case PROP_TYPE:
		g_value_set_enum(val, priv->type);
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

	g_free(priv->name);

	G_OBJECT_CLASS(tllt_cp_recipe_parent_class)->finalize(obj);
}

static void
tllt_cp_recipe_class_init(TlltCpRecipeClass *klass)
{
	GObjectClass *obj_class = G_OBJECT_CLASS(klass);

	obj_class->finalize		= tllt_cp_recipe_finalize;
	obj_class->get_property = tllt_cp_recipe_get_property;
	obj_class->set_property = tllt_cp_recipe_set_property;

	obj_properties[PROP_ID] =
		g_param_spec_uint("id", _("Recipe ID"), _("ID of the recipe"), 0, UINT_MAX, 0,
						  G_PARAM_PRIVATE | G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);
	obj_properties[PROP_NAME] =
		g_param_spec_string("name", _("Name"), _("Name of the recipe"), NULL,
							G_PARAM_PRIVATE | G_PARAM_READWRITE | G_PARAM_CONSTRUCT);
	obj_properties[PROP_TYPE] =
		g_param_spec_uint("type", _("Type"), _("Type of the recipe"), 0, POPTART, TOAST,
						  G_PARAM_PRIVATE | G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY);

	g_object_class_install_properties(obj_class, N_PROPS, obj_properties);
}

static void
tllt_cp_recipe_init(G_GNUC_UNUSED TlltCpRecipe *self)
{}

const char *
tllt_cp_recipe_get_name(TlltCpRecipe *self)
{
	TlltCpRecipePrivate *priv = tllt_cp_recipe_get_instance_private(self);

	return priv->name;
}

void
tllt_cp_recipe_set_name(TlltCpRecipe *self, const char *name)
{
	TlltCpRecipePrivate *priv = tllt_cp_recipe_get_instance_private(self);

	if (g_strcmp0(name, priv->name) != 0) {
		g_free(priv->name);
		priv->name = g_strstrip(g_strdup(name));
	}
}

TlltCpRecipeType
tllt_cp_recipe_get_rtype(TlltCpRecipe *self)
{
	TlltCpRecipePrivate *priv = tllt_cp_recipe_get_instance_private(self);

	return priv->type;
}
