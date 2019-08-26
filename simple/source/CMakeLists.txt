# C++ source files
set(SIMPLE_SRC
	simple_state.cpp
	main.cpp
	../include/simple_state.hpp
)

# Library to build
set(SIMPLE_EXE ${PROJECT_NAME})

# Determine library suffixes depending on static/shared configuration
add_executable(${SIMPLE_EXE} ${SIMPLE_SRC})
set_target_properties(${SIMPLE_EXE} PROPERTIES DEBUG_POSTFIX -d)

target_include_directories(${SIMPLE_EXE} PRIVATE ../include)

set(SIMPLE_DEPS
	hades)

#lin libraries
target_link_libraries(${SIMPLE_EXE} PUBLIC ${SIMPLE_DEPS})

# Install (use "bin" folder for DLL, "lib" for SO files)
install(TARGETS ${SIMPLE_EXE}
		RUNTIME DESTINATION bin
		LIBRARY DESTINATION lib
		ARCHIVE DESTINATION lib)