file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
  ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_library(Tetris_cpp STATIC ${SRC_FILES})

# PUBLIC으로 설정하여 이 라이브러리를 사용하는 타겟들이 자동으로 include 폴더를 참조하게 합니다.
target_include_directories(Tetris_cpp PUBLIC ${CMAKE_SOURCE_DIR}/include)

target_compile_options(Tetris_cpp PRIVATE -Wall -Wextra -Wpedantic)

target_link_libraries(Tetris_cpp PRIVATE ${CURSES_LIBRARIES})
