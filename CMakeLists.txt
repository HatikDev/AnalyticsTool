cmake_minimum_required(VERSION 3.5)

project(AnaliticsTools VERSION 1.0 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_FLAGS "-g /EHsc")

set(CMAKE_PREFIX_PATH "C:/Qt/6.4.2/msvc2019_64")

set(Qt6_DIR "C:/Qt/6.4.2/msvc2019_64/lib/cmake/Qt6")

set(QT_VERSION_MAJOR 6)

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/utils)

set(PROJECT_SOURCES
    gui/rectangle.cpp
    gui/rectangle.h
    gui/mainwindow.cpp
    gui/mainwindow.h
    gui/mainwindow.ui
    gui/paintscene.h
    gui/paintscene.cpp

    controller/controller.h
    controller/controller.cpp

    model/model.cpp
    model/model.h
    model/picture.h
    model/picture.cpp
    model/dataset.h
    model/dataset.cpp

    utils/utils.cpp
    utils/analyticsexception.cpp
    utils/utils.h

    include/constants.h
    include/analyticsexception.h

    main.cpp
)

qt_add_executable(${PROJECT_NAME}
	MANUAL_FINALIZATION
	${PROJECT_SOURCES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

target_link_libraries(${PROJECT_NAME} PUBLIC ModelLauncher)

set_target_properties(${PROJECT_NAME} PROPERTIES
    WIN32_EXECUTABLE TRUE
)

install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

qt_finalize_executable(${PROJECT_NAME})
