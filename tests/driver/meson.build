c_args = [
	'-DTLLT_CP_COMPILATION',
	'-DTLLT_WITH_WIRINGPI',
]

dependencies = [
	libgio_dep,
	libtllt_dep,
	libwiringPi_dep,
]

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

test_thermistor_raw = executable(
	'test-thermistor-raw',
	[
		'test-thermistor-raw.c'
	],
	c_args: c_args,
	dependencies: dependencies,
	include_directories: include_directories('../../driver'),
)
