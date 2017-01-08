set(SOURCES
		hello.cpp)
add_executable(hello ${SOURCES})
install (TARGETS hello DESTINATION bin)
