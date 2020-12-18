# CMakeLists.txt pour la compilation de la librairie statique modelGenerator.
# encoding: UTF-8
# author: lhm

project(MODELGENERATOR)
set(DESCRIPTION "lib used to generate finite state machine from config file")

cmake_minimum_required(VERSION 3.0)
message("CMake version found: ${CMAKE_VERSION}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(ignoreMe "${QT_QMAKE_EXECUTABLE}")

if("${CMAKE_CXX_SIZEOF_DATA_PTR}" STREQUAL "8")
	message("Compilation pour plate-forme 64 bits")
endif("${CMAKE_CXX_SIZEOF_DATA_PTR}" STREQUAL "8")

# Ajout des avertissements pour les compilations avec gcc
if(CMAKE_COMPILER_IS_GNUCXX)
  #Warnings are errors
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
  
  #Warning questionable pratices
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
	set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -Wall")
endif(CMAKE_COMPILER_IS_GNUCXX)

if(WIN32)
	# Suppression des avertissements Visual Studio
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif(WIN32)

set(SRC_DIR     ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(DATA_DIR    ${CMAKE_CURRENT_SOURCE_DIR}/data)
set(UTILS_DIR   ${SRC_DIR}/dependencies/utils)
set(PUGIXML_SRC ${SRC_DIR}/dependencies/pugixml-1.9/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(INSTALL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/install_ModelGenerator)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${INSTALL_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${INSTALL_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${INSTALL_DIR}/bin)

set(MODELGENERATOR_SOURCES
    ${SRC_DIR}/main.cpp
    ${SRC_DIR}/Logger/Logger.cpp
    ${SRC_DIR}/Model/Model.cpp
    ${SRC_DIR}/Conf/Conf_format.cpp
    ${SRC_DIR}/Model/Message.cpp
    ${SRC_DIR}/Model/Header.cpp
    ${SRC_DIR}/Model/Field.cpp
    ${SRC_DIR}/Model/Field_size.cpp
    ${SRC_DIR}/Model/Field_time.cpp
    ${SRC_DIR}/Model/Field_id.cpp
    ${SRC_DIR}/Model/State.cpp
    ${PUGIXML_SRC}/pugixml.cpp
    ${UTILS_DIR}/time_util.cpp
    ${INCLUDE_DIR}/modelGenerator_interface.cpp
)

set(MODELGENERATOR_HEADERS
    ${SRC_DIR}/Exceptions/Exception.h
    ${SRC_DIR}/Logger/Logger.h
    ${SRC_DIR}/Model/Model.h
    ${UTILS_DIR}/opt_util.h
    ${UTILS_DIR}/time_util.h
    ${UTILS_DIR}/includes.h
    ${SRC_DIR}/Conf/Conf_format.h
    ${SRC_DIR}/Model/Message.h
    ${SRC_DIR}/Model/Header.h
    ${SRC_DIR}/Model/Field.h
    ${SRC_DIR}/Model/Field_size.h
    ${SRC_DIR}/Model/Field_time.h
    ${SRC_DIR}/Model/Field_id.h
    ${SRC_DIR}/Model/State.h
    ${INCLUDE_DIR}/modelGenerator_interface.h
)

include_directories(${PUGIXML_SRC})
include_directories(${SRC_DIR})
include_directories(${SRC_DIR}/Model)
include_directories(${SRC_DIR}/Conf)
include_directories(${SRC_DIR}/Logger)
include_directories(${SRC_DIR}/Exceptions)
include_directories(${UTILS_DIR})
include_directories(${INCLUDE_DIR})

# An executable for an example
add_executable(ModelGeneratorSample ${MODELGENERATOR_SOURCES} ${MODELGENERATOR_HEADERS})

# A static library for general usage
add_library(modelGenerator ${MODELGENERATOR_SOURCES} ${MODELGENERATOR_HEADERS})

file(COPY ${INCLUDE_DIR}/modelGenerator_interface.h
        DESTINATION ${INSTALL_DIR}/include
        )

file(COPY ${DATA_DIR} 
     DESTINATION ${INSTALL_DIR}
    )

#-----------------------------------------------------------------------------------#

find_package(Doxygen)
find_package(LATEX)

set(VERSION_DIR ${SRC_DIR}/Version)

configure_file(doxygen.conf.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/doxygen.conf)

# Afin de permettre l'inclusion de version.h
include_directories(${VERSION_DIR})

#------------------------- LIBRARY VERSION UPDATING --------------------------------#

if(NOT EXISTS "${VERSION_DIR}/version.h")
file(WRITE "${VERSION_DIR}/version.h" "#define LIB_VERSION \"svn\"\n")
endif(NOT EXISTS "${VERSION_DIR}/version.h")

find_package(Git)
if(GIT_FOUND)
  message("git found: MODELGENERATOR library version number will be updated.")

  if(EXISTS "${CMAKE_SOURCE_DIR}/.git")
    execute_process(
      COMMAND git rev-parse --abbrev-ref HEAD
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
      OUTPUT_VARIABLE GIT_BRANCH
      OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    execute_process(
      COMMAND git log -1 --format=%h
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
      OUTPUT_VARIABLE GIT_COMMIT_HASH
      OUTPUT_STRIP_TRAILING_WHITESPACE
    )
  else(EXISTS "${CMAKE_SOURCE_DIR}/.git")
    message("The current directory is not versionned. Using default versioning values (NA)")
    set(GIT_BRANCH      "NA")
    set(GIT_COMMIT_HASH "NA")
  endif(EXISTS "${CMAKE_SOURCE_DIR}/.git")

  message(STATUS "\tGit current branch: ${GIT_BRANCH}")
  message(STATUS "\tGit commit hash: ${GIT_COMMIT_HASH}")

  message(STATUS "Generating version.h...")

  configure_file(
    ${VERSION_DIR}/version.h.in
    ${VERSION_DIR}/version.h
  )
else(GIT_FOUND)
    message("svnversion not found. The MODELGENERATOR library version number will not be updated.")
endif(GIT_FOUND)

#------------------- LIBRARY DOCUMENTATION GENERATION ----------------------------#

# Génération de la documentation associée.
if(DOXYGEN_FOUND)
if(PDFLATEX_COMPILER)

  if(DOXYGEN_DOT_FOUND)
      set(DOT_PATH ${DOXYGEN_DOT_PATH})
  else(DOXYGEN_DOT_FOUND)
      find_program(DOT_EXE dot
                "$ENV{ProgramFiles}/Graphviz2.38/bin")
      if(DOT_EXE)
          # The Doxyfile wants the path to Dot, not the entire path and executable
          get_filename_component(DOT_PATH "${DOT_EXE}" PATH CACHE)
      endif(DOT_EXE)
  endif(DOXYGEN_DOT_FOUND)

  set(DOC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/doc/latex)
  set(REFMAN_TEX ${DOC_DIR}/refman.tex)

  file(GLOB_RECURSE DOC_SOURCES ${CMAKE_CURRENT_SOURCE_DIR} *.cpp *.c *.h)

  add_custom_command(OUTPUT ${REFMAN_TEX}
    COMMAND ${DOXYGEN_EXECUTABLE} doxygen.conf
    DEPENDS ${DOC_SOURCES}
    COMMENT "Generating MODELGENERATOR API documentation with Doxygen..."
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )

  add_custom_target(MODELGENERATOR_doc
    DEPENDS ${REFMAN_TEX}
    )

else(PDFLATEX_COMPILER)
    message("pdflatex not found. Documentation will not be generated")
endif(PDFLATEX_COMPILER)
else(DOXYGEN_FOUND)
    message("Doxygen not found. Documentation will not be generated")
endif(DOXYGEN_FOUND)

#------------------------- TESTS --------------------------------#
option(ENABLE_TESTS "Enable tests" ON)
if(${ENABLE_TESTS})
  message("Building unit tests...")
  enable_testing()
  add_subdirectory(tests)
else(${ENABLE_TESTS})
  message("Testing disabled.")
endif(${ENABLE_TESTS})