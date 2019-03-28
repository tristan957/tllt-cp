sources = [
	'sensors/tllt-scale.c',
	'sensors/tllt-sensor.c',
	'sensors/tllt-thermistor.c',
	'tllt-toaster.c',
]

public_headers = [
	'sensors/tllt-scale.h',
	'sensors/tllt-sensor.h',
	'sensors/tllt-thermistor.h',
	'tllt-toaster.h',
]

libtllt_version = '1.0.0'

# install_headers(libtllt_public_headers, subdir: 'tllt')

pkgincludedir = join_paths(get_option('includedir'), 'tllt')

libtllt = library(
	'tllt',
  	sources,
	version: libtllt_version,
	dependencies: [
		libgio_dep,
	],
  	install: true,
)

libtllt_dep = declare_dependency(
	include_directories: include_directories('.'),
	link_with: libtllt,
	version: libtllt_version,
)
