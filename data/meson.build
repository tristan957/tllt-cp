i18n.merge_file(
	input: 'com.gitlab.tristan957.tllt-cp.desktop.in',
	output: 'com.gitlab.tristan957.tllt-cp.desktop',
	type: 'desktop',
	po_dir: '../po',
	install: true,
	install_dir: join_paths(get_option('datadir'), 'applications')
)

i18n.merge_file(
	input: 'com.gitlab.tristan957.tllt-cp.appdata.xml.in',
	output: 'com.gitlab.tristan957.tllt-cp.appdata.xml',
	po_dir: '../po',
	install: true,
	install_dir: join_paths(get_option('datadir'), 'appdata')
)

resources = gnome.compile_resources(
	'tllt-cp-resources',
	'tllt-cp-resources.gresource.xml',
	source_dir: '../data',
	c_name: 'tllt_cp',
)

install_data('com.gitlab.tristan957.tllt-cp.gschema.xml',
	install_dir: join_paths(get_option('datadir'), 'glib-2.0/schemas')
)
