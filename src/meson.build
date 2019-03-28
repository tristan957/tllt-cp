sources = [
	'main.c',
	'login/tllt-cp-login-window.c',
	'tllt-cp-application.c',
	'tllt-cp-window.c',
]

resources = gnome.compile_resources(
	'tllt-cp-resources',
	'tllt-cp-resources.gresource.xml',
	source_dir: '../data',
	c_name: 'tllt_cp',
)

c_args = []

if get_option('with-gpio')
	c_args += '-DTLLT_GPIO'
endif

executable(
	'tllt-cp',
	sources + resources,
	c_args: c_args,
	dependencies: [
		libgtk_dep,
		libtllt_cp_dep,
		libtllt_dep,
	],
	gui_app: true,
	install: true,
)
