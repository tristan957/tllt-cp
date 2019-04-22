#pragma once

#include <glib-object.h>
#include <gtk/gtk.h>

#include "tllt-cp.h"

#define TLLT_CP_TYPE_RECIPE_LIST_ITEM (tllt_cp_recipe_list_item_get_type())
G_DECLARE_FINAL_TYPE(TlltCpRecipeListItem, tllt_cp_recipe_list_item, TLLT_CP, RECIPE_LIST_ITEM,
					 GtkBox)

TlltCpRecipeListItem *tllt_cp_recipe_list_item_new(TlltCpRecipe *recipe);
void tllt_cp_recipe_list_item_toggle_run_button(TlltCpRecipeListItem *self, gboolean state);
