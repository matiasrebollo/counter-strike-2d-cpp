target_sources(taller_editor
    PRIVATE
    game_editor.ui
    game_editor.cpp
    main.cpp
    resources.qrc
    blocks_setter.cpp
    ct_spawns_setter.cpp
    PUBLIC
    # .h files
    game_editor.h
    clickablelabel.h
    grid_action.h
    blocks_setter.h
    ct_spawns_setter.h
    )
