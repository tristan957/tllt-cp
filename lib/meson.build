config_h = configuration_data({
	'PACKAGE_NAME': '"@0@"'.format(meson.project_name()),
	'PACKAGE_VERSION': '"@0@"'.format(meson.project_version()),
	'GETTEXT_PACKAGE': '"@0@"'.format(meson.project_name()),
	'G_LOG_USE_STRUCTURED': true,
	'LOCALEDIR': '"@0@"'.format(join_paths(get_option('prefix'), get_option('localedir'))),
	'LIBDIR': '"@0@"'.format(join_paths(get_option('prefix'), get_option('libdir'))),
})
if get_option('buildtype') == 'release'
	config_h.set('G_DISABLE_ASSERT', true)
	config_h.set('G_DISABLE_CHECKS', true)
	config_h.set('G_DISABLE_CAST_CHECKS', true)
elif get_option('buildtype').startswith('debug')
	config_h.set('G_ENABLE_DEBUG', true)
endif

configure_file(
	output: 'tllt-cp-config.h',
	configuration: config_h,
)

sources = [
	'tllt-cp-user.c',
]

public_headers = [
	'tllt-cp.h',
	'tllt-cp-user.h'
]

# install_headers(libtllt_cp_public_headers, subdir: 'tllt-cp')

pkgincludedir = join_paths(get_option('includedir'), 'tllt-cp')

libtllt_cp = shared_library(
	'tllt-cp',
  	sources,
	dependencies: [
		libgio_dep,
	],
  	install: true,
)

libtllt_cp_dep = declare_dependency(
	include_directories: include_directories('.'),
	link_with: libtllt_cp,
)
