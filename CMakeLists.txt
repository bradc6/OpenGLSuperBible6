#OpenGL Superbible sample code with a SDL2 twist ;)
#If you wish to compile with clang use
#"make -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ .."
#To make a release verion use the build type options
#"cmake -DCMAKE_BUILD_TYPE=Release .."
project(GraphicsSamples)
cmake_minimum_required(VERSION 2.8.6)

#We will assume you always want to build this project
#with debugging symbols
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING
      "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
endif(NOT CMAKE_BUILD_TYPE)

#Use C++11 if possible (or the most recent that we can use
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++14" COMPILER_SUPPORTS_CXX14)
if(COMPILER_SUPPORTS_CXX14)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
else()
        message(FATAL "The compiler ${CMAKE_CXX_COMPILER} has no C++14 support. Please use a different C++ compiler.")
endif()

#Sets the directory for cmake to find Cmake configration packages,
#these packages allow for cmake to find libraries that are not included
#in the standard installation of Cmake. The standard packages can be
#found @ /usr/share/cmake-2.8/Modules
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules
                      ${CMAKE_MODULE_PATH})

#Set the current version of the Graphics Examples
set(GraphicsSamples_MAJOR_VERSION 0)
set(GraphicsSamples_MINOR_VERSION 0)
set(GraphicsSamples_PATCH_VERSION 1)

#These variables will act as our list of include folders and linker flags
#for compilation
set(GraphicsSamplesLinkerFlags)
set(GraphicsSamplesIncludeDirectories)
set(GraphicsSamplesLinkerDirectories)

#Set the local source root directory.
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Source)
set(LIBRARIES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Libraries)
add_definitions(-DSOURCEDIR=${CMAKE_CURRENT_SOURCE_DIR})
add_definitions(-DTEXTUREDIR=${CMAKE_CURRENT_SOURCE_DIR}/Textures)
add_definitions(-DMODELDIR=${CMAKE_CURRENT_SOURCE_DIR}/Models)


###Platform dependant include/linker Directories
###Add fakeroot directores for different platforms
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    list(APPEND GraphicsSamplesIncludeDirectories "$ENV{HOMEPATH}/dev/fakeroot/include")
    list(APPEND GraphicsSamplesLinkerDirectories "$ENV{HOMEPATH}/dev/fakeroot/lib")
    include_directories("C:/fakeroot/include")
    link_directories("C:/fakeroot/lib")
    add_definitions(-DTARGETWINDOWS=1)
    if(MSVC)
        add_definitions(/arch:SSE2)
    endif(MSVC)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    list(APPEND GraphicsSamplesIncludeDirectories "~/dev/fakeroot/include")
    list(APPEND GraphicsSamplesLinkerDirectories "~/dev/fakeroot/lib")
    list(APPEND GraphicsSamplesIncludeDirectories "/opt/local/include")
    list(APPEND GraphicsSamplesLinkerDirectories "/opt/local/lib")
    list(APPEND GraphicsSamplesLinkerFlags "-framework Cocoa")
    list(APPEND CMAKE_PREFIX_PATH /opt/local)
    add_definitions(-DTARGETMACOSX=1)
else()
    list(APPEND GraphicsSamplesIncludeDirectories "~/dev/fakeroot/include")
    list(APPEND GraphicsSamplesLinkerDirectories "~/dev/fakeroot/lib")
endif()

#Required libraries to build the different
#components of the tutorials. Find them and add the include/linker
#directories and flags(In case the package manager find it in a weird place)
find_package(SDL2 REQUIRED)
list(APPEND GraphicsSamplesIncludeDirectories ${SDL2_INCLUDE_DIR})
list(APPEND GraphicsSamplesLinkerFlags ${SDL2_LIBRARY})

find_package(SDL2_image)
list(APPEND GraphicsSamplesIncludeDirectories ${SDL2_IMAGE_INCLUDE_DIRS})
list(APPEND GraphicsSamplesLinkerFlags ${SDL2_IMAGE_LIBRARIES})

find_package(OpenGL REQUIRED)
list(APPEND GraphicsSamplesIncludeDirectories ${OPENGL_INCLUDE_DIR})
list(APPEND GraphicsSamplesLinkerFlags ${OPENGL_LIBRARIES})

