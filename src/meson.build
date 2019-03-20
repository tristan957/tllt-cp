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

pthread_dep = cc.find_library('pthread')

dependencies = [
	libgtk_dep,
	libtllt_cp_dep,
	pthread_dep,
]

if get_option('with-driver')
	dependencies += libtllt_dep
endif

executable(
	'tllt-cp',
	sources + resources,
	dependencies: dependencies,
	include_directories: include_directories('login'),
	gui_app: true,
	install: true,
)
