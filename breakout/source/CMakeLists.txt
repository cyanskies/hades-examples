# C++ source files
set(EX_BREAKOUT_SRC
	breakout_state.cpp
	input.cpp
	main.cpp
	../include/breakout_state.hpp
	../include/input.hpp
)

source_group("Include" FILES
	../include/breakout_state.hpp
	../include/input.hpp
)

# Library to build
set(EX_BREAKOUT_EXE ${PROJECT_NAME})

# Determine library suffixes depending on static/shared configuration
add_executable(${EX_BREAKOUT_EXE} ${EX_BREAKOUT_SRC})
set_target_properties(${EX_BREAKOUT_EXE} PROPERTIES DEBUG_POSTFIX -d)

target_include_directories(${EX_BREAKOUT_EXE} PRIVATE ../include)

set(EX_BREAKOUT_DEPS
	hades)

#lin libraries
target_link_libraries(${EX_BREAKOUT_EXE} PUBLIC ${EX_BREAKOUT_DEPS})

# Install (use "bin" folder for DLL, "lib" for SO files)
install(TARGETS ${EX_BREAKOUT_EXE}
		RUNTIME DESTINATION bin
		LIBRARY DESTINATION lib
		ARCHIVE DESTINATION lib)


# Install include directory and license files
install(DIRECTORY include
		DESTINATION .)