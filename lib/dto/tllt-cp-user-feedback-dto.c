#include <glib-object.h>
#include <glib/gi18n.h>

#include "tllt-cp-user-feedback-dto.h"
#include "tllt-cp-user.h"

struct _TlltCpUserFeedbackDto
{
	GObject parent_instance;
};

typedef struct TlltCpUserFeedbackDtoPrivate
{
	TlltCpUserFeedback feedback;
} TlltCpUserFeedbackDtoPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(TlltCpUserFeedbackDto, tllt_cp_user_feedback_dto, G_TYPE_OBJECT)

typedef enum TlltCpUserFeedbackDtoProps
{
	PROP_FEEDBACK = 1,
	N_PROPS,
} TlltCpUserFeedbackDtoProps;

static GParamSpec *obj_properties[N_PROPS];

static void
tllt_cp_user_feedback_get_property(GObject *obj, guint prop_id, GValue *val, GParamSpec *pspec)
{
	TlltCpUserFeedbackDto *self		   = TLLT_CP_USER_FEEDBACK_DTO(obj);
	TlltCpUserFeedbackDtoPrivate *priv = tllt_cp_user_feedback_dto_get_instance_private(self);

	switch (prop_id) {
	case PROP_FEEDBACK:
		g_value_set_uint(val, priv->feedback);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
	}
}

static void
tllt_cp_user_feedback_set_property(GObject *obj, guint prop_id, const GValue *val,
								   GParamSpec *pspec)
{
	TlltCpUserFeedbackDto *self		   = TLLT_CP_USER_FEEDBACK_DTO(obj);
	TlltCpUserFeedbackDtoPrivate *priv = tllt_cp_user_feedback_dto_get_instance_private(self);

	switch (prop_id) {
	case PROP_FEEDBACK:
		priv->feedback = g_value_get_uint(val);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
	}
}

static void
tllt_cp_user_feedback_dto_class_init(TlltCpUserFeedbackDtoClass *klass)
{
	GObjectClass *obj_class = G_OBJECT_CLASS(klass);

	obj_class->get_property = tllt_cp_user_feedback_get_property;
	obj_class->set_property = tllt_cp_user_feedback_set_property;

	obj_properties[PROP_FEEDBACK] =
		g_param_spec_uint("feedback", _("Feedback"), _("User feedback"), 0, MORE_TOASTY, 0,
						  G_PARAM_PRIVATE | G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY);

	g_object_class_install_properties(obj_class, N_PROPS, obj_properties);
}

static void
tllt_cp_user_feedback_dto_init(G_GNUC_UNUSED TlltCpUserFeedbackDto *self)
{}

TlltCpUserFeedbackDto *
tllt_cp_user_feedback_new(const TlltCpUserFeedback feedback)
{
	return g_object_new(TLLT_CP_TYPE_USER_FEEDBACK_DTO, "feedback", feedback, NULL);
}
