sources = [
	'main.c',
	'tllt-cp-application.c',
	'tllt-cp-feedback-dialog.c',
	'tllt-cp-login-window.c',
	'tllt-cp-new-recipe-window.c',
	'tllt-cp-recipe-list-item.c',
	'tllt-cp-window.c',
]

executable(
	'tllt-cp',
	sources + resources,
	dependencies: [
		libgtk_dep,
		libtllt_cp_dep,
		libtllt_dep,
	],
	gui_app: true,
	install: true,
)
