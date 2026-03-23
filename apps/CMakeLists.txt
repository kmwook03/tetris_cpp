file(GLOB_RECURSE APP_SRC_FILES CONFIGURE_DEPENDS
  ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_executable(app ${APP_SRC_FILES})

# Tetris_cpp가 PUBLIC으로 include를 노출하므로, 여기서 별도로 include 디렉토리를 지정할 필요가 없습니다.
target_link_libraries(app PRIVATE Tetris_cpp ${CURSES_LIBRARIES})

target_compile_options(app PRIVATE -Wall -Wextra -Wpedantic)
