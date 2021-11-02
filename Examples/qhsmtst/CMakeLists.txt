add_executable(qhsmtst "original_qhsmtst.cpp" "test.cpp")
target_include_directories(qhsmtst PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(qhsmtst PUBLIC KaoriFramework_LIB)