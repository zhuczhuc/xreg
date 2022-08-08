
set(EXE_NAME "${zhuc_XREG_EXE_PREFIX}creat-mesh")

add_executable(${EXE_NAME} xreg_create_mesh_main.cpp)

target_link_libraries(${EXE_NAME} PUBLIC ${XREG_EXE_LIBS_TO_LINK})

install(TARGETS ${EXE_NAME})

