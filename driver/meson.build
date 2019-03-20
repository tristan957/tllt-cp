sources = [
	'sensors/ifaces/tllt-powerable.c',
	'sensors/ifaces/tllt-readable.c',
	'sensors/tllt-scale.c',
	'sensors/tllt-thermistor.c',
	'tllt-toaster.c',
]

public_headers = [
	'sensors/ifaces/tllt-powerable.h',
	'sensors/ifaces/tllt-readable.h',
	'sensors/tllt-scale.h',
	'sensors/tllt-thermistor.h',
	'tllt-toaster.h',
]

# install_headers(libtllt_public_headers, subdir: 'tllt')

pkgincludedir = join_paths(get_option('includedir'), 'tllt')

libtllt = shared_library(
	'tllt',
  	sources,
	dependencies: [
		libgio_dep,
	],
  	install: true,
)

libtllt_dep = declare_dependency(
	include_directories: include_directories('.'),
	link_with: libtllt,
)
