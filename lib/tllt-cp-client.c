#include <glib-object.h>
#include <glib/gi18n.h>

#include "tllt-cp-client.h"

struct _TlltCpClient
{
	GObject parent_instance;
};

typedef struct TlltCpClientPrivate
{
	char *host;
	char *port;
} TlltCpClientPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(TlltCpClient, tllt_cp_client, G_TYPE_OBJECT)

enum TlltCpClientProps
{
	PROP_HOST = 1,
	PROP_PORT,
	N_PROPS
};

static GParamSpec *obj_properties[N_PROPS];

static void
tllt_cp_client_get_property(GObject *obj, guint prop_id, GValue *val, GParamSpec *pspec)
{
	TlltCpClient *self		  = TLLT_CP_CLIENT(obj);
	TlltCpClientPrivate *priv = tllt_cp_client_get_instance_private(self);

	switch (prop_id) {
	case PROP_HOST:
		g_value_set_string(val, priv->host);
		break;
	case PROP_PORT:
		g_value_set_string(val, priv->port);
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
	case PROP_HOST:
		g_free(priv->host);
		priv->host = g_value_dup_string(val);
		break;
	case PROP_PORT:
		g_free(priv->port);
		priv->port = g_value_dup_string(val);
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

	g_free(priv->host);
	g_free(priv->host);
}

static void
tllt_cp_client_class_init(TlltCpClientClass *klass)
{
	GObjectClass *obj_class = G_OBJECT_CLASS(klass);

	obj_class->finalize		= tllt_cp_client_finalize;
	obj_class->get_property = tllt_cp_client_get_property;
	obj_class->set_property = tllt_cp_client_set_property;

	obj_properties[PROP_HOST] =
		g_param_spec_string("host", _("Host"), _("Server host"), NULL, G_PARAM_READWRITE);
	obj_properties[PROP_PORT] =
		g_param_spec_string("port", _("Port"), _("Server port"), NULL, G_PARAM_READWRITE);

	g_object_class_install_properties(obj_class, N_PROPS, obj_properties);
}

static void
tllt_cp_client_init(G_GNUC_UNUSED TlltCpClient *self)
{}

TlltCpClient *
tllt_cp_client_new_from_environment()
{
	const char *host = g_getenv("TLLT_CP_HOST");
	const char *port = g_getenv("TLLT_CP_PORT");

	g_warn_if_fail(host != NULL);
	g_warn_if_fail(port != NULL);

	return g_object_new(TLLT_CP_TYPE_CLIENT, "host", host, "port", port, NULL);
}
