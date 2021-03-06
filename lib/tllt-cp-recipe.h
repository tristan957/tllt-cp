#pragma once

#if !defined(__TLLT_CP_HEADER_INTERNAL__) && !defined(TLLT_CP_COMPILATION)
#	error "Only <tllt-cp.h> can be included directly."
#endif

#include <glib-object.h>

#include "dto/tllt-cp-cooking-details-dto.h"
#include "tllt-cp-client.h"

G_BEGIN_DECLS

typedef enum TlltCpRecipeType
{
	TOAST = 1,
	PIZZA,
	BAGEL,
	POPTART,
} TlltCpRecipeType;

#define TLLT_CP_TYPE_RECIPE (tllt_cp_recipe_get_type())
G_DECLARE_FINAL_TYPE(TlltCpRecipe, tllt_cp_recipe, TLLT_CP, RECIPE, GObject)

TlltCpRecipe *tllt_cp_recipe_create(TlltCpClient *client, const char *name,
									const TlltCpRecipeType type, GError *err);
const char *tllt_cp_recipe_get_name(TlltCpRecipe *self);
void tllt_cp_recipe_set_name(TlltCpRecipe *self, const char *name);
TlltCpRecipeType tllt_cp_recipe_get_rtype(TlltCpRecipe *self);
unsigned int tllt_cp_recipe_get_id(TlltCpRecipe *self);

G_END_DECLS
