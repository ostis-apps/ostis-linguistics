set(SC_DIALOG_MODULE_SRC "${PROBLEM_SOLVER_ROOT}/dialogModule")

set(SOURCES
		"dialogModule.cpp"
		"keynodes/keynodes.cpp"
		"agents/TextNormalizationAgent.cpp"
		)

set(HEADERS
		"dialogModule.hpp"
		"keynodes/keynodes.hpp"
		"agents/TextNormalizationAgent.hpp"
		)

include_directories(${SC_DIALOG_MODULE_SRC} ${SC_MEMORY_SRC} ${GLIB2_INCLUDE_DIRS})

add_library(dialogModule SHARED ${SOURCES} ${HEADERS})
add_dependencies(dialogModule sc-memory-cpp sc-kpm-common sc-agents-common)
target_link_libraries(dialogModule sc-memory-cpp sc-kpm-common sc-agents-common)

sc_codegen(dialogModule ${SC_DIALOG_MODULE_SRC})

SET(CMAKE_BUILD_TYPE Debug)
