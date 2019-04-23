#include <limits.h>

#include <curl/curl.h>
#include <glib-object.h>
#include <glib/gi18n.h>
#include <json-glib/json-glib.h>

#include "dto/tllt-cp-authentication-dto.h"
#include "dto/tllt-cp-create-recipe-dto.h"
#include "dto/tllt-cp-create-user-dto.h"
#include "tllt-cp-client.h"
#include "tllt-cp-recipe.h"
#include "tllt-cp-user.h"

typedef struct TlltCpUserPrivate
{
	unsigned int id;
	char *name;
	char *email;
	GList *recipes;	// TlltCpRecipe *
} TlltCpUserPrivate;

static void tllt_cp_user_json_serializable_init(JsonSerializableIface *iface);

G_DEFINE_TYPE_WITH_CODE(TlltCpUser, tllt_cp_user, G_TYPE_OBJECT,
						G_ADD_PRIVATE(TlltCpUser)
							G_IMPLEMENT_INTERFACE(JSON_TYPE_SERIALIZABLE,
												  tllt_cp_user_json_serializable_init))

typedef enum TlltCpUserProperties
{
	PROP_NAME = 1,
	PROP_EMAIL,
	PROP_ID,
	PROP_RECIPES,
	N_PROPS
} TlltCpUserProperties;

static GParamSpec *obj_properties[N_PROPS];

typedef enum TlltCpUserSignals
{
	SIGNAL_RECIPE_ADDED,
	N_SIGNALS,
} TlltCpUserSignals;

static guint obj_signals[N_SIGNALS];

static gboolean
tllt_cp_user_deserialize_property(JsonSerializable *serializable, const gchar *prop_name,
								  GValue *val, GParamSpec *pspec, JsonNode *prop_node)
{
	if (g_strcmp0("recipes", prop_name) == 0) {
		GList *recipes = NULL;
		JsonArray *arr = json_node_get_array(prop_node);
		for (guint i = 0; i < json_array_get_length(arr); i++) {
			GObject *obj =
				json_gobject_deserialize(TLLT_CP_TYPE_RECIPE, json_array_get_element(arr, i));
			if (obj == NULL) {
				return FALSE;
			}

			recipes = g_list_append(recipes, TLLT_CP_RECIPE(obj));
		}

		g_value_set_pointer(val, recipes);

		return TRUE;
	}

	return json_serializable_default_deserialize_property(serializable, prop_name, val, pspec,
														  prop_node);
}

static void
tllt_cp_user_json_serializable_init(JsonSerializableIface *iface)
{
	iface->deserialize_property = tllt_cp_user_deserialize_property;
}

static void
tllt_cp_user_get_property(GObject *obj, guint prop_id, GValue *val, GParamSpec *pspec)
{
	TlltCpUser *self		= TLLT_CP_USER(obj);
	TlltCpUserPrivate *priv = tllt_cp_user_get_instance_private(self);

	switch (prop_id) {
	case PROP_NAME:
		g_value_set_string(val, priv->name);
		break;
	case PROP_EMAIL:
		g_value_set_string(val, priv->email);
		break;
	case PROP_ID:
		g_value_set_uint(val, priv->id);
		break;
	case PROP_RECIPES:
		g_value_set_pointer(val, priv->recipes);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
		break;
	}
}

static void
tllt_cp_user_set_property(GObject *obj, guint prop_id, const GValue *val, GParamSpec *pspec)
{
	TlltCpUser *self		= TLLT_CP_USER(obj);
	TlltCpUserPrivate *priv = tllt_cp_user_get_instance_private(self);

	switch (prop_id) {
	case PROP_NAME:
		g_free(priv->name);
		priv->name = g_value_dup_string(val);
		break;
	case PROP_EMAIL:
		g_free(priv->email);
		priv->email = g_value_dup_string(val);
		break;
	case PROP_ID:
		priv->id = g_value_get_uint(val);
		break;
	case PROP_RECIPES:
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-function-type"
		priv->recipes = g_list_copy_deep(g_value_get_pointer(val), (GCopyFunc) g_object_ref, NULL);
#pragma GCC diagnostic pop
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
		break;
	}
}

static void
tllt_cp_user_finalize(GObject *obj)
{
	TlltCpUser *self		= TLLT_CP_USER(obj);
	TlltCpUserPrivate *priv = tllt_cp_user_get_instance_private(self);

	g_free(priv->name);
	g_free(priv->email);
	g_list_free_full(priv->recipes, g_object_unref);

	G_OBJECT_CLASS(tllt_cp_user_parent_class)->finalize(obj);
}

static void
tllt_cp_user_class_init(TlltCpUserClass *klass)
{
	GObjectClass *obj_class = G_OBJECT_CLASS(klass);

	obj_class->finalize		= tllt_cp_user_finalize;
	obj_class->get_property = tllt_cp_user_get_property;
	obj_class->set_property = tllt_cp_user_set_property;

	obj_properties[PROP_NAME] =
		g_param_spec_string("name", _("Name"), _("Name of the user"), NULL,
							G_PARAM_PRIVATE | G_PARAM_CONSTRUCT | G_PARAM_READWRITE);
	obj_properties[PROP_EMAIL] =
		g_param_spec_string("email", _("Email"), _("Email of the user"), NULL,
							G_PARAM_PRIVATE | G_PARAM_CONSTRUCT | G_PARAM_READWRITE);
	obj_properties[PROP_ID] =
		g_param_spec_uint("id", _("User ID"), _("ID of the user"), 0, UINT_MAX, 0,
						  G_PARAM_PRIVATE | G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);
	obj_properties[PROP_RECIPES] = g_param_spec_pointer("recipes", _("Recipes"), _("User recipes"),
														G_PARAM_PRIVATE | G_PARAM_READWRITE);

	g_object_class_install_properties(obj_class, N_PROPS, obj_properties);

	obj_signals[SIGNAL_RECIPE_ADDED] =
		g_signal_new("recipe-added", G_TYPE_FROM_CLASS(klass), G_SIGNAL_RUN_LAST, 0, NULL, NULL,
					 NULL, G_TYPE_NONE, 1, TLLT_CP_TYPE_RECIPE);
}

