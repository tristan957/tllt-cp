if get_option('with-docs')

  subdir('xml')

  docpath = join_paths(get_option('prefix'), get_option('datadir'), 'gtk-doc/html')
  glib_prefix = libgio_dep.get_pkgconfig_variable('prefix')
  glib_docpath = join_paths(glib_prefix, 'share/gtk-doc/html')

  gnome.gtkdoc('libtllt-cp',
    main_sgml: 'libtllt-cp-docs.xml',
    src_dir: meson.source_root() + '/lib',
    gobject_typesfile: 'libtllt-cp.types',
    dependencies: libtllt_cp_dep,
    scan_args: [
      '--rebuild-types',
      '--ignore-headers=' + ' '.join(libtllt_cp_private_headers),
    ],
    fixxref_args: [
      '--html-dir=' + docpath,
      '--extra-dir=' + join_paths(glib_docpath, 'glib'),
      '--extra-dir=' + join_paths(glib_docpath, 'gobject'),
    ],
    install: true
  )

endif
