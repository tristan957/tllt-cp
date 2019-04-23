#pragma once

#if !defined(__TLLT_CP_HEADER_INTERNAL__) && !defined(TLLT_CP_COMPILATION)
#	error "Only <tllt-cp.h> can be included directly."
#endif

#include <glib-object.h>

#include "tllt-cp-client.h"
#include "tllt-cp-recipe.h"
#include "tllt-cp-user.h"

G_BEGIN_DECLS

#define TLLT_CP_TYPE_USER (tllt_cp_user_get_type())
G_DECLARE_FINAL_TYPE(TlltCpUser, tllt_cp_user, TLLT_CP, USER, GObject)

struct _TlltCpUser
{
	GObject parent_instance;
};

typedef enum TlltCpUserFeedback
{
	LESS_TOASTY = 1,
	THE_SAME,
	MORE_TOASTY,
} TlltCpUserFeedback;

TlltCpUser *tllt_cp_user_get_by_id(TlltCpClient *client, unsigned int id, GError **err);
TlltCpUser *tllt_cp_user_authenticate(TlltCpClient *client, const char *email, const char *password,
									  GError **err);
TlltCpUser *tllt_cp_user_create(TlltCpClient *client, const char *name, const char *email,
								const char *password, GError **err);
unsigned int tllt_cp_user_get_id(TlltCpUser *self);
char *tllt_cp_user_get_name(TlltCpUser *self);
char *tllt_cp_user_get_email(TlltCpUser *self);
GList *tllt_cp_user_get_recipes(TlltCpUser *self);
TlltCpRecipe *tllt_cp_user_add_recipe(TlltCpUser *self, TlltCpClient *client, const char *name,
									  const TlltCpRecipeType type, GError **err);
void tllt_cp_user_remove_recipe(TlltCpUser *self, TlltCpRecipe *recipe, GError **err);
TlltCpCookingDetailsDto *tllt_cp_user_get_cooking_details_for_recipe(TlltCpUser *self,
																	 TlltCpClient *client,
																	 TlltCpRecipe *recipe,
																	 GError **err);
void tllt_cp_user_adjust_scale(TlltCpUser *self, TlltCpClient *client,
							   const TlltCpUserFeedback choice, GError **err);

G_END_DECLS
