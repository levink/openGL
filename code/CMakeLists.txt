cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 11)

project(OpenGL)

include_directories(include)

link_directories(lib/glut)

#Graphics
add_executable(Graphics 
    "src/Graphics/main.cpp")
target_link_libraries(Graphics glut32.lib)
add_custom_command(TARGET Graphics POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${PROJECT_SOURCE_DIR}/lib/glut/glut32.dll"      
    $<TARGET_FILE_DIR:Graphics>)
add_custom_command(TARGET Graphics POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/src/Graphics/pics"
        $<TARGET_FILE_DIR:Graphics>/pics)

#Tonya
add_executable(Tonya 
    "src/Tonya/main.cpp"
    "src/Tonya/bmp.cpp"
    "src/Tonya/shadows.cpp")
target_link_libraries(Tonya glut32.lib)
add_custom_command(TARGET Tonya POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${PROJECT_SOURCE_DIR}/lib/glut/glut32.dll"      
    $<TARGET_FILE_DIR:Tonya>)
add_custom_command(TARGET Tonya POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/src/Tonya/img"
        $<TARGET_FILE_DIR:Tonya>/img)

#Sasha
add_executable(Sasha 
    "src/Sasha/main.cpp"
    "src/Sasha/bmp.cpp")
target_link_libraries(Sasha glut32.lib)
add_custom_command(TARGET Sasha POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${PROJECT_SOURCE_DIR}/lib/glut/glut32.dll"      
    $<TARGET_FILE_DIR:Sasha>)
add_custom_command(TARGET Sasha POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/src/Sasha/img"
        $<TARGET_FILE_DIR:Sasha>/img)

#Fedor
add_executable(Fedor 
    "src/Fedor/main.cpp"
    "src/Fedor/bmp.cpp"
    "src/Fedor/objects.cpp")
target_link_libraries(Fedor glut32.lib)
add_custom_command(TARGET Fedor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${PROJECT_SOURCE_DIR}/lib/glut/glut32.dll"      
    $<TARGET_FILE_DIR:Fedor>)
add_custom_command(TARGET Fedor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/src/Fedor/img"
        $<TARGET_FILE_DIR:Fedor>/img)
