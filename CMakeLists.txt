cmake_minimum_required(VERSION 4.2)
project(wordforge-project)

add_executable(wordforge 
    main.c
    forge/forge.c
    utils/file_manager.c
    refine/refine.c
)
target_include_directories(wordforge PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)