project(
    'tllt-cp',
    ['c'],
    version: '0.0.1',
    license: [
        'GPL-3.0',
    ],
    default_options: [
        'c_std=c11',
    ],
)

libtllt_cp_api_version = '0.1'

c_flags = [
	'-Wall',
	'-Wextra',
	'--pedantic',
]

cc = meson.get_compiler('c')

add_project_arguments(c_flags, language: 'c')

i18n = import('i18n')
gnome = import('gnome')

libgio_dep = dependency('gio-2.0', version: '>= 2.56')
libcurl_dep = dependency('libcurl', version: '>= 7.61.1')
libjson_glib_dep = dependency('json-glib-1.0', version: '>= 1.4.2')
if not get_option('lib-only')
	libgtk_dep = dependency('gtk+-3.0', version: '>= 3.22')
endif

subdir('lib')

if get_option('with-driver')
	subdir('driver')
endif

if not get_option('lib-only')
	subdir('src')
endif

subdir('po')
subdir('data')
subdir('docs')

meson.add_install_script('meson_post_install.sh', '@0@'.format(get_option('lib-only')))