static void
tllt_cp_user_init(G_GNUC_UNUSED TlltCpUser *self)
{}

TlltCpUser *
tllt_cp_user_get_by_id(TlltCpClient *client, unsigned int id, GError **err)
{
	g_return_val_if_fail(err == NULL || *err == NULL, NULL);

	g_autoptr(GString) endpoint = g_string_new(client->server);
	g_string_append_printf(endpoint, "/users/%u", id);
	GObject *obj = tllt_cp_client_get_request(client, TLLT_CP_TYPE_USER, endpoint->str, err);

	g_return_val_if_fail(err == NULL || *err == NULL, NULL);

	return TLLT_CP_USER(obj);
}

TlltCpUser *
tllt_cp_user_authenticate(TlltCpClient *client, const char *email, const char *password,
						  GError **err)
{
	g_return_val_if_fail(err == NULL || *err == NULL, NULL);

	g_autoptr(TlltCpAuthenticationDto) dto = tllt_cp_authentication_dto_new(email, password);
	g_autoptr(GString) endpoint			   = g_string_new(client->server);
	g_string_append_printf(endpoint, "/users/authenticate");
	GObject *obj =
		tllt_cp_client_post_request(client, TLLT_CP_TYPE_USER, endpoint->str, G_OBJECT(dto), err);

	if (err == NULL) {
		return NULL;
	}

	return TLLT_CP_USER(obj);
}

TlltCpUser *
tllt_cp_user_create(TlltCpClient *client, const char *name, const char *email, const char *password,
					GError **err)
{
	g_return_val_if_fail(err == NULL || *err == NULL, NULL);

	g_autoptr(TlltCpCreateUserDto) dto = tllt_cp_create_user_dto_new(name, email, password);
	g_autoptr(GString) endpoint		   = g_string_new(client->server);
	g_string_append_printf(endpoint, "/users");
	GObject *obj =
		tllt_cp_client_post_request(client, TLLT_CP_TYPE_USER, endpoint->str, G_OBJECT(dto), err);

	if (*err != NULL) {
		return NULL;
	}

	return TLLT_CP_USER(obj);
}

TlltCpRecipe *
tllt_cp_user_add_recipe(TlltCpUser *self, TlltCpClient *client, const char *name,
						const TlltCpRecipeType type, GError **err)
{
	g_return_val_if_fail(err == NULL || *err == NULL, NULL);

	TlltCpUserPrivate *priv = tllt_cp_user_get_instance_private(self);

	g_autoptr(TlltCpCreateRecipeDto) dto = tllt_cp_create_recipe_dto_new(name, type);
	g_autoptr(GString) endpoint			 = g_string_new(client->server);
	g_string_append_printf(endpoint, "/users/%u/recipes", priv->id);
	GObject *obj =
		tllt_cp_client_post_request(client, TLLT_CP_TYPE_RECIPE, endpoint->str, G_OBJECT(dto), err);

	if (*err != NULL) {
		return NULL;
	}

	TlltCpRecipe *recipe = TLLT_CP_RECIPE(obj);
	priv->recipes		 = g_list_append(priv->recipes, recipe);

	g_signal_emit(self, obj_signals[SIGNAL_RECIPE_ADDED], 0, recipe);

	return recipe;
}

unsigned int
tllt_cp_user_get_id(TlltCpUser *self)
{
	TlltCpUserPrivate *priv = tllt_cp_user_get_instance_private(self);

	return priv->id;
}

char *
tllt_cp_user_get_email(TlltCpUser *self)
{
	TlltCpUserPrivate *priv = tllt_cp_user_get_instance_private(self);

	return priv->email;
}

char *
tllt_cp_user_get_name(TlltCpUser *self)
{
	TlltCpUserPrivate *priv = tllt_cp_user_get_instance_private(self);

	return priv->name;
}

GList *
tllt_cp_user_get_recipes(TlltCpUser *self)
{
	TlltCpUserPrivate *priv = tllt_cp_user_get_instance_private(self);

	return priv->recipes;
}

TlltCpCookingDetailsDto *
tllt_cp_user_get_cooking_details_for_recipe(TlltCpUser *self, TlltCpClient *client,
											TlltCpRecipe *recipe, GError **err)
{
	g_return_val_if_fail(err == NULL || *err == NULL, NULL);

	TlltCpUserPrivate *priv = tllt_cp_user_get_instance_private(self);

	g_autoptr(GString) endpoint = g_string_new(client->server);
	g_string_append_printf(endpoint, "/users/%u/recipes/%u/cook", priv->id,
						   tllt_cp_recipe_get_id(recipe));
	GObject *obj =
		tllt_cp_client_get_request(client, TLLT_CP_TYPE_COOKING_DETAILS_DTO, endpoint->str, err);

	g_return_val_if_fail(err == NULL || *err == NULL, NULL);

	return TLLT_CP_COOKING_DETAILS_DTO(obj);
}