find_package(GLEW REQUIRED)
list(APPEND GraphicsSamplesIncludeDirectories ${GLEW_INCLUDE_DIRS})
list(APPEND GraphicsSamplesLinkerFlags ${GLEW_LIBRARIES})
include_directories(${GLEW_INCLUDE_DIRS})

find_package(GLM REQUIRED)
list(APPEND GraphicsSamplesIncludeDirectories ${GLM_INCLUDE_DIRS})

#Add additional cmake subprojects to ours.
add_subdirectory(${LIBRARIES_DIR}/KTX/build/cmake)
include_directories(${LIBRARIES_DIR}/KTX/include)
#Add TinyObjLoader
add_subdirectory(${LIBRARIES_DIR}/TinyObjLoader)
include_directories(${LIBRARIES_DIR}/TinyObjLoader)

#With all of those libraries and user defined paths added,
#lets give them to the compiler and linker.
include_directories(GraphicsSamplesIncludeDirectories)
link_directories(GraphicsSamplesLinkerDirectories)

set(TUTORIAL1_DIR "${SOURCE_DIR}/Tutorial1")
set(TUTORIAL2_DIR "${SOURCE_DIR}/Tutorial2")
set(TUTORIAL3_DIR "${SOURCE_DIR}/Tutorial3")
set(TUTORIAL4_DIR "${SOURCE_DIR}/Tutorial4")
set(TUTORIAL5_DIR "${SOURCE_DIR}/Tutorial5")
set(TUTORIAL6_DIR "${SOURCE_DIR}/Tutorial6")
set(TUTORIAL7_DIR "${SOURCE_DIR}/Tutorial7")
set(TUTORIAL8_DIR "${SOURCE_DIR}/Tutorial8")
set(TUTORIAL9_DIR "${SOURCE_DIR}/Tutorial9")
set(TUTORIAL10_DIR "${SOURCE_DIR}/Tutorial10")
set(TUTORIAL11_DIR "${SOURCE_DIR}/Tutorial11")
set(TUTORIAL12_DIR "${SOURCE_DIR}/Tutorial12")
set(TUTORIAL13_DIR "${SOURCE_DIR}/Tutorial13")
set(TUTORIAL14_DIR "${SOURCE_DIR}/Tutorial14")
set(TUTORIAL15_DIR "${SOURCE_DIR}/Tutorial15")
set(TUTORIAL16_DIR "${SOURCE_DIR}/Tutorial16")
set(TUTORIAL17_DIR "${SOURCE_DIR}/Tutorial17")
set(TUTORIAL18_DIR "${SOURCE_DIR}/Tutorial18")
set(TUTORIAL19_DIR "${SOURCE_DIR}/Tutorial19")
set(TUTORIAL20_DIR "${SOURCE_DIR}/Tutorial20")
set(TUTORIAL21_DIR "${SOURCE_DIR}/Tutorial21")
set(TUTORIAL22_DIR "${SOURCE_DIR}/Tutorial22")
set(TUTORIAL23_DIR "${SOURCE_DIR}/Tutorial23")
set(TUTORIAL24_DIR "${SOURCE_DIR}/Tutorial24")
set(TUTORIAL25_DIR "${SOURCE_DIR}/Tutorial25")
set(TUTORIAL26_DIR "${SOURCE_DIR}/Tutorial26")
set(TUTORIAL27_DIR "${SOURCE_DIR}/Tutorial27")
set(TUTORIAL28_DIR "${SOURCE_DIR}/Tutorial28")
set(TUTORIAL29_DIR "${SOURCE_DIR}/Tutorial29")
set(TUTORIAL30_DIR "${SOURCE_DIR}/Tutorial30")

#Extra tutorials not included in the book.
set(TUTORIALSDL2IMAGE_DIR "${SOURCE_DIR}/TutorialSDL2Image")
set(ENGINE_DIR "${SOURCE_DIR}/Engine")
set(SIMPLESCENE_DIR "${SOURCE_DIR}/SimpleScene")
set(TUTORIALSEPERATESHADERS_DIR "${SOURCE_DIR}/TutorialSeperateShaders")
set(TUTORIALDIRECTSTATEACCESS_DIR "${SOURCE_DIR}/TutorialDirectStateAccess")

