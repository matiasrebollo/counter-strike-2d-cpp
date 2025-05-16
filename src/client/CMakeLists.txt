target_sources(taller_client
    PUBLIC
        lobby/lobby.h
        client_protocol_parser.h
        client_protocol.h
        client_receiver.h
        client_sender.h
    PRIVATE
        main.cpp
        lobby/lobby.cpp
        lobby/lobby.ui
        lobby/resources.qrc
        client_protocol_parser.cpp
        client_protocol.cpp
        client_receiver.cpp
        client_sender.cpp
)


