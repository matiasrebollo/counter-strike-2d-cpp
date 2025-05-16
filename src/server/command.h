#ifndef COMMAND_H
#define COMMAND_H

class CS2DGame;  // Forward declaration

class Command {

    // crear subclases y que ejecutar le de la responsabilidad al juego de ejecutar el comando como
    // corresponda, segun el tipo.

public:
    Command();

    virtual void execute(CS2DGame& game) const = 0;

    Command(const Command&) = delete;
    Command& operator=(const Command&) = delete;

    virtual ~Command();
};

#endif
