c_args = [
	'-DTLLT_CP_COMPILATION',
]

test_powerable = executable(
	'test-powerable',
	[
		'test-powerable.c'
	],
	c_args: c_args,
	dependencies: [
		libgio_dep,
		libtllt_dep,
	],
	include_directories: include_directories('../../driver')
)

test('Test powerable', test_powerable)
