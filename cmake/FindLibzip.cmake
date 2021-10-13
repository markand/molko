# FindLibzip
# -----------
#
# Find Libzip library, this modules defines:
#
# Libzip_INCLUDE_DIRS, where to find libzip.h
# Libzip_LIBRARIES, where to find library
# Libzip_FOUND, if it is found
#
# The following imported targets will be available:
#
# Libzip::Libzip, if found.
#

find_path(Libzip_INCLUDE_DIR NAMES zip.h)
find_library(Libzip_LIBRARY NAMES libzip zip)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(
	Libzip
	FOUND_VAR Libzip_FOUND
	REQUIRED_VARS Libzip_LIBRARY Libzip_INCLUDE_DIR
)

if (Libzip_FOUND)
	set(Libzip_LIBRARIES ${Libzip_LIBRARY})
	set(Libzip_INCLUDE_DIRS ${Libzip_INCLUDE_DIR})

	if (NOT TARGET Libzip::Libzip)
		add_library(Libzip::Libzip UNKNOWN IMPORTED)
		set_target_properties(
			Libzip::Libzip
			PROPERTIES
				IMPORTED_LINK_INTERFACE_LANGUAGES "C"
				IMPORTED_LOCATION "${Libzip_LIBRARY}"
				INTERFACE_INCLUDE_DIRECTORIES "${Libzip_INCLUDE_DIRS}"
		)
	endif ()
endif ()

mark_as_advanced(Libzip_INCLUDE_DIR Libzip_LIBRARY)
