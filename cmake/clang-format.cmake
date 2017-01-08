# additional target to perform clang-format run, requires clang-format

# Recurisve globbing leads to being unable to detect when new-files are added
# If anyone has a better way of doing this, please send a pull request or leave
# a comment
set (CLANG_FORMAT_FILES "" )
foreach(clang_format_dir ${CLANG_FORMAT_DIR})
	foreach(clang_format_extension ${CLANG_FORMAT_EXTENSIONS})
		file(GLOB_RECURSE CLANG_FORMAT_TMP_FILES RELATIVE ${PROJECT_SOURCE_DIR} ${clang_format_dir}/*.${clang_format_extension})
		set (CLANG_FORMAT_FILES ${CLANG_FORMAT_FILES} ${CLANG_FORMAT_TMP_FILES})
	endforeach(clang_format_extension)
endforeach(clang_format_dir)

add_custom_target(
        format
        COMMAND clang-format
        -style=file
        -i
		${CLANG_FORMAT_FILES}
		WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)
