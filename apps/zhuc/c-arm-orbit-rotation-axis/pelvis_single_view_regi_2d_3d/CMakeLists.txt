
set(EXE_NAME "${zhuc_XREG_EXE_PREFIX}hip-surg-pelvis-single-view-regi-2d-3d")

add_executable(${EXE_NAME} xreg_hip_single_regi_2d_3d_main.cpp)

target_link_libraries(${EXE_NAME} PUBLIC ${XREG_EXE_LIBS_TO_LINK})

install(TARGETS ${EXE_NAME})
