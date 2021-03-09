
####### Expanded from @PACKAGE_INIT@ by configure_package_config_file() #######
####### Any changes to this file will be overwritten by the next CMake run ####
####### The input file was clownaudioConfig.cmake.in                            ########

get_filename_component(PACKAGE_PREFIX_DIR "${CMAKE_CURRENT_LIST_DIR}/../../../" ABSOLUTE)

macro(set_and_check _var _file)
  set(${_var} "${_file}")
  if(NOT EXISTS "${_file}")
    message(FATAL_ERROR "File or directory ${_file} referenced by variable ${_var} does not exist !")
  endif()
endmacro()

macro(check_required_components _NAME)
  foreach(comp ${${_NAME}_FIND_COMPONENTS})
    if(NOT ${_NAME}_${comp}_FOUND)
      if(${_NAME}_FIND_REQUIRED_${comp})
        set(${_NAME}_FOUND FALSE)
      endif()
    endif()
  endforeach()
endmacro()

####################################################################################

if(NOT OFF)
	if(OFF)
		find_package(PkgConfig REQUIRED)
		pkg_check_modules(vorbisfile REQUIRED IMPORTED_TARGET vorbisfile)
	endif()

	if(OFF)
		find_package(PkgConfig REQUIRED)
		pkg_check_modules(flac REQUIRED IMPORTED_TARGET flac)
	endif()

	if(OFF)
		find_package(PkgConfig REQUIRED)
		pkg_check_modules(opusfile REQUIRED IMPORTED_TARGET opusfile)
	endif()

	if(OFF)
		find_package(PkgConfig REQUIRED)
		pkg_check_modules(sndfile REQUIRED IMPORTED_TARGET sndfile)
	endif()

	if(OFF)
		find_package(PkgConfig REQUIRED)
		pkg_check_modules(libopenmpt REQUIRED IMPORTED_TARGET libopenmpt)
	endif()

	if()
		find_package(PkgConfig REQUIRED)
		pkg_check_modules(libxmp-lite REQUIRED IMPORTED_TARGET libxmp-lite)
	endif()
endif()

include("${CMAKE_CURRENT_LIST_DIR}/clownaudioTargets.cmake")
check_required_components(clownaudio)
