# additional target to generate AmdRegDb.h

add_custom_target(
		genregs
		COMMAND ${PROJECT_SOURCE_DIR}/bin/amdgenreg.py
        > ${PROJECT_SOURCE_DIR}/src/AmdRegDb.h
)
