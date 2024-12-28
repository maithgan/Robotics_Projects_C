cmake_minimum_required(VERSION 3.10)
project(climber)

# 1. create a library using climber.cpp and set it's include path
# 2. create an executable using print_map.cpp and link it to the climber library
# 3. create an executable using run_climber.cpp and link it to the climber libraryr
# --- Your code here
add_library(climber climber.cpp)

add_executable(print_map print_map.cpp)

add_executable(run_climber run_climber.cpp)

target_link_libraries(print_map PUBLIC climber)
target_link_libraries(run_climber PUBLIC climber)
# ---
