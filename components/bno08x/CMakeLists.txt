set(BNO08X_ROOT ${CMAKE_CURRENT_LIST_DIR}/../../HF-BNO08x)
set(SH2_ROOT ${BNO08X_ROOT}/src/sh2)

set(SRCS
    ${BNO08X_ROOT}/src/BNO085.cpp
    ${SH2_ROOT}/euler.c
    ${SH2_ROOT}/sh2.c
    ${SH2_ROOT}/sh2_SensorValue.c
    ${SH2_ROOT}/sh2_util.c
    ${SH2_ROOT}/shtp.c
    esp_i2c_transport.cpp
)

idf_component_register(SRCS ${SRCS}
                       INCLUDE_DIRS include ${BNO08X_ROOT}/src ${SH2_ROOT}
                       REQUIRES driver)
