
set(EXE_NAME "${zhuc_XREG_EXE_PREFIX}proj-est-orbital-rot")

add_executable(${EXE_NAME} xreg_proj_est_orbital_rot_main.cpp)

target_link_libraries(${EXE_NAME} PUBLIC ${XREG_EXE_LIBS_TO_LINK})

install(TARGETS ${EXE_NAME})

