#include <glib-object.h>
#include <glib/gi18n.h>

#include "tllt-cp-create-recipe-dto.h"
#include "tllt-cp-recipe.h"

struct _TlltCpCreateRecipeDto
{
	GObject parent_instance;
};

typedef struct TlltCpCreateRecipeDtoPrivate
{
	char *name;
	TlltCpRecipeType type;
} TlltCpCreateRecipeDtoPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(TlltCpCreateRecipeDto, tllt_cp_create_recipe_dto, G_TYPE_OBJECT)

typedef enum TlltCpCreateRecipeDtoProps
{
	PROP_NAME = 1,
	PROP_TYPE,
	N_PROPS,
} TlltCpCreateRecipeDtoProps;

static GParamSpec *obj_properties[N_PROPS];

static void
tllt_cp_create_recipe_dto_finalize(GObject *obj)
{
	TlltCpCreateRecipeDto *self		   = TLLT_CP_CREATE_RECIPE_DTO(obj);
	TlltCpCreateRecipeDtoPrivate *priv = tllt_cp_create_recipe_dto_get_instance_private(self);

	g_free(priv->name);

	G_OBJECT_CLASS(tllt_cp_create_recipe_dto_parent_class)->finalize(obj);
}

static void
tllt_cp_create_recipe_dto_get_property(GObject *obj, guint prop_id, GValue *val, GParamSpec *pspec)
{
	TlltCpCreateRecipeDto *self		   = TLLT_CP_CREATE_RECIPE_DTO(obj);
	TlltCpCreateRecipeDtoPrivate *priv = tllt_cp_create_recipe_dto_get_instance_private(self);

	switch (prop_id) {
	case PROP_NAME:
		g_value_set_string(val, priv->name);
		break;
	case PROP_TYPE:
		g_value_set_uint(val, priv->type);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
		break;
	}
}

static void
tllt_cp_create_recipe_dto_set_property(GObject *obj, guint prop_id, const GValue *val,
									   GParamSpec *pspec)
{
	TlltCpCreateRecipeDto *self		   = TLLT_CP_CREATE_RECIPE_DTO(obj);
	TlltCpCreateRecipeDtoPrivate *priv = tllt_cp_create_recipe_dto_get_instance_private(self);

	switch (prop_id) {
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
tllt_cp_create_recipe_dto_class_init(TlltCpCreateRecipeDtoClass *klass)
{
	GObjectClass *obj_class = G_OBJECT_CLASS(klass);

	obj_class->finalize		= tllt_cp_create_recipe_dto_finalize;
	obj_class->get_property = tllt_cp_create_recipe_dto_get_property;
	obj_class->set_property = tllt_cp_create_recipe_dto_set_property;

	obj_properties[PROP_NAME] =
		g_param_spec_string("name", _("Name"), _("Name of the recipe"), NULL,
							G_PARAM_CONSTRUCT_ONLY | G_PARAM_PRIVATE | G_PARAM_READWRITE);
	obj_properties[PROP_TYPE] =
		g_param_spec_uint("type", _("Type"), _("Type of the recipe"), 0, POPTART, 0,
						  G_PARAM_CONSTRUCT_ONLY | G_PARAM_PRIVATE | G_PARAM_READWRITE);

	g_object_class_install_properties(obj_class, N_PROPS, obj_properties);
}

static void
tllt_cp_create_recipe_dto_init(G_GNUC_UNUSED TlltCpCreateRecipeDto *self)
{}

TlltCpCreateRecipeDto *
tllt_cp_create_recipe_dto_new(const char *name, const TlltCpRecipeType type)
{
	return g_object_new(TLLT_CP_TYPE_CREATE_RECIPE_DTO, "name", name, "type", type, NULL);
}
