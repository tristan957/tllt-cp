config_h = configuration_data()
config_h.set_quoted('PACKAGE_NAME', meson.project_name())
config_h.set_quoted('PACKAGE_VERSION', meson.project_version())
config_h.set_quoted('GETTEXT_PACKAGE', meson.project_name())
config_h.set('G_LOG_DOMAIN', false)
config_h.set_quoted('G_LOG_DOMAIN', 'TlltCp')
config_h.set_quoted('LOCALEDIR', join_paths(get_option('prefix'), get_option('localedir')))
config_h.set_quoted('LIBDIR', join_paths(get_option('prefix'), get_option('libdir')))
config_h.set('G_LOG_USE_STRUCTURED', true)
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

libtllt_cp_sources = [
	'tllt-cp-ex.c',
]

libtllt_cp_public_headers = [
	'tllt-cp.h',
	'tllt-cp-ex.h'
]

# install_headers(libtllt_cp_public_headers, subdir: 'tllt-cp')

libtllt_cp_deps = [
	libgio_dep,
]

pkgincludedir = join_paths(get_option('includedir'), 'tllt-cp')

libtllt_cp_marshal = gnome.genmarshal(
	'tllt-cp-marshal',
	sources: 'tllt-cp-marshal.list',
	prefix: 'tllt_cp_marshal',
	# install_header: true,
	internal: true,
	install_dir: pkgincludedir,
)

libtllt_cp_enums = gnome.mkenums(
	'tllt-cp-enumtypes',
	sources: libtllt_cp_public_headers,
	c_template: 'tllt-cp-enumtypes.c.template',
	h_template: 'tllt-cp-enumtypes.h.template',
	# install_header: true,
	install_dir: pkgincludedir,
)

libtllt_cp_gen_headers = [
	libtllt_cp_marshal[1],
	libtllt_cp_enums[1],
]

libtllt_cp_gen_sources = [
	libtllt_cp_marshal[0],
	libtllt_cp_enums[0],
]

libtllt_cp_cflags = [
  '-include', 'tllt-cp-config.h'
]

libtllt_cp_link_flags = [
  '-Wl,--version-script,' + join_paths(meson.current_source_dir(), 'tllt-cp.map'),
]

libtllt_cp = shared_library(
	'tllt-cp',
  	libtllt_cp_gen_headers + libtllt_cp_gen_sources + libtllt_cp_sources,
  	c_args: libtllt_cp_cflags,
  	link_args: libtllt_cp_link_flags,
  	link_depends: 'tllt-cp.map',
  	dependencies: libtllt_cp_deps,
  	install: true,
)

libtllt_cp_dep = declare_dependency(
	sources: libtllt_cp_gen_headers,
	dependencies: libgio_dep,
	link_with: libtllt_cp,
)

libirc_gir = gnome.generate_gir(
	libtllt_cp,
	sources: libtllt_cp_sources + libtllt_cp_public_headers,
	nsversion: libtllt_cp_api_version,
	namespace: 'TlltCp',
	symbol_prefix: 'tllt_cp',
	identifier_prefix: 'TlltCp',
	install: true,
	includes: 'Gio-2.0',
	extra_args: [
		'--c-include=tllt-cp.h',
	],
)

if get_option('with-vapi')

  gnome.generate_vapi('libtllt-cp-' + libirc_api_version,
    sources: libtllt_cp_gir[0],
    packages: 'gio-2.0',
    install: true
  )

endif
