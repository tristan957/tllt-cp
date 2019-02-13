incdir = include_directories('../include')

files = [
	'main.c',
	'tllt-cp-application.c',
	'tllt-cp-header-bar.c',
	'tllt-cp-window.c',
]

client_resources = gnome.compile_resources(
	'tllt-cp-resources',
	'tllt-cp-resources.gresource.xml',
	source_dir: '../data',
	c_name: 'tllt-cp',
)

executable(
	'tllt-cp',
	files,
	dependencies: libgtk_dep,
	include_directories: incdir,
	install: true,
)
