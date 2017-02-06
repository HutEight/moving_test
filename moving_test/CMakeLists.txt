cmake_minimum_required(VERSION 2.8.3)
project(moving_test)


find_package(catkin REQUIRED COMPONENTS
  std_msgs
)
find_package(roscpp REQUIRED)
find_package(roslib REQUIRED)

include_directories(
  ${catkin_INCLUDE_DIRS}
)


catkin_package(
	INCLUDE_DIRS ${catkin_INCLUDE_DIRS}
)

add_executable(moving_test src/moving_test.cpp)

target_link_libraries(moving_test ${catkin_LIBRARIES})
