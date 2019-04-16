#pragma once

#if !defined(__TLLT_CP_HEADER_INTERNAL__) && !defined(TLLT_CP_COMPILATION)
#	error "Only <tllt-cp.h> can be included directly."
#endif

#include <glib-object.h>

#include "tllt-cp-client.h"

G_BEGIN_DECLS

typedef enum TlltCpRecipeType
{
	TOAST,
	PIZZA,
	BAGEL,
} TlltCpRecipeType;

#define TLLT_CP_TYPE_RECIPE (tllt_cp_recipe_get_type())
G_DECLARE_FINAL_TYPE(TlltCpRecipe, tllt_cp_recipe, TLLT_CP, RECIPE, GObject)

TlltCpRecipe *tllt_cp_recipe_create(TlltCpClient *client, const char *name,
									const TlltCpRecipeType type, GError *err);

G_END_DECLS
