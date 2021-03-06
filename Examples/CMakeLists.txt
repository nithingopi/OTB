# $Id$

PROJECT(OTBExamples)

IF(OTB_BINARY_DIR)
  # We are building inside the tree.

ADD_SUBDIRECTORY(BasicFilters)
ADD_SUBDIRECTORY(FeatureExtraction)
ADD_SUBDIRECTORY(DataRepresentation)
ADD_SUBDIRECTORY(IO)
ADD_SUBDIRECTORY(Filtering)
ADD_SUBDIRECTORY(ChangeDetection)
ADD_SUBDIRECTORY(Learning)
ADD_SUBDIRECTORY(Classification)
ADD_SUBDIRECTORY(Segmentation)
ADD_SUBDIRECTORY(Iterators)
ADD_SUBDIRECTORY(MultiScale)
ADD_SUBDIRECTORY(DisparityMap)
ADD_SUBDIRECTORY(Projections)
ADD_SUBDIRECTORY(Registration)
ADD_SUBDIRECTORY(Radiometry)
ADD_SUBDIRECTORY(SARPolarimetry)
ADD_SUBDIRECTORY(Fusion)
ADD_SUBDIRECTORY(Tutorials)
ADD_SUBDIRECTORY(Markov)
ADD_SUBDIRECTORY(OBIA)


IF(OTB_USE_VISU_GUI)
        ADD_SUBDIRECTORY(Visualization)
ENDIF(OTB_USE_VISU_GUI)

IF(OTB_USE_PATENTED)
  ADD_SUBDIRECTORY( Patented )
ENDIF(OTB_USE_PATENTED)

IF(OTB_USE_PQXX)
        ADD_SUBDIRECTORY(GeospatialAnalysis)
ENDIF(OTB_USE_PQXX)

#Recopie du fichier README.txt dans l'arborescence BINARY
IF( EXISTS ${OTB_BINARY_DIR}/Examples/README.txt )
ELSE( EXISTS ${OTB_BINARY_DIR}/Examples/README.txt )
  CONFIGURE_FILE(${OTB_SOURCE_DIR}/Examples/README.txt
    ${OTB_BINARY_DIR}/Examples/README.txt @ONLY IMMEDIATE)
ENDIF( EXISTS ${OTB_BINARY_DIR}/Examples/README.txt )


ELSE(OTB_BINARY_DIR)
  # We are building as a stand-alone project.
  SET(LIBRARY_OUTPUT_PATH ${OTBExamples_BINARY_DIR}/bin CACHE PATH "Single output directory for building all libraries.")
  SET(EXECUTABLE_OUTPUT_PATH ${OTBExamples_BINARY_DIR}/bin CACHE PATH "Single output directory for building all executables.")
  MARK_AS_ADVANCED(LIBRARY_OUTPUT_PATH EXECUTABLE_OUTPUT_PATH)
  SET(CXX_TEST_PATH ${EXECUTABLE_OUTPUT_PATH})

  # Configure Dart testing support.
  SET(PROJECT_SOURCE_DIR "${OTBExamples_SOURCE_DIR}/..")
  INCLUDE(${CMAKE_ROOT}/Modules/Dart.cmake)
  SET(PROJECT_SOURCE_DIR "${OTBExamples_SOURCE_DIR}")
  MARK_AS_ADVANCED(TCL_TCLSH)
  IF(BUILD_TESTING)
    ENABLE_TESTING()
  ENDIF(BUILD_TESTING)

  # Find OTB and load its settings.
  FIND_PACKAGE(OTB)
  IF(OTB_FOUND)
    INCLUDE(${OTB_USE_FILE})
        ADD_SUBDIRECTORY(BasicFilters)
        ADD_SUBDIRECTORY(FeatureExtraction)
        ADD_SUBDIRECTORY(DataRepresentation)
        ADD_SUBDIRECTORY(IO)
        ADD_SUBDIRECTORY(Filtering)
        ADD_SUBDIRECTORY(ChangeDetection)
        ADD_SUBDIRECTORY(Learning)
        ADD_SUBDIRECTORY(Classification)
        ADD_SUBDIRECTORY(Segmentation)
        ADD_SUBDIRECTORY(Iterators)
        ADD_SUBDIRECTORY(MultiScale)
        ADD_SUBDIRECTORY(DisparityMap)
        ADD_SUBDIRECTORY(Registration)
        ADD_SUBDIRECTORY(Radiometry)
        ADD_SUBDIRECTORY(SARPolarimetry)
        ADD_SUBDIRECTORY(Fusion)
        ADD_SUBDIRECTORY(Tutorials)
        ADD_SUBDIRECTORY(Markov)
	ADD_SUBDIRECTORY(OBIA)

    IF(OTB_USE_VISU_GUI)
        ADD_SUBDIRECTORY(Visualization)
    ENDIF(OTB_USE_VISU_GUI)

    IF(OTB_USE_PATENTED)
        ADD_SUBDIRECTORY( Patented )
    ENDIF(OTB_USE_PATENTED)

    IF(OTB_USE_PQXX)
	ADD_SUBDIRECTORY(GeospatialAnalysis)
    ENDIF(OTB_USE_PQXX)
  ELSE(OTB_FOUND)
    MESSAGE("OTB not found.  Please set OTB_DIR")
  ENDIF(OTB_FOUND)
ENDIF(OTB_BINARY_DIR)

