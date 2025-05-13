# Reglas de Qt
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Buscar Qt
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# Ejecutable y fuentes
add_executable(taller_client
    main.cpp
    lobby/lobby.cpp
    lobby/lobby.ui
)

target_sources(taller_client
    PUBLIC
        lobby/lobby.h
)

target_include_directories(taller_client
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/lobby
    PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR}
        ${libSDL2pp_SOURCE_DIR}
)

target_link_libraries(taller_client
    PRIVATE
        Qt${QT_VERSION_MAJOR}::Widgets
        taller_common
        SDL2pp::SDL2pp
)

