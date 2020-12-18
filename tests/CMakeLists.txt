# Creates every unit tests for the library

set(CATCH_INCLUDE_DIR 
    ${CMAKE_CURRENT_SOURCE_DIR}/catch2/)

include_directories(${CATCH_INCLUDE_DIR})
link_directories(${INSTALL_DIR}/lib)

# Compile each source independently - they each define different tests
set(INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include )
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(SRCS 
	01-dummy
	02-version
	03-logs
	04-model
    05-time)

foreach(H ${HEADERS})
    LIST(APPEND ALL_HEADERS ${INC_DIR}/${H}.hpp)
endforeach()

foreach(S ${SRCS})
    add_executable(${S} ${SRC_DIR}/${S}.cpp ${ALL_HEADERS})
    target_include_directories(${S} PUBLIC ${INC_DIR})
    target_link_libraries(${S} modelGenerator)
    add_test(NAME ${S} COMMAND ${S})
endforeach()
