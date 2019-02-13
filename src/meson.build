sources = [
	'main.c',
	'tllt-cp-application.c',
	'tllt-cp-header-bar.c',
	'tllt-cp-window.c',
]

resources = gnome.compile_resources(
	'tllt-cp-resources',
	'tllt-cp-resources.gresource.xml',
	source_dir: '../data',
	c_name: 'tllt_cp',
)

cflags = [
  '-include', 'tllt-cp-config.h'
]

executable(
	'tllt-cp',
	sources + resources,
	dependencies: [
		libgtk_dep,
		libtllt_cp_dep,
	],
	c_args: cflags,
	install: true,
)
