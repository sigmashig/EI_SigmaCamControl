file(GLOB_RECURSE COMPONENT_SRCS "main.cpp" "*.cpp")

idf_component_register(SRCS ${COMPONENT_SRCS}
                    INCLUDE_DIRS "." "../components/SigmaCamControl/" "../components/SigmaCamControl/include/"
                    REQUIRES SigmaCamControl esp_system)

