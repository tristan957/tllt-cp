#pragma once

#if !defined(__TLLT_CP_HEADER_INTERNAL__) && !defined(TLLT_CP_COMPILATION)
#	error "Only <tllt-cp.h> can be included directly."
#endif

#include <glib-object.h>

G_BEGIN_DECLS

#define TLLT_CP_TYPE_RECIPE (tllt_cp_recipe_get_type())
G_DECLARE_FINAL_TYPE(TlltCpRecipe, tllt_cp_recipe, TLLT_CP, RECIPE, GObject)

TlltCpRecipe *tllt_cp_recipe_new();

G_END_DECLS
