if not get_option('lib-only')
  i18n.merge_file(
    input: 'com.gitlab.tristan957.TlltCp.desktop.in',
    output: 'com.gitlab.tristan957.TlltCp.desktop',
    type: 'desktop',
    po_dir: '../po',
    install: true,
    install_dir: join_paths(get_option('datadir'), 'applications')
  )

  i18n.merge_file(
    input: 'com.gitlab.tristan957.TlltCp.appdata.xml.in',
    output: 'com.gitlab.tristan957.TlltCp.appdata.xml',
    po_dir: '../po',
    install: true,
    install_dir: join_paths(get_option('datadir'), 'appdata')
  )
endif

install_data('com.gitlab.tristan957.TlltCp.gschema.xml',
  install_dir: join_paths(get_option('datadir'), 'glib-2.0/schemas')
)
