sources = [
	'tllt-powerable.c',
	'tllt-readable.c',
	'tllt-scale.c',
	'tllt-thermistor.c',
	'tllt-toaster.c',
]

public_headers = [
	'tllt-powerable.h',
	'tllt-readable.h',
	'tllt-scale.h',
	'tllt-thermistor.h',
	'tllt-toaster.h',
]

# install_headers(libtllt_public_headers, subdir: 'tllt-driver')

pkgincludedir = join_paths(get_option('includedir'), 'tllt-driver')

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
