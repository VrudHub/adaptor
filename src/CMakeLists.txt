set(exe_name "adaptor")

set(sources
    colours.cpp
    colours.hpp
    main.cpp
    project.cpp
    project.hpp
    screen.cpp
    screen.hpp
    screen-windows.cpp
    screen-windows.hpp
    transforms.cpp
    transforms.hpp
    utils.cpp
    utils.hpp
    webcam.cpp
    webcam.hpp
    )

find_package(OpenCV)
find_package(SDL2)
find_package(SDL2_image)

set(inc_dirs
    ${inc_dirs}
${SDL2_INCLUDEDIR}
${SDL2IMAGE_INCLUDE_DIR}
    )
set(link_dirs
${link_dirs}
    )
set(libs
    ${libs}
    ${OpenCV_LIBS}
    ${SDL2_LIBRARY}
    ${SDL2IMAGE_LIBRARY}
    )

include_directories(${inc_dirs})
link_directories(${link_dirs})
add_executable(${exe_name} ${sources})
target_link_libraries(${exe_name} ${libs})
