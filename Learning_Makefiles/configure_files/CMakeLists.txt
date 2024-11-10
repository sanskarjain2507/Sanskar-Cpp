set(PROJECT_VERSION "1.0.0")
set(OTHER_TEXT "123")

configure_file(
  ${CMAKE_SOURCE_DIR}/config.h.in
  ${CMAKE_SOURCE_DIR}/config.h
  COPYONLY
)
