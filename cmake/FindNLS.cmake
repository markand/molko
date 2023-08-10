# FindNLS
# -------
#
# Find Jansson library, this modules defines:
#
# Jansson_INCLUDE_DIRS, where to find jansson.h
# Jansson_LIBRARIES, where to find library
# Jansson_FOUND, if it is found
#
# The following imported targets will be available:
#
# NLS::gettext, if found.
# NLS::msgfmt, if found.
# NLS::msgmerge, if found.
# NLS::xgettext, if found.
#

find_program(GETTEXT_EXECUTABLE gettext)
find_program(MSGFMT_EXECUTABLE msgfmt)
find_program(MSGMERGE_EXECUTABLE msgmerge)
find_program(XGETTEXT_EXECUTABLE xgettext)

if (GETTEXT_EXECUTABLE)
	add_executable(NLS::gettext IMPORTED GLOBAL)
	set_target_properties(NLS::gettext PROPERTIES
		IMPORTED_LOCATION "${GETTEXT_EXECUTABLE}"
	)
endif ()

if (MSGFMT_EXECUTABLE)
	add_executable(NLS::msgfmt IMPORTED GLOBAL)
	set_target_properties(NLS::msgfmt PROPERTIES
		IMPORTED_LOCATION "${MSGFMT_EXECUTABLE}"
	)
endif ()

if (MSGMERGE_EXECUTABLE)
	add_executable(NLS::msgmerge IMPORTED GLOBAL)
	set_target_properties(NLS::msgmerge PROPERTIES
		IMPORTED_LOCATION "${MSGMERGE_EXECUTABLE}"
	)
endif ()

if (XGETTEXT_EXECUTABLE)
	add_executable(NLS::xgettext IMPORTED GLOBAL)
	set_target_properties(NLS::xgettext PROPERTIES
		IMPORTED_LOCATION "${XGETTEXT_EXECUTABLE}"
	)
endif ()

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(
	NLS
	FOUND_VAR NLS_FOUND
	REQUIRED_VARS GETTEXT_EXECUTABLE MSGFMT_EXECUTABLE MSGMERGE_EXECUTABLE XGETTEXT_EXECUTABLE
)
