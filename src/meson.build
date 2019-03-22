sources = [
	'main.c',
	'tllt-cp-application.c',
	'tllt-cp-recipe-sidebar.c',
	'tllt-cp-window.c',
	'login/tllt-cp-login-window.c'
]

resources = gnome.compile_resources(
	'tllt-cp-resources',
	'tllt-cp-resources.gresource.xml',
	source_dir: '../data',
	c_name: 'tllt_cp',
)

dependencies = [
	libgtk_dep,
	libtllt_cp_dep,
]

c_args = []

if get_option('with-driver')
	dependencies += libtllt_dep
	if get_option('with-gpio')
		c_args += '-DTLLT_GPIO'
	endif
endif

executable(
	'tllt-cp',
	sources + resources,
	c_args: c_args,
	dependencies: dependencies,
	gui_app: true,
	install: true,
)
