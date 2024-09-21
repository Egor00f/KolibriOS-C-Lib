macro(add_kolibri_executable EXE_TARGET SOURCES)

	add_executable(${EXE_TARGET} ${SOURCES})

	add_custom_command(
		TARGET ${EXE_TARGET} POST_BUILD
		COMMAND ${CMAKE_OBJCOPY} ARGS ${EXE_TARGET} -O binary
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	)

endmacro()