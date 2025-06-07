target_sources(taller_tests
    PRIVATE
    # .cpp files
    common_tests.cpp
    ../src/common/socket.cpp
    ../src/client/client_protocol.cpp
    ../src/server/server_protocol.cpp
    lobby_tests.cpp
    game_tests.cpp
    PUBLIC
    common_tests.h
    ../src/common/socket.h
    ../src/client/client_protocol.h
    ../src/server/server_protocol.h
    # .h files
    )
