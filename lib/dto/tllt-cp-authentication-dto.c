#include <glib-object.h>
#include <glib/gi18n.h>

#include "tllt-cp-authentication-dto.h"

struct _TlltCpAuthenticationDto
{
	GObject parent_instance;
};

typedef struct TlltCpAuthenticationDtoPrivate
{
	char *email;
	char *password;
} TlltCpAuthenticationDtoPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(TlltCpAuthenticationDto, tllt_cp_authentication_dto, G_TYPE_OBJECT)

typedef enum TlltCpAuthenticationDtoProps
{
	PROP_EMAIL = 1,
	PROP_PASSWORD,
	N_PROPS,
} TlltCpAuthenticationDtoProps;

static GParamSpec *obj_properties[N_PROPS];

static void
tllt_cp_authentication_dto_finalize(GObject *obj)
{
	TlltCpAuthenticationDto *self		 = TLLT_CP_AUTHENTICATION_DTO(obj);
	TlltCpAuthenticationDtoPrivate *priv = tllt_cp_authentication_dto_get_instance_private(self);

	g_free(priv->email);
	g_free(priv->password);

	G_OBJECT_CLASS(tllt_cp_authentication_dto_parent_class)->finalize(obj);
}

static void
tllt_cp_authentication_dto_get_property(GObject *obj, guint prop_id, GValue *val, GParamSpec *pspec)
{
	TlltCpAuthenticationDto *self		 = TLLT_CP_AUTHENTICATION_DTO(obj);
	TlltCpAuthenticationDtoPrivate *priv = tllt_cp_authentication_dto_get_instance_private(self);

	switch (prop_id) {
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
tllt_cp_authentication_dto_set_property(GObject *obj, guint prop_id, const GValue *val,
										GParamSpec *pspec)
{
	TlltCpAuthenticationDto *self		 = TLLT_CP_AUTHENTICATION_DTO(obj);
	TlltCpAuthenticationDtoPrivate *priv = tllt_cp_authentication_dto_get_instance_private(self);

	switch (prop_id) {
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
tllt_cp_authentication_dto_class_init(TlltCpAuthenticationDtoClass *klass)
{
	GObjectClass *obj_class = G_OBJECT_CLASS(klass);

	obj_class->finalize		= tllt_cp_authentication_dto_finalize;
	obj_class->get_property = tllt_cp_authentication_dto_get_property;
	obj_class->set_property = tllt_cp_authentication_dto_set_property;

	obj_properties[PROP_EMAIL] =
		g_param_spec_string("email", _("Email"), _("Email of the user"), NULL,
							G_PARAM_CONSTRUCT_ONLY | G_PARAM_PRIVATE | G_PARAM_READWRITE);
	obj_properties[PROP_PASSWORD] =
		g_param_spec_string("password", _("Password"), _("Password of the user"), NULL,
							G_PARAM_CONSTRUCT_ONLY | G_PARAM_PRIVATE | G_PARAM_READWRITE);

	g_object_class_install_properties(obj_class, N_PROPS, obj_properties);
}

static void
tllt_cp_authentication_dto_init(G_GNUC_UNUSED TlltCpAuthenticationDto *self)
{}

TlltCpAuthenticationDto *
tllt_cp_authentication_dto_new(const char *email, const char *password)
{
	return g_object_new(TLLT_CP_TYPE_AUTHENTICATION_DTO, "email", email, "password", password,
						NULL);
}
