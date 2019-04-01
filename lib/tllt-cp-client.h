#pragma once

#if !defined(__TLLT_CP_HEADER_INTERNAL__) && !defined(TLLT_CP_COMPILATION)
#	error "Only <tllt-cp.h> can be included directly."
#endif

#include <curl/curl.h>
#include <glib-object.h>

G_BEGIN_DECLS

typedef struct TlltCpBuffer
{
	char *buf;
	size_t len;
} TlltCpBuffer;

struct _TlltCpClient
{
	GObject parent_instance;
	CURL *handle;
	char *server;
};

#define TLLT_CP_TYPE_CLIENT (tllt_cp_client_get_type())
G_DECLARE_FINAL_TYPE(TlltCpClient, tllt_cp_client, TLLT_CP, CLIENT, GObject)

TlltCpClient *tllt_cp_client_new_from_environment();
GObject *tllt_cp_client_get_request(TlltCpClient *self, const GType type, const char *endpoint,
									GError **err) G_GNUC_WARN_UNUSED_RESULT;
size_t tllt_cp_client_write_cb(char *ptr, size_t size, size_t nmemb, void *user_data);

G_END_DECLS
