CMAKE_MINIMUM_REQUIRED(VERSION 3.14)


project(
	KolibriLib
	VERSION 0.0.3
	DESCRIPTION "C++ Library for KolibriOS"
	LANGUAGES CXX C
)

#################################################
################### CONFIG ######################
#################################################
#
set(CUSTOM_TOOCLAIN_PATH	OFF)
set(KOLIBRIOS_REPO	${CMAKE_CURRENT_SOURCE_DIR}/../kolibrios)#Репозиторий нужен только для libc, libstdc++-v3 и C_Layer, можете указать свой если очень хочется

option(KolibriLib_BUILD_EXAMPLES "Build KolibriLib examples"      ON)
option(KolibriLib_BUILD_DOCS     "Build KolibriLib documantation" ON)
option(KolibriLib_BUILD_TESTS    "Build KolibriLib test"          ON)
#
#################################################

get_directory_property(IS_SUBPROJECT PARENT_DIRECTORY)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(microlog)


set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake")

#enable FASM
include(CMakeDetermineASM_FASMCompiler)
include(CMakeASM_FASMInformation)
include(CMakeTestASM_FASMCompiler)

enable_language("ASM_FASM")


get_filename_component(REPO ${KOLIBRIOS_REPO} REALPATH)
set(KOLIBRIOS_REPO ${REPO})
if(EXISTS "${REPO}")
	message(STATUS "KOLIBRIOS REPO Found: ${KOLIBRIOS_REPO}")
else()
	message(FATAL_ERROR "KOLIBRIOS Repo not found, try set KOLIBRIOS_REPO or download kolibros repo")
endif()

SET(SDK_DIR "${KOLIBRIOS_REPO}/contrib/sdk")



include(CMakeDetermineKOS32Compiler)
include(CMakeTestKOS32Compiler)

# PATH TO KOLIBRILIB SOURCES
set(KolibriLib_INCLUDE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/include)
SET(SRC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src/KolibriLib)
SET(HEADERS_PATH ${KolibriLib_INCLUDE_PATH}/kolibriLib)
SET(C_LAYER_HEADERS ${KolibriLib_INCLUDE_PATH}/C_Layer/INCLUDE)

SET(SOURCES
	${SRC_PATH}/C_Layer/rasterworks.cpp
	${SRC_PATH}/C_Layer/buf2d.cpp
	${SRC_PATH}/C_Layer/libimg.cpp
	${SRC_PATH}/C_Layer/opendialog.cpp
	${SRC_PATH}/C_Layer/msgbox.cpp
	${SRC_PATH}/color.cpp
	${SRC_PATH}/os.cpp
	${SRC_PATH}/system/notify.cpp
	${SRC_PATH}/graphic.cpp
	${SRC_PATH}/img.cpp
	${SRC_PATH}/thread.cpp
	${SRC_PATH}/types.cpp
	${SRC_PATH}/textBase.cpp
	${SRC_PATH}/UI.cpp
	${SRC_PATH}/image.cpp
	${SRC_PATH}/text.cpp
	${SRC_PATH}/textlabel.cpp
	${SRC_PATH}/buttonsBase.cpp
	${SRC_PATH}/BaseButton.cpp
	${SRC_PATH}/Button.cpp
	${SRC_PATH}/TextButton.cpp
	${SRC_PATH}/ImageButton.cpp
	${SRC_PATH}/checkbox.cpp
	${SRC_PATH}/BaseEditor.cpp
	#${SRC_PATH}/menu.cpp
	${SRC_PATH}/window.cpp
	${SRC_PATH}/windowAttached.cpp
	${SRC_PATH}/debug.cpp
	${SRC_PATH}/globals.cpp
	#${SRC_PATH}/freetype.cpp
	${SRC_PATH}/keyboard.cpp
	${SRC_PATH}/OpenDialog.cpp
	${SRC_PATH}/clipboard.cpp
	${SRC_PATH}/time.cpp
	${SRC_PATH}/ProgressBar.cpp
	${SRC_PATH}/filesystem/filesystem.cpp
	${SRC_PATH}/filesystem/file_type.cpp
)

