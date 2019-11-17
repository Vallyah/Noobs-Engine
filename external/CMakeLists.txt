include(ExternalProject)
find_package(Git REQUIRED)
# find_package(Hg REQUIRED)
include(cleanCppExtensions)

#------------------------------------------------------------------------------
# Convenience update all, dependencies are added in ExternalHeaderOnly_Add
add_custom_target(update COMMENT "Updated all external projects")

#------------------------------------------------------------------------------
# Catch - Unit test header only library

# For details about ExternalHeaderOnly_Add see cmake/cleanCppExtensions.cmake

# target_link_libraries (exampleApp PRIVATE catch)
# #include <catch.hpp>
ExternalHeaderOnly_Add(Catch
    "https://github.com/catchorg/Catch2.git" "master" "single_include/catch2")

#------------------------------------------------------------------------------
# Examples of other external projects
#------------------------------------------------------------------------------

#------------------------------------------------------------------------------
# target_link_libraries (exampleApp PRIVATE json)
# #include <nlohmann/json.hpp>
# ExternalHeaderOnly_Add(json
#     "https://github.com/nlohmann/json.git" "master" "single_include")

#------------------------------------------------------------------------------
# target_link_libraries (exampleApp PRIVATE spdlog)
# #include <spdlog/spdlog.h>
# Bundled fmt can be used: #include <spdlog/fmt/fmt.h>
# spdlog::error("Some error message with arg: {}", 1);
# ExternalHeaderOnly_Add(spdlog
#     "https://github.com/gabime/spdlog.git" "v1.x" "include")

#------------------------------------------------------------------------------
# target_link_libraries (noobsEngine PRIVATE GLFW)
# #include <GLFW/glfw3.h>
ExternalDownloadNowGit(GLFW
    https://github.com/glfw/glfw master)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(${GLFW_SOURCE_DIR})

#------------------------------------------------------------------------------
# target_link_libraries (noobsEngine PRIVATE GLAD)
# #include <glad/glad.h>
add_library(GLAD STATIC ${CMAKE_CURRENT_SOURCE_DIR}/Glad/src/glad.c)
    target_include_directories(GLAD SYSTEM PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Glad/include)

#------------------------------------------------------------------------------
# target_link_libraries (noobsEngine PRIVATE stb)
# #include <stb_image.h>
add_library(stb STATIC ${CMAKE_CURRENT_SOURCE_DIR}/Stb/src/stb_image.cpp)
target_include_directories(stb SYSTEM PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Stb/include)

#------------------------------------------------------------------------------
# target_link_libraries (noobsEngine PRIVATE glm)
# #include <glm/glm.hpp>
# #include <glm/gtc/matrix_transform.hpp>
# #include <glm/gtc/type_ptr.hpp>
ExternalHeaderOnly_Add(glm
    "https://github.com/g-truc/glm.git" "master" "")

#------------------------------------------------------------------------------

# target_link_libraries (noobsEngine PRIVATE assimp)
# #include <assimp/Importer.hpp>
# #include <assimp/scene.h>
# #include <assimp/postprocess.h>
ExternalDownloadNowGit(assimp
    https://github.com/assimp/assimp assimp_5.0_release)
add_subdirectory(${assimp_SOURCE_DIR})
#------------------------------------------------------------------------------
# target_link_libraries (exampleApp PRIVATE doctest)
# #include <doctest.h>
# ExternalHeaderOnly_Add("DocTest"
#     "https://github.com/onqtam/doctest.git" "master" "src/DocTest")

#------------------------------------------------------------------------------
# target_link_libraries (exampleApp PRIVATE fmt)
# #include <fmt/format.h>
# Use with: `fmt::print("Hello {} - {}!\n", "world", 3.56f);`

# ExternalDownloadNowGit(fmt "https://github.com/fmtlib/fmt.git" "master")
# add_library(fmt STATIC ${fmt_SOURCE_DIR}/src/format.cc)
# target_include_directories(fmt SYSTEM PUBLIC ${fmt_SOURCE_DIR}/include)
# # target_compile_definitions(spdlog INTERFACE -DSPDLOG_FMT_EXTERNAL) #if using SPDLOG


#------------------------------------------------------------------------------
# cpr - HTTP(S) requests library with nice interface

# target_link_libraries (exampleApp PRIVATE fmt)
# #include <cpr/cpr.h>
# auto r = cpr::Get(cpr::Url{"https://ohnheiser.cz"});
# cout<<r.text<<endl;

# find_package(CURL REQUIRED)
# ExternalDownloadNowGit(cpr https://github.com/finkandreas/cpr.git master)
# option(BUILD_CPR_TESTS "" OFF) #disable tests
# option(USE_SYSTEM_CURL "" ON ) #use system curl
# add_subdirectory(${cpr_SOURCE_DIR} EXCLUDE_FROM_ALL)
# # Make headers come as SYSTEM, to get rid of all warning on included headers
# target_include_directories(cpr SYSTEM PUBLIC ${CPR_INCLUDE_DIRS} ${CURL_INCLUDE_DIRS})


