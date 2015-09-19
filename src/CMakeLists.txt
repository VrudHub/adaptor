set(exe_name "adaptor")

set(sources
    colours.cpp
    colours.hpp
    main.cpp
    screen.cpp
    screen.hpp
    transforms.cpp
    transforms.hpp
    utils.cpp
    utils.hpp
    webcam.cpp
    webcam.hpp
    )

find_package(OpenCV)

set(inc_dirs
    ${inc_dirs}
    )
set(link_dirs
    ${link_dirs}
    )
set(libs
    ${libs}
    ${OpenCV_LIBS}
    )

include_directories(${inc_dirs})
link_directories(${link_dirs})
add_executable(${exe_name} ${sources})
target_link_libraries(${exe_name} ${libs})
