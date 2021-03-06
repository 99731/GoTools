PROJECT(GoCompositeModel)


# Include directories

INCLUDE_DIRECTORIES(
  ${GoCompositeModel_SOURCE_DIR}/include
  ${parametrization_SOURCE_DIR}/include
  ${GoTopology_SOURCE_DIR}/include
  ${GoIntersections_SOURCE_DIR}/include
  ${GoImplicitization_SOURCE_DIR}/include
  ${GoIgeslib_SOURCE_DIR}/include
  ${GoToolsCore_SOURCE_DIR}/include
  ${GoTools_COMMON_INCLUDE_DIRS}
  )


# Linked in libraries

SET(DEPLIBS
  parametrization
  GoTopology
  GoIntersections
  GoImplicitization
  GoIgeslib
  GoToolsCore
  ttl
  sisl
  newmat
  )


# Make the GoCompositeModel library

FILE(GLOB_RECURSE GoCompositeModel_SRCS src/*.C include/*.h)
ADD_LIBRARY(GoCompositeModel ${GoCompositeModel_SRCS})
TARGET_LINK_LIBRARIES(GoCompositeModel ${DEPLIBS})
SET_PROPERTY(TARGET GoCompositeModel
  PROPERTY FOLDER "GoCompositeModel/Libs")
SET_TARGET_PROPERTIES(GoCompositeModel PROPERTIES SOVERSION ${GoTools_ABI_VERSION})



# Apps and tests
MACRO(ADD_APPS SUBDIR PROPERTY_FOLDER IS_TEST)
  FILE(GLOB_RECURSE GoCompositeModel_APPS ${SUBDIR}/*.C)
  FOREACH(app ${GoCompositeModel_APPS})
    GET_FILENAME_COMPONENT(appname ${app} NAME_WE)
    ADD_EXECUTABLE(${appname} ${app})
    TARGET_LINK_LIBRARIES(${appname} GoCompositeModel ${DEPLIBS})
    SET_TARGET_PROPERTIES(${appname}
      PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${SUBDIR})
    SET_PROPERTY(TARGET ${appname}
      PROPERTY FOLDER "GoCompositeModel/${PROPERTY_FOLDER}")
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
FILE(COPY ${GoCompositeModel_SOURCE_DIR}/data
  DESTINATION ${GoCompositeModel_BINARY_DIR})
FILE(COPY ${GoCompositeModel_SOURCE_DIR}/examples/data
  DESTINATION ${GoCompositeModel_BINARY_DIR}/examples)


# 'install' target

IF(WIN32)
  # Windows
  # lib
  INSTALL(TARGETS GoCompositeModel DESTINATION ${GoTools_INSTALL_PREFIX}/lib)
  #include
  INSTALL(DIRECTORY include/GoTools/compositemodel
    DESTINATION ${GoTools_INSTALL_PREFIX}/include/GoTools
    FILES_MATCHING PATTERN "*.h"
    PATTERN ".svn" EXCLUDE
    )
ELSE(WIN32)
  # Linux
  #lib
  INSTALL(TARGETS GoCompositeModel DESTINATION lib COMPONENT compositemodel)
  # include
  INSTALL(DIRECTORY include/GoTools/compositemodel
    DESTINATION include/GoTools
    COMPONENT compositemodel-dev
    FILES_MATCHING PATTERN "*.h"
    PATTERN ".svn" EXCLUDE
    )
ENDIF(WIN32)

SET(CPACK_STRIP_FILES ${CPACK_STRIP_FILES} libGoCompositeModel.so)
