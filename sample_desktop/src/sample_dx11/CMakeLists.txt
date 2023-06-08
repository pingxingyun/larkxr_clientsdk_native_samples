# linking
set(LINK_DIR 
    # LINK_DIR
    ${PROJECT_SOURCE_DIR}/larkxr/lib/x64/
)

link_directories(${LINK_DIR})

# source dir
set(src_dir ${CMAKE_CURRENT_SOURCE_DIR})

# 输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    
add_executable(test_xr_dx11 
    WIN32
    Resource.h
    main.cpp
    application.h
    application.cpp
)

# include
target_include_directories(test_xr_dx11 PRIVATE
    ${src_dir}
    ${PROJECT_SOURCE_DIR}/larkxr/include/
)

target_link_libraries(test_xr_dx11 
    # cloudlark_sr
    # xr
    lark_xr
    # dx
    d3d11
    d3dcompiler
    dxguid
    winmm
    comctl32
    # helper
    Iphlpapi.lib
)