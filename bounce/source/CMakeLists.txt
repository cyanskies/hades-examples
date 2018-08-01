# C++ source files
set(BOUNCE_SRC
	collision_state.cpp
	main.cpp
	../include/collision_state.hpp
)

source_group("Include" FILES
	../include/collision_state.hpp
)

# Library to build
set(BOUNCE_EXE ${PROJECT_NAME})

# Determine library suffixes depending on static/shared configuration
add_executable(${BOUNCE_EXE} ${BOUNCE_SRC})
set_target_properties(${BOUNCE_EXE} PROPERTIES DEBUG_POSTFIX -d)

target_include_directories(${BOUNCE_EXE} PRIVATE ../include)

set(BOUNCE_DEPS
	hades)

#lin libraries
target_link_libraries(${BOUNCE_EXE} PUBLIC ${BOUNCE_DEPS})

# Install (use "bin" folder for DLL, "lib" for SO files)
install(TARGETS ${BOUNCE_EXE}
		RUNTIME DESTINATION bin
		LIBRARY DESTINATION lib
		ARCHIVE DESTINATION lib)