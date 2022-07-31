set(EXE_NAME "${zhuc_XREG_EXE_PREFIX}crop-vol")

add_executable(${EXE_NAME} xreg_crop_vol_main.cpp)

target_link_libraries(${EXE_NAME} PUBLIC ${XREG_EXE_LIBS_TO_LINK})

install(TARGETS ${EXE_NAME})

