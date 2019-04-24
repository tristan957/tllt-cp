#include <stdarg.h>
#include <stddef.h>
#include <string.h>

#include <curl/curl.h>
#include <glib-object.h>
#include <glib/gi18n.h>
#include <json-glib/json-glib.h>

#include "tllt-cp-client.h"
#include "tllt-cp-config.h"
#include "tllt-cp-error.h"

G_DEFINE_TYPE(TlltCpClient, tllt_cp_client, G_TYPE_OBJECT)

enum TlltCpClientProps
{
	PROP_SERVER = 1,
	N_PROPS,
};

static GParamSpec *obj_properties[N_PROPS];

static void
tllt_cp_client_get_property(GObject *obj, guint prop_id, GValue *val, GParamSpec *pspec)
{
	TlltCpClient *self = TLLT_CP_CLIENT(obj);

	switch (prop_id) {
	case PROP_SERVER:
		g_value_set_string(val, self->server);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
		break;
	}
}

static void
tllt_cp_client_set_property(GObject *obj, guint prop_id, const GValue *val, GParamSpec *pspec)
{
	TlltCpClient *self = TLLT_CP_CLIENT(obj);

	switch (prop_id) {
	case PROP_SERVER:
		g_free(self->server);
		self->server = g_value_dup_string(val);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
		break;
	}
}

