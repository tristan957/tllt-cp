#pragma once

#if !defined(__TLLT_CP_HEADER_INTERNAL__) && !defined(TLLT_CP_COMPILATION)
#	error "Only <tllt-cp.h> can be included directly."
#endif

#include <glib-object.h>

G_BEGIN_DECLS

struct _TlltCpCookingDetailsDto
{
	GObject parent_instance;

	unsigned int time;
	unsigned int temperature;
};

#define TLLT_CP_TYPE_COOKING_DETAILS_DTO (tllt_cp_cooking_details_dto_get_type())
G_DECLARE_FINAL_TYPE(TlltCpCookingDetailsDto, tllt_cp_cooking_details_dto, TLLT_CP,
					 COOKING_DETAILS_DTO, GObject)

G_END_DECLS
