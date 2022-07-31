set(EXE_NAME "${zhuc_XREG_EXE_PREFIX}convert-dicom-vols")

add_executable(${EXE_NAME} xreg-convert-dicom-vols-main.cpp)

target_link_libraries(${EXE_NAME} PUBLIC ${XREG_EXE_LIBS_TO_LINK})

install(TARGETS ${EXE_NAME})
