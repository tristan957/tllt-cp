config_h = configuration_data({
	'PACKAGE_DOMAIN': '2',
})
if get_option('buildtype') == 'release'
	config_h.set('G_DISABLE_ASSERT', true)
	config_h.set('G_DISABLE_CHECKS', true)
	config_h.set('G_DISABLE_CAST_CHECKS', true)
elif get_option('buildtype').startswith('debug')
	config_h.set('G_ENABLE_DEBUG', true)
endif

sources = [
	'components/tllt-heating-element.c',
	'sensors/tllt-scale.c',
	'sensors/tllt-sensor.c',
	'sensors/tllt-thermistor.c',
	'tllt-driver.c',
	'tllt-powerable.c',
	'tllt-toaster.c',
]

public_headers = [
	'components/tllt-heating-element.h',
	'sensors/tllt-scale.h',
	'sensors/tllt-sensor.h',
	'sensors/tllt-thermistor.h',
	'tllt-driver.h',
	'tllt-powerable.h',
	'tllt-toaster.h',
]

libtllt_version = '1.0.0'

# install_headers(libtllt_public_headers, subdir: 'tllt')

pkgincludedir = join_paths(get_option('includedir'), 'tllt')

c_args = []
dependencies = [
	libgio_dep,
	libjson_glib_dep,
]

libwiringPi_dep = cc.find_library('wiringPi')

if libwiringPi_dep.found()
	config_h.set('TLLT_UNUSED', true)
	c_args += '-DTLLT_WITH_WIRINGPI'
	dependencies += libwiringPi_dep
else
	config_h.set('TLLT_UNUSED', 'G_GNUC_UNUSED')
endif

configure_file(
	output: 'tllt-config.h',
	configuration: config_h,
)

libtllt = library(
	'tllt',
  	sources,
	c_args: c_args,
	version: libtllt_version,
	dependencies: dependencies,
  	install: true,
)

libtllt_dep = declare_dependency(
	include_directories: include_directories('.'),
	link_with: libtllt,
	version: libtllt_version,
)
