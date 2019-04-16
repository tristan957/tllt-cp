project(
    'tllt-cp',
    ['c'],
    version: '1.0.0',
    license: [
        'GPL-3.0',
    ],
    default_options: [
        'c_std=gnu11',
    ],
)

libtllt_cp_api_version = '1.0.0'

c_args = [
	'-Wall',
	'-Wextra',
	'--pedantic',
]

cc = meson.get_compiler('c')

add_project_arguments(c_args, language: 'c')

i18n = import('i18n')
gnome = import('gnome')

libgio_dep = dependency('gio-2.0', version: '>= 2.50', required: true)
libcurl_dep = dependency('libcurl', version: '>= 7.52', required: true)
libjson_glib_dep = dependency('json-glib-1.0', version: '>= 1.2', required: true)
libgtk_dep = dependency('gtk+-3.0', version: '>= 3.22', required: true)
libmath_dep = cc.find_library('m', required: true)
libwiringPi_dep = cc.find_library('wiringPi')

subdir('data')
subdir('driver')
subdir('lib')
subdir('src')
subdir('po')

if get_option('with-docs')
    subdir('docs')
endif

if get_option('with-tests')
	subdir('tests')
endif

meson.add_install_script('meson_post_install.sh')
