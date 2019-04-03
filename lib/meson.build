config_h = configuration_data({
	'PACKAGE_NAME': '"@0@"'.format('The Learning Little Toaster Control Panel'),
	'PACKAGE_DOMAIN': '1',
	'PACKAGE_VERSION': '"@0@"'.format(meson.project_version()),
	'GETTEXT_PACKAGE': '"@0@"'.format(meson.project_name()),
	'G_LOG_USE_STRUCTURED': true,
	'LOCALEDIR': '"@0@"'.format(join_paths(get_option('prefix'), get_option('localedir'))),
	'LIBDIR': '"@0@"'.format(join_paths(get_option('prefix'), get_option('libdir'))),
	'PACKAGE_LICENSE': 'GTK_LICENSE_GPL_3_0_ONLY',
	'PACKAGE_WEBSITE': '"@0@"'.format('https://gitlab.com/tristan957/tllt-cp'),
	'PACKAGE_WEBSITE_LABEL': '"@0@"'.format('Repository'),
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
	'dto/tllt-cp-authentication-dto.c',
	'dto/tllt-cp-create-user-dto.c',
	'tllt-cp-client.c',
	'tllt-cp-error.c',
	'tllt-cp-recipe.c',
	'tllt-cp-user.c',
]

public_headers = [
	'dto/tllt-cp-authentication-dto.h',
	'tllt-cp.h',
	'tllt-cp-client.h',
	'tllt-cp-error.h',
	'tllt-cp-recipe.h',
	'tllt-cp-user.h',
]

# install_headers(libtllt_cp_public_headers, subdir: 'tllt-cp')

pkgincludedir = join_paths(get_option('includedir'), 'tllt-cp')

libtllt_cp_version = '1.0.0'

c_args = [
	'-DTLLT_CP_COMPILATION',
]

libtllt_cp = library(
	'tllt-cp',
  	sources,
	c_args: c_args,
	version: libtllt_cp_version,
	dependencies: [
		libgio_dep,
		libcurl_dep,
		libjson_glib_dep,
	],
  	install: true,
)

libtllt_cp_dep = declare_dependency(
	include_directories: include_directories('.'),
	link_with: libtllt_cp,
	version: libtllt_cp_version
)
