add_executable(DemoTfIdf DemoTfIdf.cpp)
target_include_directories(DemoTfIdf PRIVATE ${CMAKE_SOURCE_DIR}/hanlp)
target_link_libraries(DemoTfIdf HanlpLibrary)