set(Tutorial1-Source
	${TUTORIAL1_DIR}/main.hpp
	${TUTORIAL1_DIR}/main.cpp
	)

set(Tutorial2-Source
	${TUTORIAL2_DIR}/main.hpp
	${TUTORIAL2_DIR}/main.cpp
	)

set(Tutorial3-Source
	${TUTORIAL3_DIR}/main.hpp
	${TUTORIAL3_DIR}/main.cpp
	${TUTORIAL3_DIR}/Shaders/Main.glsl.vert
	${TUTORIAL3_DIR}/Shaders/Main.glsl.frag
	)

set(Tutorial4-Source
	${TUTORIAL4_DIR}/main.hpp
	${TUTORIAL4_DIR}/main.cpp
	${TUTORIAL4_DIR}/Shaders/Main.glsl.vert
	${TUTORIAL4_DIR}/Shaders/Main.glsl.frag
	)

set(Tutorial5-Source
	${TUTORIAL5_DIR}/main.hpp
	${TUTORIAL5_DIR}/main.cpp
	${TUTORIAL5_DIR}/Shaders/Main.glsl.vert
	${TUTORIAL5_DIR}/Shaders/Main.glsl.frag
	)

set(Tutorial6-Source
	${TUTORIAL6_DIR}/main.hpp
	${TUTORIAL6_DIR}/main.cpp
	${TUTORIAL6_DIR}/Shaders/Main.glsl.vert
	${TUTORIAL6_DIR}/Shaders/Main.glsl.frag
	)

set(Tutorial7-Source
	${TUTORIAL7_DIR}/main.hpp
	${TUTORIAL7_DIR}/main.cpp
	${TUTORIAL7_DIR}/Shaders/Main.glsl.vert
	${TUTORIAL7_DIR}/Shaders/Main.glsl.frag
	)

set(Tutorial8-Source
	${TUTORIAL8_DIR}/main.hpp
	${TUTORIAL8_DIR}/main.cpp
	${TUTORIAL8_DIR}/Shaders/Main.glsl.vert
	${TUTORIAL8_DIR}/Shaders/Main.glsl.tesc
	${TUTORIAL8_DIR}/Shaders/Main.glsl.tese
	${TUTORIAL8_DIR}/Shaders/Main.glsl.frag
	)

set(Tutorial9-Source
	${TUTORIAL9_DIR}/main.hpp
	${TUTORIAL9_DIR}/main.cpp
	${TUTORIAL9_DIR}/Shaders/Main.glsl.vert
	${TUTORIAL9_DIR}/Shaders/Main.glsl.tesc
	${TUTORIAL9_DIR}/Shaders/Main.glsl.tese
	${TUTORIAL9_DIR}/Shaders/Main.glsl.geom
	${TUTORIAL9_DIR}/Shaders/Main.glsl.frag
	)

set(Tutorial10-Source
	${TUTORIAL10_DIR}/main.hpp
	${TUTORIAL10_DIR}/main.cpp
	${TUTORIAL10_DIR}/Shaders/Main.glsl.vert
	${TUTORIAL10_DIR}/Shaders/Main.glsl.tesc
	${TUTORIAL10_DIR}/Shaders/Main.glsl.tese
	${TUTORIAL10_DIR}/Shaders/Main.glsl.geom
	${TUTORIAL10_DIR}/Shaders/Main.glsl.frag
	)

set(Tutorial11-Source
	${TUTORIAL11_DIR}/main.hpp
	${TUTORIAL11_DIR}/main.cpp	
	${TUTORIAL11_DIR}/Shaders/Main.glsl.vert
	${TUTORIAL11_DIR}/Shaders/Main.glsl.frag
	)

set(Tutorial12-Source
	${TUTORIAL12_DIR}/main.hpp
	${TUTORIAL12_DIR}/main.cpp
	${TUTORIAL12_DIR}/Shaders/Main.glsl.vert
	${TUTORIAL12_DIR}/Shaders/Main.glsl.frag
	)

