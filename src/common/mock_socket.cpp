#include "mock_socket.h"

MockSocket::MockSocket(std::shared_ptr<std::vector<uint8_t>> input_buffer,
                       std::shared_ptr<std::vector<uint8_t>> output_buffer):
        input_buffer(std::move(input_buffer)),
        output_buffer(std::move(output_buffer)),
        val(false) {}

/* Constructores no utilizados */

MockSocket::MockSocket(const char* hostname, const char* servname):
        input_buffer(std::make_shared<std::vector<uint8_t>>()),
        output_buffer(std::make_shared<std::vector<uint8_t>>()),
        val(false) {
    (void)hostname;
    (void)servname;
}

MockSocket::MockSocket(const char* servname):
        input_buffer(std::make_shared<std::vector<uint8_t>>()),
        output_buffer(std::make_shared<std::vector<uint8_t>>()),
        val(false) {
    (void)servname;
}

/* --------- */

int MockSocket::sendall(const void* data, unsigned int size) {
    const uint8_t* bytes = static_cast<const uint8_t*>(data);
    output_buffer->insert(output_buffer->end(), bytes, bytes + size);
    return size;
}

int MockSocket::recvall(void* data, unsigned int size) {

    unsigned int to_read = std::min(size, static_cast<unsigned int>(input_buffer->size()));
    std::memcpy(data, input_buffer->data(), to_read);
    input_buffer->erase(input_buffer->begin(), input_buffer->begin() + to_read);
    return to_read;
}

void MockSocket::shutdown(int how) {
    (void)how;
    val = true;
}

int MockSocket::close() {
    val = true;
    return 0;
}

bool MockSocket::is_stream_recv_closed() { return val; }

bool MockSocket::is_stream_send_closed() { return val; }
