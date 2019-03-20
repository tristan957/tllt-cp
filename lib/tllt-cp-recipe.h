#pragma once

#include <glib-object.h>

G_BEGIN_DECLS

#define TLLT_CP_TYPE_RECIPE (tllt_cp_recipe_get_type())
G_DECLARE_FINAL_TYPE(TlltCpRecipe, tllt_cp_recipe, TLLT_CP, RECIPE, GObject)

TlltCpRecipe *tllt_cp_recipe_new();

G_END_DECLS
