#include <exception>
#include <string>

#define EXCEPTION_MESSAGE "The communication has ended"

class CommunicationEnded: public std::exception {
private:
    std::string message;

public:
    CommunicationEnded(): message(EXCEPTION_MESSAGE) {}

    const char* what() const noexcept override { return message.c_str(); }
};
