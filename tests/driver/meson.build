c_args = [
	'-DTLLT_CP_COMPILATION',
]

dependencies = [
	libgio_dep,
	libtllt_dep,
]

if libwiringPi_dep.found()
	c_args += '-DTLLT_WITH_WIRINGPI'
	dependencies += libwiringPi_dep
endif

test_thermistor = executable(
	'test-thermistor',
	[
		'test-thermistor.c'
	],
	c_args: c_args,
	dependencies: dependencies,
	include_directories: include_directories('../../driver'),
)

test_heating_elements = executable(
	'test-heating-elements',
	[
		'test-heating-elements.c'
	],
	c_args: c_args,
	dependencies: dependencies,
	include_directories: include_directories('../../driver'),
)

test_wiringPi = executable(
	'test-wiringPi',
	[
		'test-wiringPi.c'
	],
	c_args: c_args,
	dependencies: dependencies,
	include_directories: include_directories('../../driver'),
)
