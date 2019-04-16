#pragma once

#if !defined(__TLLT_CP_HEADER_INTERNAL__) && !defined(TLLT_CP_COMPILATION)
#	error "Only <tllt-cp.h> can be included directly."
#endif

#include <glib-object.h>

#include "tllt-cp-recipe.h"

G_BEGIN_DECLS

#define TLLT_CP_TYPE_CREATE_RECIPE_DTO (tllt_cp_create_recipe_dto_get_type())
G_DECLARE_FINAL_TYPE(TlltCpCreateRecipeDto, tllt_cp_create_recipe_dto, TLLT_CP, CREATE_RECIPE_DTO,
					 GObject)

TlltCpCreateRecipeDto *tllt_cp_create_recipe_dto_new(const char *name, const TlltCpRecipeType type);

G_END_DECLS
