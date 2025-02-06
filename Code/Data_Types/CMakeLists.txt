add_library(Data_Types
  date.cpp
  duration.cpp
)

target_include_directories(Data_Types PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_options(Data_Types PUBLIC -std=c++17)
