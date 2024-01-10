cmake_minimum_required(VERSION 3.8)
project(camera_fov_rviz_plugin)

# Default to C++17
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()


if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(Qt5 REQUIRED COMPONENTS Widgets)
find_package(pluginlib REQUIRED)
find_package(rviz_common REQUIRED)
find_package(rviz_rendering REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)


## This setting causes Qt's "MOC" generation to happen automatically.
set(CMAKE_AUTOMOC ON)

# specify includes
include_directories(
	include
)

# specify sources
set(SRC_FILES
	src/camera_fov_display.cpp
	src/camera_fov_visual.cpp
	include/camera_fov_rviz_plugin/camera_fov_display.h
	include/camera_fov_rviz_plugin/camera_fov_visual.h
)


## An rviz plugin is just a shared library

add_library(${PROJECT_NAME} SHARED ${SRC_FILES})
ament_target_dependencies(${PROJECT_NAME}
  "geometry_msgs"
  "rclcpp"
  "rviz_common"
  "rviz_rendering"
  "sensor_msgs")
target_include_directories(${PROJECT_NAME} PUBLIC
  ${Qt5Widgets_INCLUDE_DIRS})


target_compile_definitions(${PROJECT_NAME}  PRIVATE "RVIZ_DEFAULT_PLUGINS_BUILDING_LIBRARY")

# prevent pluginlib from using boost
target_compile_definitions(${PROJECT_NAME}  PUBLIC "PLUGINLIB__DISABLE_BOOST_FUNCTIONS")




ament_export_include_directories(include)
ament_export_targets(${PROJECT_NAME} HAS_LIBRARY_TARGET)

ament_export_dependencies(
  geometry_msgs
  rclcpp
  rviz_common
  rviz_rendering
  sensor_msgs)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  ament_lint_auto_find_test_dependencies()
endif()

install(
  TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES DESTINATION include
)

install(
  DIRECTORY include/
  DESTINATION include
)

install(
  DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/icons"
  DESTINATION "share/${PROJECT_NAME}"
)


## Here we export the plugins and meshes so they can be found by RViz at runtime.
pluginlib_export_plugin_description_file(rviz_common plugin_description.xml)

ament_package()