static void
tllt_cp_client_finalize(GObject *obj)
{
	TlltCpClient *self = TLLT_CP_CLIENT(obj);

	if (self->server != NULL) {
		g_free(self->server);
	}

	if (self->handle != NULL) {
		curl_global_cleanup();
		curl_easy_cleanup(self->handle);
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

	obj_properties[PROP_SERVER] = g_param_spec_string(
		"server", _("Server"), _("Location of the server"), NULL, G_PARAM_READWRITE);

	g_object_class_install_properties(obj_class, N_PROPS, obj_properties);
}

static void
tllt_cp_client_init(TlltCpClient *self)
{
	const CURLcode req_code = curl_global_init(CURL_GLOBAL_ALL);
	g_return_if_fail(req_code == CURLE_OK);
	self->handle = curl_easy_init();
	g_warn_if_fail(self->handle != NULL);
}

TlltCpClient *
tllt_cp_client_new_from_environment()
{
	const char *server = g_getenv("TLLT_CP_SERVER");

	g_return_val_if_fail(server != NULL, NULL);

	return g_object_new(TLLT_CP_TYPE_CLIENT, "server", server, NULL);
}

static gboolean
status_code_valid(const unsigned int status_code)
{
	if (status_code != 200 && status_code != 201 && status_code != 202) {
		return FALSE;
	}

	return TRUE;
}

GObject *
tllt_cp_client_get_request(TlltCpClient *self, const GType type, const char *endpoint, GError **err)
{
	g_return_val_if_fail(err == NULL || *err == NULL, NULL);

	TlltCpBuffer buffer		   = {.buf = NULL, .len = 0};
	struct curl_slist *headers = NULL;
	CURLcode req_code		   = 0;

	if ((req_code = curl_easy_setopt(self->handle, CURLOPT_URL, endpoint)) != CURLE_OK) {
		g_set_error(err, PACKAGE_DOMAIN, ERROR_CURL, "Failed to set CURLOPT_URL: %s",
					curl_easy_strerror(req_code));
		goto on_error;
	}
	if ((req_code = curl_easy_setopt(self->handle, CURLOPT_WRITEFUNCTION,
									 tllt_cp_client_write_cb)) != CURLE_OK) {
		g_set_error(err, PACKAGE_DOMAIN, ERROR_CURL, "Failed to set CURLOPT_WRITEFUNCTION: %s",
					curl_easy_strerror(req_code));
		goto on_error;
	}
	if ((req_code = curl_easy_setopt(self->handle, CURLOPT_WRITEDATA, &buffer)) != CURLE_OK) {
		g_set_error(err, PACKAGE_DOMAIN, ERROR_CURL, "Failed to set CURLOPT_WRITEDATA: %s",
					curl_easy_strerror(req_code));
		goto on_error;
	}
	if ((req_code = curl_easy_setopt(self->handle, CURLOPT_USERAGENT, "libcurl-agent/tllt-cp")) !=
		CURLE_OK) {
		g_set_error(err, PACKAGE_DOMAIN, ERROR_CURL, "Failed to set CURLOPT_USERAGENT: %s",
					curl_easy_strerror(req_code));
		goto on_error;
	}

	// Setting headers
	headers = NULL;
	headers = curl_slist_append(headers, "Accept: application/json");
	if (headers == NULL) {
		g_set_error(err, PACKAGE_DOMAIN, ERROR_CURL, "Failed to set request headers");
		goto on_error;
	}
	if ((req_code = curl_easy_setopt(self->handle, CURLOPT_HTTPHEADER, headers)) != CURLE_OK) {
		g_set_error(err, PACKAGE_DOMAIN, ERROR_CURL, "Failed to set CURLOPT_HEADERDATA: %s",
					curl_easy_strerror(req_code));
		goto on_error;
	}

	if ((req_code = curl_easy_perform(self->handle)) != CURLE_OK) {
		g_set_error(err, PACKAGE_DOMAIN, ERROR_CURL, "Failed to perform request: %s",
					curl_easy_strerror(req_code));
		goto on_error;
	}

	long status_code  = 0;
	CURLcode res_code = CURLE_OK;
	if ((res_code = curl_easy_getinfo(self->handle, CURLINFO_RESPONSE_CODE, &status_code)) !=
		CURLE_OK) {
		g_set_error(err, PACKAGE_DOMAIN, ERROR_CURL, "Failed to get status req_code: %s",
					curl_easy_strerror(res_code));
		goto on_error;
	}

	if (req_code == CURLE_ABORTED_BY_CALLBACK) {
		g_set_error(err, PACKAGE_DOMAIN, ERROR_CURL, "Memory error while reading response data");
		goto on_error;
	}

	if (!status_code_valid(status_code)) {
		g_set_error(err, PACKAGE_DOMAIN, ERROR_CURL, "Invalid response code of %ld", status_code);
		goto on_error;
	}

	curl_slist_free_all(headers);

	return json_gobject_from_data(type, buffer.buf, -1, err);

on_error:
	curl_slist_free_all(headers);
	return NULL;
}

GObject *
tllt_cp_client_post_request(TlltCpClient *self, const GType type, const char *endpoint,
							GObject *data, GError **err)
{
	g_return_val_if_fail(err == NULL || *err == NULL, NULL);

	TlltCpBuffer buffer		   = {.buf = NULL, .len = 0};
	struct curl_slist *headers = NULL;
	CURLcode req_code		   = 0;

	if ((req_code = curl_easy_setopt(self->handle, CURLOPT_URL, endpoint)) != CURLE_OK) {
		g_set_error(err, PACKAGE_DOMAIN, ERROR_CURL, "Failed to set CURLOPT_URL: %s",
					curl_easy_strerror(req_code));
		goto on_error;
	}
	if ((req_code = curl_easy_setopt(self->handle, CURLOPT_POST, TRUE)) != CURLE_OK) {
		g_set_error(err, PACKAGE_DOMAIN, ERROR_CURL, "Failed to set CURLOPT_POST: %s",
					curl_easy_strerror(req_code));
		goto on_error;
	}

	if (data != NULL) {
		gsize length	   = 0;
		char *deserialized = json_gobject_to_data(data, &length);
		if (deserialized == NULL || length == 0) {
			g_set_error(err, PACKAGE_DOMAIN, ERROR_JSON, "Failed to deserialize object to JSON");
			goto on_error;
		}

		if ((req_code = curl_easy_setopt(self->handle, CURLOPT_COPYPOSTFIELDS, deserialized)) !=
			CURLE_OK) {
			g_set_error(err, PACKAGE_DOMAIN, ERROR_CURL, "Failed to set CURLOPT_COPYPOSTFIELDS: %s",
						curl_easy_strerror(req_code));
			g_free(deserialized);
			goto on_error;
		}

		g_print("%s\n", deserialized);

		g_free(deserialized);
	}

	// if (type != G_TYPE_NONE) {
	if ((req_code = curl_easy_setopt(self->handle, CURLOPT_WRITEFUNCTION,
									 tllt_cp_client_write_cb)) != CURLE_OK) {
		g_set_error(err, PACKAGE_DOMAIN, ERROR_CURL, "Failed to set CURLOPT_WRITEFUNCTION: %s",
					curl_easy_strerror(req_code));
		goto on_error;
	}
	if ((req_code = curl_easy_setopt(self->handle, CURLOPT_WRITEDATA, &buffer)) != CURLE_OK) {
		g_set_error(err, PACKAGE_DOMAIN, ERROR_CURL, "Failed to set CURLOPT_WRITEDATA: %s",
					curl_easy_strerror(req_code));
		goto on_error;
	}
	// }

	if ((req_code = curl_easy_setopt(self->handle, CURLOPT_USERAGENT, "libcurl/tllt-cp")) !=
		CURLE_OK) {
		g_set_error(err, PACKAGE_DOMAIN, ERROR_CURL, "Failed to set CURLOPT_USERAGENT: %s",
					curl_easy_strerror(req_code));
		goto on_error;
	}

	// Setting headers
	headers = curl_slist_append(headers, "Accept: application/json");
	headers = curl_slist_append(headers, "Content-Type: application/json");
	headers = curl_slist_append(headers, "charsets: utf-8");
	if (headers == NULL) {
		g_set_error(err, PACKAGE_DOMAIN, ERROR_CURL, "Failed to set request headers");
		goto on_error;
	}
	if ((req_code = curl_easy_setopt(self->handle, CURLOPT_HTTPHEADER, headers)) != CURLE_OK) {
		g_set_error(err, PACKAGE_DOMAIN, ERROR_CURL, "Failed to set CURLOPT_HEADEROPT: %s",
					curl_easy_strerror(req_code));
		goto on_error;
	}

	if ((req_code = curl_easy_perform(self->handle)) != CURLE_OK) {
		g_set_error(err, PACKAGE_DOMAIN, ERROR_CURL, "Failed to perform request: %s",
					curl_easy_strerror(req_code));
		goto on_error;
	}

	long status_code  = 0;
	CURLcode res_code = CURLE_OK;
	if ((res_code = curl_easy_getinfo(self->handle, CURLINFO_RESPONSE_CODE, &status_code)) !=
		CURLE_OK) {
		g_set_error(err, PACKAGE_DOMAIN, ERROR_CURL, "Failed to get status req_code: %s",
					curl_easy_strerror(res_code));
		goto on_error;
	}

	if (req_code == CURLE_ABORTED_BY_CALLBACK) {
		g_set_error(err, PACKAGE_DOMAIN, ERROR_CURL, "Memory error while reading response data");
		goto on_error;
	}

	if (!status_code_valid(status_code)) {
		g_set_error(err, PACKAGE_DOMAIN, ERROR_CURL, "Invalid response code of %ld", status_code);
		goto on_error;
	}

	GObject *obj = NULL;
	if (type != G_TYPE_NONE) {
		obj = json_gobject_from_data(type, buffer.buf, -1, err);
	}

	curl_slist_free_all(headers);
	curl_easy_reset(self->handle);
	g_free(buffer.buf);

	return obj;

on_error:
	curl_slist_free_all(headers);
	curl_easy_reset(self->handle);
	g_free(buffer.buf);

	return NULL;
}

size_t
tllt_cp_client_write_cb(char *content, size_t size, size_t nmemb, void *user_data)
{
	const size_t real_size = size * nmemb;
	TlltCpBuffer *buffer   = user_data;

	if (buffer->buf == NULL) {
		buffer->buf = g_malloc(1);
		g_return_val_if_fail(buffer->buf != NULL, 0);
		buffer->buf[0] = 0;
		buffer->len	= 0;
	}

	const size_t new_len = strlen(buffer->buf) + buffer->len;
	buffer->buf			 = g_realloc(buffer->buf, new_len + real_size + 1);
	g_return_val_if_fail(buffer->buf != NULL, 0);

	memcpy(&buffer->buf[buffer->len], content, real_size);

	buffer->len += real_size;
	buffer->buf[buffer->len] = 0;

	return real_size;
}
