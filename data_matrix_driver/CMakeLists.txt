cmake_minimum_required(VERSION 3.0.2)
project(data_matrix_driver)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  message_generation
  message_runtime
)


catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES data_matrix_driver
#  CATKIN_DEPENDS roscpp
#  DEPENDS system_lib
)


include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

add_executable(${PROJECT_NAME}_node src/camera.cpp src/serial.cpp src/main.cpp)

target_link_libraries(${PROJECT_NAME}_node
   ${catkin_LIBRARIES}
 )

