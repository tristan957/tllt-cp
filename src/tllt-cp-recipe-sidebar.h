#pragma once

#include <gtk/gtk.h>

#include "tllt-cp-user.h"

G_BEGIN_DECLS

#define TLLT_CP_TYPE_RECIPE_SIDEBAR (tllt_cp_recipe_sidebar_get_type())
G_DECLARE_FINAL_TYPE(TlltCpRecipeSidebar, tllt_cp_recipe_sidebar, TLLT_CP, RECIPE_SIDEBAR, GtkBox)

TlltCpRecipeSidebar *tllt_cp_recipe_sidebar_new();
void tllt_cp_recipe_sidebar_clear(TlltCpRecipeSidebar *self);
void tllt_cp_recipe_sidebar_set_user(TlltCpRecipeSidebar *self, TlltCpUser *user);

G_END_DECLS
