cmake_minimum_required(VERSION 4.2)
project(wordforge-project)

add_executable(wordforge 
    main.c
    forge/forge.c
    utils/file_manager.c
    utils/data_manager.c
    utils/prompt_manager.c
    refine/refine.c
    cjson/cJSON.c
    stats/stats.c
)
target_include_directories(wordforge PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)