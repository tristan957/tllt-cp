#include <limits.h>

#include <glib-object.h>
#include <glib/gi18n.h>

#include "tllt-cp-cooking-details-dto.h"

G_DEFINE_TYPE(TlltCpCookingDetailsDto, tllt_cp_cooking_details_dto, G_TYPE_OBJECT)

typedef enum TlltCpCookingDetailsDtoProps
{
	PROP_TIME = 1,
	PROP_TEMPERATURE,
	N_PROPS,
} TlltCpCookingDetailsDtoProps;

static GParamSpec *obj_properties[N_PROPS];

static void
tllt_cp_cooking_details_dto_get_property(GObject *obj, guint prop_id, GValue *val,
										 GParamSpec *pspec)
{
	TlltCpCookingDetailsDto *self = TLLT_CP_COOKING_DETAILS_DTO(obj);

	switch (prop_id) {
	case PROP_TIME:
		g_value_set_uint(val, self->time);
		break;
	case PROP_TEMPERATURE:
		g_value_set_uint(val, self->time);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
	}
}

static void
tllt_cp_cooking_details_dto_set_property(GObject *obj, guint prop_id, const GValue *val,
										 GParamSpec *pspec)
{
	TlltCpCookingDetailsDto *self = TLLT_CP_COOKING_DETAILS_DTO(obj);

	switch (prop_id) {
	case PROP_TIME:
		self->time = g_value_get_uint(val);
		break;
	case PROP_TEMPERATURE:
		self->temperature = g_value_get_uint(val);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
	}
}

static void
tllt_cp_cooking_details_dto_class_init(TlltCpCookingDetailsDtoClass *klass)
{
	GObjectClass *obj_class = G_OBJECT_CLASS(klass);

	obj_class->get_property = tllt_cp_cooking_details_dto_get_property;
	obj_class->set_property = tllt_cp_cooking_details_dto_set_property;

	obj_properties[PROP_TIME] =
		g_param_spec_uint("time", _("Time"), _("Time to cook the recipe for"), 0, UINT_MAX, 0,
						  G_PARAM_CONSTRUCT_ONLY | G_PARAM_WRITABLE);
	obj_properties[PROP_TEMPERATURE] =
		g_param_spec_uint("temperature", _("Temperature"), _("Temperature to cook the recipe at"),
						  250, UINT_MAX, 250, G_PARAM_CONSTRUCT_ONLY | G_PARAM_WRITABLE);

	g_object_class_install_properties(obj_class, N_PROPS, obj_properties);
}

static void
tllt_cp_cooking_details_dto_init(G_GNUC_UNUSED TlltCpCookingDetailsDto *self)
{}
