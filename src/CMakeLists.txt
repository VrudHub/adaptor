set(exe_name "adaptor")

set(sources
    colours.cpp
    main.cpp
    screen.cpp
    transforms.cpp
    webcam.cpp
    )

set(inc_dirs
    ${inc_dirs}
    )
set(link_dirs
    ${link_dirs}
    )

include_directories(${inc_dirs})
link_directories(${link_dirs})
add_executable(${exe_name} ${sources})
target_link_libraries(${exe_name} ${libs})
