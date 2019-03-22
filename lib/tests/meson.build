host = 'localhost'
port = 8080

env = [
	'TLLT_CP_SERVER=@0@:@1@'.format(host, port),
]

test_client = executable(
	'test-client',
	[
		'test-client.c'
	],
	dependencies: [
		libgio_dep,
		libtllt_cp_dep,
	],
	include_directories: include_directories('..')
)

test('Test client', test_client, env: env)
