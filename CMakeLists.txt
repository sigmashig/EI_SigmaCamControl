# The following lines of boilerplate have to be in your project
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.16)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(SigmaCamControl VERSION 0.1.0)
idf_build_set_property(PROJECT_VER ${PROJECT_VERSION})

# Exclude deprecated driver source files to avoid compilation errors
# Must run after project() when __idf_driver target exists
#if(TARGET __idf_driver)
#    get_target_property(driver_srcs __idf_driver SOURCES)
#    if(driver_srcs)
#        # Remove all deprecated driver source files
#        set(filtered_srcs "")
#        foreach(src ${driver_srcs})
#            string(TOLOWER "${src}" src_lower)
#            if(NOT src_lower MATCHES ".*driver.*deprecated.*")
#                list(APPEND filtered_srcs "${src}")
#            endif()
#        endforeach()
#        set_target_properties(__idf_driver PROPERTIES SOURCES "${filtered_srcs}")
#    endif()
#endif()

