#!/bin/sh

if [ -z $DESTDIR ]; then
	echo $MESON_INSTALL_PREFIX
	install_prefix="${MESON_INSTALL_PREFIX:-.local}"
	echo 'Compiling GSchema'
	glib-compile-schemas "/usr/share/glib-2.0/schemas"
	if [ "$1" = "false" ] # Was it lib-only?
	then
		echo 'Updating icon cache'
		gtk-update-icon-cache -qtf "/usr/share/icons/hicolor"
		echo 'Updating desktop database'
		update-desktop-database -q "/usr/share/applications"
	fi
fi
