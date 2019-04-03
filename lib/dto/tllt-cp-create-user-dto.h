#pragma once

#if !defined(__TLLT_CP_HEADER_INTERNAL__) && !defined(TLLT_CP_COMPILATION)
#	error "Only <tllt-cp.h> can be included directly."
#endif

#include <glib-object.h>

G_BEGIN_DECLS

#define TLLT_CP_TYPE_CREATE_USER_DTO (tllt_cp_create_user_dto_get_type())
G_DECLARE_FINAL_TYPE(TlltCpCreateUserDto, tllt_cp_create_user_dto, TLLT_CP, CREATE_USER_DTO,
					 GObject)

TlltCpCreateUserDto *tllt_cp_create_user_dto_new(const char *name, const char *email,
												 const char *password);

G_END_DECLS
