ent_conf = configuration_data()
ent_conf.set('PACKAGE', 'The Learning Little Toaster Control Panel')
ent_conf.set('PACKAGE_BUGREPORT', 'https://gitlab.com/tristan957/tllt-cp/issues')
ent_conf.set('PACKAGE_NAME', 'tllt-cp')
ent_conf.set('PACKAGE_STRING', 'tllt-cp')
ent_conf.set('PACKAGE_TARNAME', 'tllt-cp-' + meson.project_version())
ent_conf.set('PACKAGE_URL', 'https://gitlab.com/tristan957/tllt-cp')
ent_conf.set('PACKAGE_VERSION', meson.project_version())
ent_conf.set('PACKAGE_API_VERSION', libtllt_cp_api_version)

configure_file(
	input: 'gtkdocentities.ent.in',
	output: 'gtkdocentities.ent',
	configuration: ent_conf,
)
