#pragma once

#if !defined(__TLLT_CP_HEADER_INTERNAL__) && !defined(TLLT_CP_COMPILATION)
#	error "Only <tllt-cp.h> can be included directly."
#endif

#include <glib-object.h>

#include "tllt-cp-client.h"
#include "tllt-cp-user.h"

G_BEGIN_DECLS

#define TLLT_CP_TYPE_USER (tllt_cp_user_get_type())
G_DECLARE_FINAL_TYPE(TlltCpUser, tllt_cp_user, TLLT_CP, USER, GObject)

struct _TlltCpUser
{
	GObject parent_instance;
};

TlltCpUser *tllt_cp_user_new(const gchar *name, const gchar *email, const guint user_id);
TlltCpUser *tllt_cp_user_get_by_id(TlltCpClient *client, unsigned int id, GError **err);
TlltCpUser *tllt_cp_user_authenticate(TlltCpClient *client, const char *email, const char *password,
									  GError **err);
TlltCpUser *tllt_cp_user_create(TlltCpClient *client, const char *name, const char *email,
								const char *password, GError **err);
unsigned int tllt_cp_user_get_id(TlltCpUser *self);
char *tllt_cp_user_get_name(TlltCpUser *self);
char *tllt_cp_user_get_email(TlltCpUser *self);

G_END_DECLS
