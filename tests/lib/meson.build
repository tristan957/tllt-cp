host = 'http://localhost:8080/api/v1'

env = [
	'TLLT_CP_SERVER=@0@'.format(host),
]

c_args = [
	'-DTLLT_CP_COMPILATION',
]

test_client = executable(
	'test-client',
	[
		'test-client.c'
	],
	c_args: c_args,
	dependencies: [
		libgio_dep,
		libtllt_cp_dep,
	],
	include_directories: include_directories('../../lib')
)

test('Test client', test_client, env: env)
