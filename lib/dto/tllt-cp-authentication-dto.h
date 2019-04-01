#pragma once

#if !defined(__TLLT_CP_HEADER_INTERNAL__) && !defined(TLLT_CP_COMPILATION)
#	error "Only <tllt-cp.h> can be included directly."
#endif

#include <glib-object.h>

G_BEGIN_DECLS

#define TLLT_CP_TYPE_AUTHENTICATION_DTO (tllt_cp_authentication_dto_get_type())
G_DECLARE_FINAL_TYPE(TlltCpAuthenticationDto, tllt_cp_authentication_dto, TLLT_CP,
					 AUTHENTICATION_DTO, GObject)

TlltCpAuthenticationDto *tllt_cp_authentication_dto_new(const char *email, const char *password);

G_END_DECLS
