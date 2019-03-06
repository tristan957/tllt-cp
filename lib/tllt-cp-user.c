#include <glib-object.h>
#include <glib/gi18n.h>
#include <limits.h>

#include "tllt-cp-user.h"

typedef struct TlltCpUserPrivate
{
	guint user_id;
} TlltCpUserPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(TlltCpUser, tllt_cp_user, G_TYPE_OBJECT)

enum TlltCpUserProperties
{
	PROP_NAME = 1,
	PROP_EMAIL,
	PROP_USER_ID,
	N_PROPS
};

static GParamSpec *obj_properties[N_PROPS];

static void
tllt_cp_user_get_property(GObject *obj, guint prop_id, GValue *val, GParamSpec *pspec)
{
	TlltCpUser *self		= TLLT_CP_USER(obj);
	TlltCpUserPrivate *priv = tllt_cp_user_get_instance_private(self);

	switch (prop_id) {
	case PROP_NAME:
		g_value_set_string(val, self->name);
		break;
	case PROP_EMAIL:
		g_value_set_string(val, self->email);
		break;
	case PROP_USER_ID:
		g_value_set_uint(val, priv->user_id);
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
		g_free(self->name);
		self->name = g_value_dup_string(val);
		break;
	case PROP_EMAIL:
		g_free(self->email);
		self->email = g_value_dup_string(val);
		break;
	case PROP_USER_ID:
		priv->user_id = g_value_get_uint(val);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
		break;
	}
}

static void
tllt_cp_user_class_init(TlltCpUserClass *klass)
{
	GObjectClass *obj_class = G_OBJECT_CLASS(klass);

	obj_class->get_property = tllt_cp_user_get_property;
	obj_class->set_property = tllt_cp_user_set_property;

	obj_properties[PROP_NAME]  = g_param_spec_string("name", _("Name"), _("Name of the user"), NULL,
													 G_PARAM_CONSTRUCT | G_PARAM_READWRITE);
	obj_properties[PROP_EMAIL] = g_param_spec_string("email", _("Email"), _("Email of the user"),
													 NULL, G_PARAM_CONSTRUCT | G_PARAM_READWRITE);
	obj_properties[PROP_USER_ID] =
		g_param_spec_uint("user-id", _("User ID"), _("ID of the user"), 0, UINT_MAX, 0,
						  G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);

	g_object_class_install_properties(obj_class, N_PROPS, obj_properties);
}

static void
tllt_cp_user_init(G_GNUC_UNUSED TlltCpUser *self)
{}

TlltCpUser *
tllt_cp_user_new(const gchar *name, const gchar *email, const guint user_id)
{
	return g_object_new(TLLT_CP_TYPE_USER, "name", name, "email", email, "user-id", user_id, NULL);
}
