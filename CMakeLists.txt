cmake_minimum_required(VERSION 3.11)

project(
  Tetris_cpp
  VERSION 0.2
  DESCRIPTION "Tetris in cpp"
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Curses REQUIRED)

# 전역 설정보다는 타겟별 설정을 권장하지만, 출력 디렉토리는 여기서 유지합니다.
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(src)
add_subdirectory(apps)
# add_subdirectory(tests) # 나중에 테스트 추가 시 활성화
