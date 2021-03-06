
IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/OTB/Images)
SET(BASELINE_FILES ${OTB_DATA_ROOT}/Baseline/OTB/Files)
SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
SET(TEMP ${OTBTesting_BINARY_DIR}/Temporary)

#Tolerance de base
SET(NOTOL 0.0)

#Tolerance sur diff pixel image
#EPSILON est different de 0.0 pour les tests multiplateformes avec différentes options de compilation.
SET(EPSILON_8 0.00000001)

SET(FUSION_TESTS1 ${CXX_TEST_PATH}/otbFusionTests1)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbFusion1 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# -------            otb::ImageFusionBase   ------------------------------
ADD_TEST(fuTuFusionImageBaseNew ${FUSION_TESTS1}
        otbFusionImageBaseNew
)
# -------            otb::BayesianFusionFilter   ------------------------------
ADD_TEST(fuTuBayesianFusionFilterNew ${FUSION_TESTS1}
        otbBayesianFusionFilterNew
)

ADD_TEST(fuTvBayesianFusionFilter ${FUSION_TESTS1}
    --compare-image ${NOTOL}  ${BASELINE}/fuTvBayesianFusion.tif
                           ${TEMP}/fuTvBayesianFusion.tif
        otbBayesianFusionFilter
	${INPUTDATA}/multiSpect.tif
	${INPUTDATA}/multiSpectInterp.tif
	${INPUTDATA}/panchro.tif
	${TEMP}/fuTvBayesianFusion.tif
)


# -------    otb::SimpleRcsPanSharpeningFusionImageFilter   -------------------
ADD_TEST(fuTuSimpleRcsPanSharpeningFusionImageFilterNew ${FUSION_TESTS1}
        otbSimpleRcsPanSharpeningFusionImageFilterNew
)

ADD_TEST(fuTvSimpleRcsPanSharpeningFusionImageFilter ${FUSION_TESTS1}
    --compare-image ${EPSILON_8}  ${BASELINE}/fuTvRcsPanSharpeningFusion.tif
                           ${TEMP}/fuTvRcsPanSharpeningFusion.tif
        otbSimpleRcsPanSharpeningFusionImageFilter
	${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
	${INPUTDATA}/QB_Toulouse_Ortho_XS.tif
	${TEMP}/fuTvRcsPanSharpeningFusion.tif
)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbFusion2 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# A enrichir
SET(Fusion_SRCS1
otbFusionTests1.cxx
otbFusionImageBaseNew.cxx
otbBayesianFusionFilterNew.cxx
otbBayesianFusionFilter.cxx
otbSimpleRcsPanSharpeningFusionImageFilterNew.cxx
otbSimpleRcsPanSharpeningFusionImageFilter.cxx
)

OTB_ADD_EXECUTABLE(otbFusionTests1 "${Fusion_SRCS1}" "OTBFusion;OTBIO;OTBTesting")

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

