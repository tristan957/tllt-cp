#pragma once

#include <glib-object.h>
#include <gtk/gtk.h>

#include "tllt-cp.h"

G_BEGIN_DECLS

#define TLLT_CP_TYPE_NEW_RECIPE_WINDOW (tllt_cp_new_recipe_window_get_type())
G_DECLARE_FINAL_TYPE(TlltCpNewRecipeWindow, tllt_cp_new_recipe_window, TLLT_CP, NEW_RECIPE_WINDOW,
					 GtkWindow)

TlltCpNewRecipeWindow *tllt_cp_new_recipe_window_new(GtkWindow *parent, TlltCpUser *author);

G_END_DECLS
