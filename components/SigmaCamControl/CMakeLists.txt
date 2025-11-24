file(GLOB_RECURSE COMPONENT_SRCS  "*.cpp")
list(FILTER COMPONENT_SRCS EXCLUDE REGEX ".*test.*\\.cpp$")

idf_component_register(
    SRCS ${COMPONENT_SRCS}
    INCLUDE_DIRS "include"
    REQUIRES esp_timer esp_system esp_http_client esp_netif
)
