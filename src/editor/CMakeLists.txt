target_sources(taller_editor
    PRIVATE
    game_editor.ui
    game_editor.cpp
    clickable_label.cpp
    main.cpp
    resources.qrc
    pixmap_manager.cpp
    logic_map.cpp
    PUBLIC
    # .h files
    game_editor.h
    clickable_label.h
    pixmap_manager.h
    logic_map.h
    )

add_subdirectory(grid_action)
