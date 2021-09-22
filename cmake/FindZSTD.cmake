# FindZSTD
# --------
#
# Find ZSTD library, this modules defines:
#
# ZSTD_INCLUDE_DIRS, where to find zstd.h
# ZSTD_LIBRARIES, where to find library
# ZSTD_FOUND, if it is found
# ZSTD_EXE, path to zstd executable if found
#
# The following imported targets will be available:
#
# ZSTD::ZSTD, if found.
# ZSTD::exe, alias to ZSTD_EXE
#

find_path(ZSTD_INCLUDE_DIR NAMES zstd.h)
find_library(ZSTD_LIBRARY NAMES libzstd zstd)
find_program(ZSTD_EXE NAMES zstd)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(
	ZSTD
	FOUND_VAR ZSTD_FOUND
	REQUIRED_VARS ZSTD_LIBRARY ZSTD_INCLUDE_DIR
)

if (ZSTD_FOUND)
	set(ZSTD_LIBRARIES ${ZSTD_LIBRARY})
	set(ZSTD_INCLUDE_DIRS ${ZSTD_INCLUDE_DIR})

	if (NOT TARGET ZSTD::ZSTD)
		add_library(ZSTD::ZSTD UNKNOWN IMPORTED)
		set_target_properties(
			ZSTD::ZSTD
			PROPERTIES
				IMPORTED_LINK_INTERFACE_LANGUAGES "C"
				IMPORTED_LOCATION "${ZSTD_LIBRARY}"
				INTERFACE_INCLUDE_DIRECTORIES "${ZSTD_INCLUDE_DIRS}"
		)
	endif ()

	if (NOT TARGET ZSTD::exe)
		add_executable(ZSTD::exe IMPORTED GLOBAL)
		set_target_properties(ZSTD::exe PROPERTIES IMPORTED_LOCATION "${ZSTD_EXE}")
	endif ()
endif ()

mark_as_advanced(ZSTD_INCLUDE_DIR ZSTD_LIBRARY)
