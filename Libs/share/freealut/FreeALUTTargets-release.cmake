#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "FreeALUT::alut" for configuration "Release"
set_property(TARGET FreeALUT::alut APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(FreeALUT::alut PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/alut.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/alut.dll"
  )

list(APPEND _cmake_import_check_targets FreeALUT::alut )
list(APPEND _cmake_import_check_files_for_FreeALUT::alut "${_IMPORT_PREFIX}/lib/alut.lib" "${_IMPORT_PREFIX}/bin/alut.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
