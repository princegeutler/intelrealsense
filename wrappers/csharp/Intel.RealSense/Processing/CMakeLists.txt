target_sources(${LRS_DOTNET_TARGET}
    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/Align.cs"
        "${CMAKE_CURRENT_LIST_DIR}/Colorizer.cs"
        "${CMAKE_CURRENT_LIST_DIR}/CustomProcessingBlock.cs"
        "${CMAKE_CURRENT_LIST_DIR}/DecimationFilter.cs"
        "${CMAKE_CURRENT_LIST_DIR}/DisparityTransform.cs"
        "${CMAKE_CURRENT_LIST_DIR}/FrameSource.cs"
        "${CMAKE_CURRENT_LIST_DIR}/HoleFillingFilter.cs"
        "${CMAKE_CURRENT_LIST_DIR}/PointCloud.cs"
        "${CMAKE_CURRENT_LIST_DIR}/ProcessingBlock.cs"
        "${CMAKE_CURRENT_LIST_DIR}/SpatialFilter.cs"
        "${CMAKE_CURRENT_LIST_DIR}/Syncer.cs"
        "${CMAKE_CURRENT_LIST_DIR}/TemporalFilter.cs"
        "${CMAKE_CURRENT_LIST_DIR}/ZeroOrderInvalidationFilter.cs"
        "${CMAKE_CURRENT_LIST_DIR}/ProcessingBlockList.cs"
        "${CMAKE_CURRENT_LIST_DIR}/ThresholdFilter.cs"
        "${CMAKE_CURRENT_LIST_DIR}/UnitsTransform.cs"
)