set(Tutorial13-Source
	${TUTORIAL13_DIR}/main.hpp
	${TUTORIAL13_DIR}/main.cpp
	${TUTORIAL13_DIR}/Shaders/Main.glsl.vert
	${TUTORIAL13_DIR}/Shaders/Main.glsl.frag
	)

set(Tutorial14-Source
	${TUTORIAL14_DIR}/main.hpp
	${TUTORIAL14_DIR}/main.cpp
	${TUTORIAL14_DIR}/Shaders/Main.glsl.vert
	${TUTORIAL14_DIR}/Shaders/Main.glsl.frag
	)

set(Tutorial15-Source
	${TUTORIAL15_DIR}/main.hpp
	${TUTORIAL15_DIR}/main.cpp
        ${TUTORIAL15_DIR}/Shaders/Main.glsl.vert
        ${TUTORIAL15_DIR}/Shaders/Main.glsl.frag
	)

set(Tutorial16-Source
	${TUTORIAL16_DIR}/main.hpp
	${TUTORIAL16_DIR}/main.cpp
        ${TUTORIAL16_DIR}/Shaders/Main.glsl.vert
        ${TUTORIAL16_DIR}/Shaders/Main.glsl.frag
	)

set(Tutorial17-Source
	${TUTORIAL17_DIR}/main.hpp
	${TUTORIAL17_DIR}/main.cpp
        ${TUTORIAL17_DIR}/Shaders/Main.glsl.vert
        ${TUTORIAL17_DIR}/Shaders/Main.glsl.frag
	)

set(Tutorial18-Source
	${TUTORIAL18_DIR}/main.hpp
	${TUTORIAL18_DIR}/main.cpp
        ${TUTORIAL18_DIR}/Shaders/Main.glsl.vert
        ${TUTORIAL18_DIR}/Shaders/Main.glsl.frag
	)

set(Tutorial19-Source
	${TUTORIAL19_DIR}/main.hpp
	${TUTORIAL19_DIR}/main.cpp
        ${TUTORIAL19_DIR}/Shaders/Main.glsl.vert
        ${TUTORIAL19_DIR}/Shaders/Main.glsl.frag
	)

set(Tutorial20-Source
	${TUTORIAL20_DIR}/main.hpp
	${TUTORIAL20_DIR}/main.cpp
        ${TUTORIAL20_DIR}/Shaders/Main.glsl.vert
        ${TUTORIAL20_DIR}/Shaders/Main.glsl.frag
	)

set(Tutorial21-Source
        ${TUTORIAL21_DIR}/main.hpp
        ${TUTORIAL21_DIR}/main.cpp
	${TUTORIAL21_DIR}/Shaders/Clear.glsl.vert
	${TUTORIAL21_DIR}/Shaders/Clear.glsl.frag
	${TUTORIAL21_DIR}/Shaders/Append.glsl.vert
	${TUTORIAL21_DIR}/Shaders/Append.glsl.frag
	${TUTORIAL21_DIR}/Shaders/Resolve.glsl.vert
	${TUTORIAL21_DIR}/Shaders/Resolve.glsl.frag
        )

set(Tutorial22-Source
        ${TUTORIAL22_DIR}/main.hpp
        ${TUTORIAL22_DIR}/main.cpp
        )

set(Tutorial23-Source
        ${TUTORIAL23_DIR}/main.hpp
        ${TUTORIAL23_DIR}/main.cpp
        )

set(Tutorial24-Source
        ${TUTORIAL24_DIR}/main.hpp
        ${TUTORIAL24_DIR}/main.cpp
        )

set(Tutorial25-Source
        ${TUTORIAL25_DIR}/main.hpp
        ${TUTORIAL25_DIR}/main.cpp
        )

set(Tutorial26-Source
        ${TUTORIAL26_DIR}/main.hpp
        ${TUTORIAL26_DIR}/main.cpp
        )

set(Tutorial27-Source
        ${TUTORIAL27_DIR}/main.hpp
        ${TUTORIAL27_DIR}/main.cpp
        )

set(Tutorial28-Source
        ${TUTORIAL28_DIR}/main.hpp
        ${TUTORIAL28_DIR}/main.cpp
        )

