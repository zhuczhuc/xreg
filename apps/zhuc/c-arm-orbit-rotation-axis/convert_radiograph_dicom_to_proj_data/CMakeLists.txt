
set(EXE_NAME "${zhuc_XREG_EXE_PREFIX}convert-dicom-radiograph")

add_executable(${EXE_NAME} convert_radiograph_dcm_to_proj_data_main.cpp)

target_link_libraries(${EXE_NAME} PUBLIC ${XREG_EXE_LIBS_TO_LINK})

install(TARGETS ${EXE_NAME})