cmake_minimum_required(VERSION 2.8)

if (NOT DEFINED CMAKE_TOOLCHAIN_FILE)
	if (DEFINED ENV{VITASDK})
		set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
	else()
		message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
	endif()
endif()

set(SHORT_NAME VitaBackup)
project(${SHORT_NAME})
include("${VITASDK}/share/vita.cmake" REQUIRED)

set(VITA_APP_NAME "Vita Backup")
set(VITA_TITLEID  "VITABUP00")
set(VITA_VERSION  "01.10")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Werror -O2 -mword-relocations -fomit-frame-pointer -ffunction-sections")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti -fno-exceptions -std=gnu++11")

FUNCTION(ADD_RESOURCES out_var)
	SET(result)
	FOREACH(in_f ${ARGN})
		SET(out_f "${CMAKE_CURRENT_BINARY_DIR}/${in_f}.o")
		GET_FILENAME_COMPONENT(out_dir ${out_f} DIRECTORY)
		ADD_CUSTOM_COMMAND(OUTPUT ${out_f}
			COMMAND ${CMAKE_COMMAND} -E make_directory ${out_dir}
			COMMAND ${CMAKE_LINKER} -r -b binary -o ${out_f} ${in_f}
			DEPENDS ${in_f}
			WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
			COMMENT "Building resource ${out_f}"
			VERBATIM
			)
		LIST(APPEND result ${out_f})
	ENDFOREACH()
	SET(${out_var} "${result}" PARENT_SCOPE)
ENDFUNCTION()

file(GLOB res_files RELATIVE
  ${CMAKE_SOURCE_DIR}
  res/Light/*.png
  res/Phalaris/*.png
)
add_resources(vitaident_res ${res_files})

include_directories(
	include
	include/menus
	include/microtar
)

add_executable(${SHORT_NAME}
	${vitaident_res}
	source/fs.c
	source/log.c
	source/main.c
	source/menus/menu_backup.c
	source/menus/menu_delete.c
	source/menus/menu_main.c
	source/menus/menu_options.c
	source/menus/menu_restore.c
	source/microtar/microtar.c
	source/microtar/microtar_read.c
	source/microtar/microtar_write.c
	source/progress_bar.c
	source/textures.c
	source/touch.c
	source/utils.c
)

target_link_libraries(${SHORT_NAME}
	vita2d
	png
	jpeg
	z
	m
	c
	SceAppMgr_stub
	SceAppUtil_stub
	SceCommonDialog_stub
	SceCtrl_stub
	SceDisplay_stub
	SceGxm_stub
	SceIofilemgr_stub 
	SceLibKernel_stub
	SceNet_stub
	SceNetCtl_stub
	ScePower_stub
	ScePvf_stub
	SceShellSvc_stub
	SceSysmodule_stub
	SceTouch_stub 
	SceUdcd_stub
	SceUsbstorVStorDriver_stub
)

vita_create_self(eboot.bin ${SHORT_NAME} UNSAFE)
vita_create_vpk(${SHORT_NAME}.vpk ${VITA_TITLEID} eboot.bin
	VERSION ${VITA_VERSION}
	NAME ${VITA_APP_NAME}
	FILE sce_sys/icon0.png sce_sys/icon0.png
		 sce_sys/livearea/contents/bg.png sce_sys/livearea/contents/bg.png
		 sce_sys/livearea/contents/startup.png sce_sys/livearea/contents/startup.png
		 sce_sys/livearea/contents/template.xml sce_sys/livearea/contents/template.xml
)
