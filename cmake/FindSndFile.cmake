# FindSndFile
# -----------
#
# Find SndFile library, this modules defines:
#
# SndFile_INCLUDE_DIRS, where to find sndfile.h
# SndFile_LIBRARIES, where to find library
# SndFile_FOUND, if it is found
#
# The following imported targets will be available:
#
# SndFile::SndFile, if found.
#

find_path(SndFile_INCLUDE_DIR NAMES sndfile.h)
find_library(SndFile_LIBRARY NAMES libsndfile sndfile)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(
	SndFile
	FOUND_VAR SndFile_FOUND
	REQUIRED_VARS SndFile_LIBRARY SndFile_INCLUDE_DIR
)

if (SndFile_FOUND)
	set(SndFile_LIBRARIES ${SndFile_LIBRARY})
	set(SndFile_INCLUDE_DIRS ${SndFile_INCLUDE_DIR})

	if (NOT TARGET SndFile::SndFile)
		add_library(SndFile::SndFile UNKNOWN IMPORTED)
		set_target_properties(
			SndFile::SndFile
			PROPERTIES
				IMPORTED_LINK_INTERFACE_LANGUAGES "C"
				IMPORTED_LOCATION "${SndFile_LIBRARY}"
				INTERFACE_INCLUDE_DIRECTORIES "${SndFile_INCLUDE_DIRS}"
		)
	endif ()
endif ()

mark_as_advanced(SndFile_INCLUDE_DIR SndFile_LIBRARY)
