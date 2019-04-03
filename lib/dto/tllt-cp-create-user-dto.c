#include <glib-object.h>
#include <glib/gi18n.h>

#include "tllt-cp-create-user-dto.h"

struct _TlltCpCreateUserDto
{
	GObject parent_instance;
};

typedef struct TlltCpCreateUserDtoPrivate
{
	char *name;
	char *email;
	char *password;
} TlltCpCreateUserDtoPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(TlltCpCreateUserDto, tllt_cp_create_user_dto, G_TYPE_OBJECT)

typedef enum TlltCpCreateUserDtoProps
{
	PROP_NAME = 1,
	PROP_EMAIL,
	PROP_PASSWORD,
	N_PROPS,
} TlltCpCreateUserDtoProps;

static GParamSpec *obj_properties[N_PROPS];

static void
tllt_cp_create_user_dto_finalize(GObject *obj)
{
	TlltCpCreateUserDto *self		 = TLLT_CP_CREATE_USER_DTO(obj);
	TlltCpCreateUserDtoPrivate *priv = tllt_cp_create_user_dto_get_instance_private(self);

	g_free(priv->name);
	g_free(priv->email);
	g_free(priv->password);

	G_OBJECT_CLASS(tllt_cp_create_user_dto_parent_class)->finalize(obj);
}

static void
tllt_cp_create_user_dto_get_property(GObject *obj, guint prop_id, GValue *val, GParamSpec *pspec)
{
	TlltCpCreateUserDto *self		 = TLLT_CP_CREATE_USER_DTO(obj);
	TlltCpCreateUserDtoPrivate *priv = tllt_cp_create_user_dto_get_instance_private(self);

	switch (prop_id) {
	case PROP_NAME:
		g_value_set_string(val, priv->name);
		break;
	case PROP_EMAIL:
		g_value_set_string(val, priv->email);
		break;
	case PROP_PASSWORD:
		g_value_set_string(val, priv->password);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
		break;
	}
}

static void
tllt_cp_create_user_dto_set_property(GObject *obj, guint prop_id, const GValue *val,
									 GParamSpec *pspec)
{
	TlltCpCreateUserDto *self		 = TLLT_CP_CREATE_USER_DTO(obj);
	TlltCpCreateUserDtoPrivate *priv = tllt_cp_create_user_dto_get_instance_private(self);

	switch (prop_id) {
	case PROP_NAME:
		g_free(priv->name);
		priv->name = g_value_dup_string(val);
		break;
	case PROP_EMAIL:
		g_free(priv->email);
		priv->email = g_value_dup_string(val);
		break;
	case PROP_PASSWORD:
		g_free(priv->password);
		priv->password = g_value_dup_string(val);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
		break;
	}
}

static void
tllt_cp_create_user_dto_class_init(TlltCpCreateUserDtoClass *klass)
{
	GObjectClass *obj_class = G_OBJECT_CLASS(klass);

	obj_class->finalize		= tllt_cp_create_user_dto_finalize;
	obj_class->get_property = tllt_cp_create_user_dto_get_property;
	obj_class->set_property = tllt_cp_create_user_dto_set_property;

	obj_properties[PROP_NAME] =
		g_param_spec_string("name", _("Name"), _("Name of the user"), NULL,
							G_PARAM_CONSTRUCT_ONLY | G_PARAM_PRIVATE | G_PARAM_READWRITE);
	obj_properties[PROP_EMAIL] =
		g_param_spec_string("email", _("Email"), _("Email of the user"), NULL,
							G_PARAM_CONSTRUCT_ONLY | G_PARAM_PRIVATE | G_PARAM_READWRITE);
	obj_properties[PROP_PASSWORD] =
		g_param_spec_string("password", _("Password"), _("Password of the user"), NULL,
							G_PARAM_CONSTRUCT_ONLY | G_PARAM_PRIVATE | G_PARAM_READWRITE);

	g_object_class_install_properties(obj_class, N_PROPS, obj_properties);
}

static void
tllt_cp_create_user_dto_init(G_GNUC_UNUSED TlltCpCreateUserDto *self)
{}

TlltCpCreateUserDto *
tllt_cp_create_user_dto_new(const char *name, const char *email, const char *password)
{
	return g_object_new(TLLT_CP_TYPE_CREATE_USER_DTO, "name", name, "email", email, "password",
						password, NULL);
}