set(Tutorial29-Source
        ${TUTORIAL29_DIR}/main.hpp
        ${TUTORIAL29_DIR}/main.cpp
        )

set(Tutorial30-Source
        ${TUTORIAL30_DIR}/main.hpp
        ${TUTORIAL30_DIR}/main.cpp
        )

set(TutorialSDL2Image-Source
	${TUTORIALSDL2IMAGE_DIR}/main.hpp
	${TUTORIALSDL2IMAGE_DIR}/main.cpp
        ${TUTORIALSDL2IMAGE_DIR}/Shaders/Main.glsl.vert
        ${TUTORIALSDL2IMAGE_DIR}/Shaders/Main.glsl.frag
	)

set(TutorialSeprateShaders-Source
        ${TUTORIALSEPERATESHADERS_DIR}/main.hpp
        ${TUTORIALSEPERATESHADERS_DIR}/main.cpp
        ${TUTORIALSEPERATESHADERS_DIR}/Shaders/Main.glsl.vert
        ${TUTORIALSEPERATESHADERS_DIR}/Shaders/SimpleFragmentShader.glsl.frag
        ${TUTORIALSEPERATESHADERS_DIR}/Shaders/TextureCoordinateAware.glsl.frag
        )



set(TutorialDirectStateAccess-Source
        ${TUTORIALDIRECTSTATEACCESS_DIR}/main.hpp
        ${TUTORIALDIRECTSTATEACCESS_DIR}/main.cpp
        ${TUTORIALDIRECTSTATEACCESS_DIR}/Shaders/Main.glsl.vert
        ${TUTORIALDIRECTSTATEACCESS_DIR}/Shaders/Main.glsl.frag
        )

add_executable(tutorial1 ${Tutorial1-Source})
target_link_libraries(tutorial1 ${GraphicsSamplesLinkerFlags})

add_executable(tutorial2 ${Tutorial2-Source})
target_link_libraries(tutorial2 ${GraphicsSamplesLinkerFlags})

add_executable(tutorial3 ${Tutorial3-Source})
target_link_libraries(tutorial3 ${GraphicsSamplesLinkerFlags})

add_executable(tutorial4 ${Tutorial4-Source})
target_link_libraries(tutorial4 ${GraphicsSamplesLinkerFlags})

add_executable(tutorial5 ${Tutorial5-Source})
target_link_libraries(tutorial5 ${GraphicsSamplesLinkerFlags})

add_executable(tutorial6 ${Tutorial6-Source})
target_link_libraries(tutorial6 ${GraphicsSamplesLinkerFlags})

add_executable(tutorial7 ${Tutorial7-Source})
target_link_libraries(tutorial7 ${GraphicsSamplesLinkerFlags})

add_executable(tutorial8 ${Tutorial8-Source})
target_link_libraries(tutorial8 ${GraphicsSamplesLinkerFlags})

add_executable(tutorial9 ${Tutorial9-Source})
target_link_libraries(tutorial9 ${GraphicsSamplesLinkerFlags})

add_executable(tutorial10 ${Tutorial10-Source})
target_link_libraries(tutorial10 ${GraphicsSamplesLinkerFlags})

add_executable(tutorial11 ${Tutorial11-Source})
target_link_libraries(tutorial11 ${GraphicsSamplesLinkerFlags})

add_executable(tutorial12 ${Tutorial12-Source})
target_link_libraries(tutorial12 ${GraphicsSamplesLinkerFlags})

add_executable(tutorial13 ${Tutorial13-Source})
target_link_libraries(tutorial13 ${GraphicsSamplesLinkerFlags})

add_executable(tutorial14 ${Tutorial14-Source})
target_link_libraries(tutorial14 ${GraphicsSamplesLinkerFlags} ktx)

add_executable(tutorial15 ${Tutorial15-Source})
target_link_libraries(tutorial15 ${GraphicsSamplesLinkerFlags} tinyobjloader)

add_executable(tutorial16 ${Tutorial16-Source})
target_link_libraries(tutorial16 ${GraphicsSamplesLinkerFlags} tinyobjloader)

add_executable(tutorial17 ${Tutorial17-Source})
target_link_libraries(tutorial17 ${GraphicsSamplesLinkerFlags} tinyobjloader)

