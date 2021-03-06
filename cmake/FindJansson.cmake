# FindJansson
# -----------
#
# Find Jansson library, this modules defines:
#
# Jansson_INCLUDE_DIRS, where to find jansson.h
# Jansson_LIBRARIES, where to find library
# Jansson_FOUND, if it is found
#
# The following imported targets will be available:
#
# Jansson::Jansson, if found.
#

find_path(Jansson_INCLUDE_DIR NAMES jansson.h)
find_library(Jansson_LIBRARY NAMES libjansson jansson jansson_d libjansson_d)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(
	Jansson
	FOUND_VAR Jansson_FOUND
	REQUIRED_VARS Jansson_LIBRARY Jansson_INCLUDE_DIR
)

if (Jansson_FOUND)
	set(Jansson_LIBRARIES ${Jansson_LIBRARY})
	set(Jansson_INCLUDE_DIRS ${Jansson_INCLUDE_DIR})

	if (NOT TARGET Jansson::Jansson)
		add_library(Jansson::Jansson UNKNOWN IMPORTED)
		set_target_properties(
			Jansson::Jansson
			PROPERTIES
				IMPORTED_LINK_INTERFACE_LANGUAGES "C"
				IMPORTED_LOCATION "${Jansson_LIBRARY}"
				INTERFACE_INCLUDE_DIRECTORIES "${Jansson_INCLUDE_DIRS}"
		)
	endif ()
endif ()

mark_as_advanced(Jansson_INCLUDE_DIR Jansson_LIBRARY)
