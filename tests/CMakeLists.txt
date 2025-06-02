target_sources(taller_tests
    PRIVATE
    # .cpp files
    ../src/common/socket.cpp
    ../src/client/client_protocol.cpp
    ../src/client/client_protocol_parser.cpp
    ../src/server/server_protocol.cpp
    lobby_tests.cpp
    PUBLIC
    ../src/common/socket.h
    ../src/client/client_protocol.h
    ../src/client/client_protocol_parser.h
    ../src/server/server_protocol.h
    # .h files
    )
