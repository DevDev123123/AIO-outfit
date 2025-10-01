cmake_minimum_required(VERSION 3.16)
project(OutfitConverterPro VERSION 3.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Find Qt6 or Qt5
find_package(Qt6 COMPONENTS Core Widgets QUIET)
if (NOT Qt6_FOUND)
    find_package(Qt5 5.15 REQUIRED COMPONENTS Core Widgets)
endif()

# Source files
set(SOURCES
    main.cpp
)

# Create executable
if(WIN32)
    add_executable(${PROJECT_NAME} WIN32 ${SOURCES})
elseif(APPLE)
    add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${SOURCES})
else()
    add_executable(${PROJECT_NAME} ${SOURCES})
endif()

# Link Qt libraries
if (Qt6_FOUND)
    target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core Qt6::Widgets)
else()
    target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Core Qt5::Widgets)
endif()

# Set output directory
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Install target
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    BUNDLE DESTINATION .
)

# Platform-specific settings
if(WIN32)
    # Windows-specific settings
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
    
    # Deploy Qt DLLs on Windows
    if(Qt6_FOUND)
        find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${Qt6_DIR}/../../../bin")
    else()
        find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${Qt5_DIR}/../../../bin")
    endif()
    
    if(WINDEPLOYQT_EXECUTABLE)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${WINDEPLOYQT_EXECUTABLE} $<TARGET_FILE:${PROJECT_NAME}>
            COMMENT "Running windeployqt..."
        )
    endif()
    
elseif(APPLE)
    # macOS-specific settings
    set_target_properties(${PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_BUNDLE_NAME "Outfit Converter Pro"
        MACOSX_BUNDLE_BUNDLE_VERSION "3.0"
        MACOSX_BUNDLE_SHORT_VERSION_STRING "3.0"
        MACOSX_BUNDLE_GUI_IDENTIFIER "com.sizrox.outfitconverter"
    )
    
    # Deploy Qt frameworks on macOS
    if(Qt6_FOUND)
        find_program(MACDEPLOYQT_EXECUTABLE macdeployqt HINTS "${Qt6_DIR}/../../../bin")
    else()
        find_program(MACDEPLOYQT_EXECUTABLE macdeployqt HINTS "${Qt5_DIR}/../../../bin")
    endif()
    
    if(MACDEPLOYQT_EXECUTABLE)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${MACDEPLOYQT_EXECUTABLE} $<TARGET_BUNDLE_DIR:${PROJECT_NAME}>
            COMMENT "Running macdeployqt..."
        )
    endif()
    
elseif(UNIX)
    # Linux-specific settings
    install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/outfit-converter.desktop"
        DESTINATION share/applications
        OPTIONAL
    )
endif()
