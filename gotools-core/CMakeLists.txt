PROJECT(GoToolsCore)


# Include directories

INCLUDE_DIRECTORIES(
  ${GoToolsCore_SOURCE_DIR}/include
  ${GoTools_COMMON_INCLUDE_DIRS}
  )


# Linked in libraries


# Make the gotools-core library

FILE(GLOB_RECURSE GoToolsCore_SRCS src/*.cpp src/*.C include/*.h)
ADD_LIBRARY(GoToolsCore ${GoToolsCore_SRCS})
SET_PROPERTY(TARGET GoToolsCore
  PROPERTY FOLDER "GoToolsCore/Libs")
SET_TARGET_PROPERTIES(GoToolsCore PROPERTIES SOVERSION ${GoTools_ABI_VERSION})


# Apps and tests
MACRO(ADD_APPS SUBDIR PROPERTY_FOLDER IS_TEST)
  FILE(GLOB_RECURSE GoToolsCore_APPS ${SUBDIR}/*.C)
  FOREACH(app ${GoToolsCore_APPS})
    GET_FILENAME_COMPONENT(appname ${app} NAME_WE)
    ADD_EXECUTABLE(${appname} ${app})
    TARGET_LINK_LIBRARIES(${appname} GoToolsCore ${DEPLIBS})
    SET_TARGET_PROPERTIES(${appname}
      PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${SUBDIR})
    SET_PROPERTY(TARGET ${appname}
      PROPERTY FOLDER "GoToolsCore/${PROPERTY_FOLDER}")
    IF(${IS_TEST})
      ADD_TEST(${appname} ${SUBDIR}/${appname}
		--log_format=XML --log_level=all --log_sink=../Testing/${appname}.xml)
    ENDIF(${IS_TEST})
  ENDFOREACH(app)
ENDMACRO(ADD_APPS)

IF(GoTools_COMPILE_APPS)
  ADD_APPS(app "Apps" FALSE)
  ADD_APPS(examples "Examples" FALSE)
ENDIF(GoTools_COMPILE_APPS)

IF(GoTools_COMPILE_TESTS)
  SET(DEPLIBS ${DEPLIBS} ${Boost_LIBRARIES})
  ADD_APPS(unit_tests "Unit Tests" TRUE)
  ADD_APPS(regression_tests "Regression Tests" TRUE)
ENDIF(GoTools_COMPILE_TESTS)


# Copy data
FILE(COPY ${GoToolsCore_SOURCE_DIR}/data
  DESTINATION ${GoToolsCore_BINARY_DIR})
FILE(COPY ${GoToolsCore_SOURCE_DIR}/examples/data
  DESTINATION ${GoToolsCore_BINARY_DIR}/examples)


# 'install' target
IF(WIN32)
  # Windows
  # lib
  INSTALL(TARGETS GoToolsCore DESTINATION ${GoTools_INSTALL_PREFIX}/lib)
  # include
  INSTALL(DIRECTORY include/
    DESTINATION ${GoTools_INSTALL_PREFIX}/include
    FILES_MATCHING PATTERN "*.h"
    PATTERN ".svn" EXCLUDE
    )
ELSE(WIN32)
  # Linux
  # lib
  INSTALL(TARGETS GoToolsCore DESTINATION lib COMPONENT core)
  # include
  INSTALL(DIRECTORY include/
    DESTINATION include
    COMPONENT core-dev
    FILES_MATCHING PATTERN "*.h"
    PATTERN ".svn" EXCLUDE
    )
ENDIF(WIN32)

SET(CPACK_STRIP_FILES ${CPACK_STRIP_FILES} libGoToolsCore.so)
