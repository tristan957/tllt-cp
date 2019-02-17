sources = [
	'main.c',
	'tllt-cp-application.c',
	'tllt-cp-window.c',
]

resources = gnome.compile_resources(
	'tllt-cp-resources',
	'tllt-cp-resources.gresource.xml',
	source_dir: '../data',
	c_name: 'tllt_cp',
)

executable(
	'tllt-cp',
	sources + resources,
	dependencies: [
		libgtk_dep,
		libtllt_cp_dep,
	],
	install: true,
)
