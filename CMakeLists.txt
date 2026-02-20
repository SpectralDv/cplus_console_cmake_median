cmake_minimum_required(VERSION 3.24)
project(MyBoostProject)
set(APP_NAME "csv_median_calculator")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) # OFF для -std=c++23, ON для -std=gnu++23


include(FetchContent)
# Скачивание и сборка Boost
# FetchContent_Declare(
    #boost
    #URL https://archives.boost.io/release/1.85.0/source/boost_1_85_0.tar.bz2
    #URL_HASH SHA256=7009fe1faa1697476bdc7027703a2badb84e849b7b0baad5086b087b971f8617
    #DOWNLOAD_EXTRACT_TIMESTAMP TRUE  # Для CMake 3.24+
# )
# FetchContent_MakeAvailable(boost)

# Настройка режима сборки
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
endif()

# Поиск установленного Boost
find_package(Boost REQUIRED COMPONENTS program_options system thread filesystem)
# Создаем импортированную библиотеку

# Если Boost найден, добавляем его в проект
if(Boost_FOUND)

message(STATUS "Boost version: ${Boost_VERSION}")
message(STATUS "Boost libraries: ${Boost_LIBRARIES}")
message(STATUS "Boost include: ${Boost_INCLUDE_DIRS}")
if(WIN32)
message(STATUS "Boost dll: ${BOOST_DLL_DIR}")
endif()

include_directories(${Boost_INCLUDE_DIRS})


file(GLOB_RECURSE SOURCES 
    LIST_DIRECTORIES false
    "*.cpp"
    "*.cxx"
    "*.cc"
)
list(FILTER SOURCES EXCLUDE REGEX ".*/build/.*|.*/cmake-build-.*/.*")
message(STATUS "Found .cpp files: ${SOURCES}")

add_executable(${APP_NAME} ${SOURCES})
# add_executable(${APP_NAME} main.cpp)

# Линковка csv_median_calculator
target_link_libraries(${APP_NAME} PRIVATE ${Boost_LIBRARIES})
target_include_directories(${APP_NAME} PRIVATE ${Boost_INCLUDE_DIRS})

find_package(spdlog REQUIRED CONFIG)
target_link_libraries(${APP_NAME} PRIVATE spdlog::spdlog)

if(WIN32)
# Находим конкретную DLL
file(GLOB BOOST_DLLS "${BOOST_DLL_DIR}/*program_options*.dll")

set(BOOST_DLL_PATH "C:/vcpkg/installed/x64-windows/debug/bin")
message(STATUS "Boost dll: ${BOOST_DLL_PATH}")

# Копирование DLL в выходную директорию
add_custom_command(TARGET ${APP_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${BOOST_DLL_PATH}/boost_program_options-vc143-mt-gd-x64-1_90.dll"
        $<TARGET_FILE_DIR:${APP_NAME}>
)
endif()

endif()


