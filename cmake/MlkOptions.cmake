
mlk_option(MLK_WITH_DOXYGEN On
    BOOL "Enable doxygen documentation")
mlk_option(MLK_WITH_EXAMPLES On
    BOOL "Enable examples")
mlk_option(MLK_WITH_NLS On
    BOOL "Enable NLS support")
mlk_option(MLK_WITH_TESTS On
    BOOL "Enable unit tests")
mlk_option(MLK_WITH_TESTS_GRAPHICAL On
    BOOL "Enable unit tests that requires graphical context")
mlk_option(MLK_WITH_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake"
    STRING "Destination for CMake files")