# CONFIGURED C_Layer ASM
set(C_Layer_Path ${KOLIBRIOS_REPO}/contrib/C_Layer)
set(C_Layer_SRC_PATH ${C_Layer_Path}/ASM)

#LOADERS THAT LOAD SHARED LIBS
ADD_LIBRARY(
	C_LAYER_LOADERS OBJECT 
	${C_Layer_SRC_PATH}/loadlibimg.asm
	${C_Layer_SRC_PATH}/loadrasterworks.asm
	${C_Layer_SRC_PATH}/loadbuf2d.asm
	${C_Layer_SRC_PATH}/loadproclib.asm
)

target_include_directories(
	C_LAYER_LOADERS
	PRIVATE ${KOLIBRIOS_REPO}/programs
)


add_library(
	C_LAYER STATIC
	$<TARGET_OBJECTS:C_LAYER_LOADERS>
	${SRC_PATH}/C_Layer/kolibri_buf2d.c
	${SRC_PATH}/C_Layer/kolibri_libimg.c
	${SRC_PATH}/C_Layer/kolibri_rasterworks.c
	${SRC_PATH}/C_Layer/kolibri_opendialog.c
)

target_compile_options(
	C_LAYER
	PRIVATE -D__MakeStaticLib__
	PUBLIC -fno-ident -fomit-frame-pointer -U__WIN32__ -U_Win32 -U_WIN32 -U__MINGW32__ -UWIN32
	PUBLIC $<$<CONFIG:DEBUG>:-Wall -Wextra -Wcast-align -Wcast-qual -Wconversion -Wenum-compare -Wredundant-decls -Wfloat-equal -Wsign-conversion -DDEBUG -Og>
)

target_include_directories(
	C_LAYER
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
	PUBLIC ${LIBC_DIR}/include
)


# ТА САМАЯ ЛИБА ИЗ_ЗА КОТОРОЙ ВСЁ ЭТО
ADD_LIBRARY(KolibriLib STATIC 
	${SOURCES}
	$<TARGET_OBJECTS:C_LAYER>
	$<TARGET_OBJECTS:microlog>
)

target_link_libraries(
	KolibriLib 
	PUBLIC C_LAYER
)

target_compile_options(
	KolibriLib
	PRIVATE -D__MakeStaticLib__
	PUBLIC $<$<CONFIG:DEBUG>:-Wctor-dtor-privacy -Wnon-virtual-dtor -Wsign-promo -Wold-style-cast -Woverloaded-virtual>
)

target_include_directories(
	KolibriLib
	PUBLIC ${LIBSTDCPP_DIR}/include
	PUBLIC ${KOLIBRIOS_REPO}/contrib/sdk/sources/freetype/include
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/microlog/include
)


#GENERATE DOXYGEN ONLY THIS PROJECT ISNT SUBPROECT
if(KolibriLib_BUILD_DOCS)
	message("-- Build KolibriLib Docs")

	FIND_PACKAGE(Doxygen)

	if(DOXYGEN_FOUND)
		set(DOXYGEN_GENERATE_HTML YES)
		set(DOXYGEN_PROJECT_NUMBER ${PROJECT_NUMBER})
		set(DOXYGEN_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Docs)
		set(DOXYGEN_IMAGE_PATH ${DOXYGEN_OUTPUT_DIRECTORY}/media)
		set(DOXYGEN_USE_MDFILE_AS_MAINPAGE ${CMAKE_CURRENT_SOURCE_DIR}/Readme.md)
		
		doxygen_add_docs(doc ${CMAKE_SOURCE_DIR} )

	else(DOXYGEN_FOUND)
		message(STATUS "Doxygen not found. Documentation will not be generated")
	endif(DOXYGEN_FOUND)

endif()

if(KolibriLib_BUILD_EXAMPLES)

	message("-- Build KolibriLib Examples")

	add_subdirectory(examples)

endif()

if(KolibriLib_BUILD_TESTS)
	message("-- Build KolibriLib tests")

	add_subdirectory(tests)
endif()