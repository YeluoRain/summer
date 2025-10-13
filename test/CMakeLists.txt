cmake_minimum_required(VERSION 3.23)

aux_source_directory(. TEST_SRC)
add_executable(summer_test ${TEST_SRC})
target_include_directories(summer_test PUBLIC
        "${REPO_DIR}/include"
        "${ROOT_DIR}/summer")
target_link_directories(summer_test PUBLIC "${REPO_DIR}/lib")
target_link_libraries(summer_test gtest gtest_main)
add_test(NAME summer_test COMMAND summer_test)