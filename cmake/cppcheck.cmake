# additional target to perform cppcheck run, requires cppcheck


# Recurisve globbing leads to being unable to detect when new-files are added
# If anyone has a better way of doing this, please send a pull request or leave
# a comment
set (CPPCHECK_FILES "" )
foreach(cppcheck_dir ${CPPCHECK_DIR})
	foreach(cppcheck_extension ${CPPCHECK_EXTENSIONS})
		file(GLOB_RECURSE CPPCHECK_TMP_FILES RELATIVE ${PROJECT_SOURCE_DIR} ${cppcheck_dir}/*.${cppcheck_extension})
		set (CPPCHECK_FILES ${CPPCHECK_FILES} ${CPPCHECK_TMP_FILES})
	endforeach(cppcheck_extension)
endforeach(cppcheck_dir)

get_property(inc_dirs DIRECTORY PROPERTY INCLUDE_DIRECTORIES)

set (CPPCHECK_INCLUDE "" )
foreach(inc_dir ${inc_dirs})
	set (CPPCHECK_INCLUDE ${CPPCHECK_INCLUDE} " -I ${inc_dir}")
endforeach(inc_dir)

MESSAGE (STATUS "include dirs:" ${inc_dirs})
add_custom_target(
		cppcheck
		COMMAND cppcheck
		--enable=all
		--suppress=missingIncludeSystem
		--std=c++11
		--quiet
		--verbose
		--error-exitcode=1
		${CPPCHECK_INCLUDE}
		${CPPCHECK_FILES}
		WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)
