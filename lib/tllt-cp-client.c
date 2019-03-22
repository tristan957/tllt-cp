#include <string.h>

#include <glib-object.h>
#include <glib/gi18n.h>

#include "tllt-cp-client.h"

struct _TlltCpClient
{
	GObject parent_instance;
};

typedef struct TlltCpClientPrivate
{
	char *server;
} TlltCpClientPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(TlltCpClient, tllt_cp_client, G_TYPE_OBJECT)

enum TlltCpClientProps
{
	PROP_SERVER = 1,
	N_PROPS,
};

static GParamSpec *obj_properties[N_PROPS];

static void
tllt_cp_client_get_property(GObject *obj, guint prop_id, GValue *val, GParamSpec *pspec)
{
	TlltCpClient *self		  = TLLT_CP_CLIENT(obj);
	TlltCpClientPrivate *priv = tllt_cp_client_get_instance_private(self);

	switch (prop_id) {
	case PROP_SERVER:
		g_value_set_string(val, priv->server);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
		break;
	}
}

static void
tllt_cp_client_set_property(GObject *obj, guint prop_id, const GValue *val, GParamSpec *pspec)
{
	TlltCpClient *self		  = TLLT_CP_CLIENT(obj);
	TlltCpClientPrivate *priv = tllt_cp_client_get_instance_private(self);

	switch (prop_id) {
	case PROP_SERVER:
		g_free(priv->server);
		priv->server = g_value_dup_string(val);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
		break;
	}
}

static void
tllt_cp_client_finalize(GObject *obj)
{
	TlltCpClient *self		  = TLLT_CP_CLIENT(obj);
	TlltCpClientPrivate *priv = tllt_cp_client_get_instance_private(self);

	if (priv->server != NULL) {
		g_free(priv->server);
	}

	G_OBJECT_CLASS(tllt_cp_client_parent_class)->finalize(obj);
}

static void
tllt_cp_client_class_init(TlltCpClientClass *klass)
{
	GObjectClass *obj_class = G_OBJECT_CLASS(klass);

	obj_class->finalize		= tllt_cp_client_finalize;
	obj_class->get_property = tllt_cp_client_get_property;
	obj_class->set_property = tllt_cp_client_set_property;

	obj_properties[PROP_SERVER] =
		g_param_spec_string("server", _("Server"), _("Location of the server"), NULL,
							G_PARAM_READWRITE | G_PARAM_PRIVATE | G_PARAM_CONSTRUCT_ONLY);

	g_object_class_install_properties(obj_class, N_PROPS, obj_properties);
}

static void
tllt_cp_client_init(G_GNUC_UNUSED TlltCpClient *self)
{}

TlltCpClient *
tllt_cp_client_new_from_environment()
{
	const char *server = g_getenv("TLLT_CP_SERVER");

	g_warn_if_fail(server != NULL);

	return g_object_new(TLLT_CP_TYPE_CLIENT, "server", server, NULL);
}
