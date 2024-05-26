cmake_minimum_required(VERSION 3.15)
project(CServ)

# set cmake to export compile commands to clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# add the source files
file(GLOB_RECURSE SOURCES 
  ${CMAKE_SOURCE_DIR}/src/*.c
)

# add the executable
add_executable(cserv ${SOURCES})

# specify the directories with included header files
target_include_directories(cserv PRIVATE
  ${CMAKE_SOURCE_DIR}/src
  ${CMAKE_SOURCE_DIR}/src/ArgumentHandler
  ${CMAKE_SOURCE_DIR}/src/Receiver
)
