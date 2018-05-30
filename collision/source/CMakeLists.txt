# C++ source files
set(COLLISION_SRC
	collision_state.cpp
	main.cpp
	../include/collision_state.hpp
)

source_group("Include" FILES
	../include/collision_state.hpp
)

# Library to build
set(COLLISION_EXE ${PROJECT_NAME})

# Determine library suffixes depending on static/shared configuration
add_executable(${COLLISION_EXE} ${COLLISION_SRC})
set_target_properties(${COLLISION_EXE} PROPERTIES DEBUG_POSTFIX -d)

target_include_directories(${COLLISION_EXE} PRIVATE ../include)

set(COLLISION_DEPS
	hades)

#lin libraries
target_link_libraries(${COLLISION_EXE} PUBLIC ${COLLISION_DEPS})

# Install (use "bin" folder for DLL, "lib" for SO files)
install(TARGETS ${COLLISION_EXE}
		RUNTIME DESTINATION bin
		LIBRARY DESTINATION lib
		ARCHIVE DESTINATION lib)