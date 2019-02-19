#pragma once

#include <glib-object.h>

G_BEGIN_DECLS

#define TLLT_CP_TYPE_USER (tllt_cp_user_get_type())
G_DECLARE_FINAL_TYPE(TlltCpUser, tllt_cp_user, TLLT_CP, USER, GObject)

struct _TlltCpUser
{
	GObject parent_instance;
	gchar *name;
	gchar *email;
};

TlltCpUser *tllt_cp_user_new(const gchar *name, const gchar *email, const guint user_id);

G_END_DECLS
