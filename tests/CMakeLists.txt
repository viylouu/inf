add_executable(inf_init init.c)

target_link_libraries(inf_init
    PRIVATE
        inf
)

#target_include_directories(inf_init
#    PRIVATE
#        ${PROJECT_SOURCE_DIR}/include
#)