add_executable(tutorial18 ${Tutorial18-Source})
target_link_libraries(tutorial18 ${GraphicsSamplesLinkerFlags})

add_executable(tutorial19 ${Tutorial19-Source})
target_link_libraries(tutorial19 ${GraphicsSamplesLinkerFlags})

add_executable(tutorial20 ${Tutorial20-Source})
target_link_libraries(tutorial20 ${GraphicsSamplesLinkerFlags})

add_executable(tutorial21 ${Tutorial21-Source})
target_link_libraries(tutorial21 ${GraphicsSamplesLinkerFlags} tinyobjloader)

add_executable(tutorial22 ${Tutorial20-Source})
target_link_libraries(tutorial22 ${GraphicsSamplesLinkerFlags})

add_executable(tutorial23 ${Tutorial23-Source})
target_link_libraries(tutorial23 ${GraphicsSamplesLinkerFlags})

add_executable(tutorial24 ${Tutorial24-Source})
target_link_libraries(tutorial24 ${GraphicsSamplesLinkerFlags})

add_executable(tutorial25 ${Tutorial25-Source})
target_link_libraries(tutorial25 ${GraphicsSamplesLinkerFlags})

add_executable(tutorial26 ${Tutorial26-Source})
target_link_libraries(tutorial26 ${GraphicsSamplesLinkerFlags})

add_executable(tutorial27 ${Tutorial27-Source})
target_link_libraries(tutorial27 ${GraphicsSamplesLinkerFlags})

add_executable(tutorial28 ${Tutorial28-Source})
target_link_libraries(tutorial28 ${GraphicsSamplesLinkerFlags})

add_executable(tutorial29 ${Tutorial29-Source})
target_link_libraries(tutorial29 ${GraphicsSamplesLinkerFlags})

add_executable(tutorial30 ${Tutorial30-Source})
target_link_libraries(tutorial30 ${GraphicsSamplesLinkerFlags})


#Extra examples that are not part of the book
if(SDL2_IMAGE_FOUND)
add_executable(tutorialSDL2Image ${TutorialSDL2Image-Source})
target_link_libraries(tutorialSDL2Image ${GraphicsSamplesLinkerFlags})
endif()

if(SDL2_IMAGE_FOUND)
add_executable(tutorialSeprateShaders ${TutorialSeprateShaders-Source})
target_link_libraries(tutorialSeprateShaders ${GraphicsSamplesLinkerFlags} tinyobjloader)
endif()

if(SDL2_IMAGE_FOUND)
add_executable(tutorialDirectStateAccess ${TutorialDirectStateAccess-Source})
target_link_libraries(tutorialDirectStateAccess ${GraphicsSamplesLinkerFlags} tinyobjloader)
endif()

#A simple rendering engine to try to show off some things I learned
#via the book.
#Allows for using the sample rendering engine
include_directories(${SOURCE_DIR})

set(Engine-Source
    ${ENGINE_DIR}/Camera/Camera.hpp
    ${ENGINE_DIR}/Camera/Camera.cpp
    ${ENGINE_DIR}/Model/Model.hpp
    ${ENGINE_DIR}/Model/Model.cpp
    ${ENGINE_DIR}/Model/Model.hpp
    ${ENGINE_DIR}/Model/Model.cpp
    ${ENGINE_DIR}/Scene/Scene.hpp
    ${ENGINE_DIR}/Scene/Scene.cpp
    ${ENGINE_DIR}/Render/Render.hpp
    ${ENGINE_DIR}/Render/Render.cpp
    ${ENGINE_DIR}/Engine.hpp
)

add_library(Engine ${Engine-Source})
target_link_libraries(Engine ${GraphicsSamplesLinkerFlags} tinyobjloader)

#Sample Projects using the render engine.
set(SimpleScene-Source
    ${SIMPLESCENE_DIR}/main.hpp
    ${SIMPLESCENE_DIR}/main.cpp
)

add_executable(SimpleScene ${SimpleScene-Source})
target_link_libraries(SimpleScene ${GraphicsSamplesLinkerFlags} Engine tinyobjloader)
