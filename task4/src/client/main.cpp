#include "Client.h"
#include "../../../config.h"

int main() {
    auto address = boost::asio::ip::make_address(SERVER_IP);
    boost::asio::ip::tcp::endpoint endpoint(address, SERVER_PORT);
    Client client;
    client.start_session_with(endpoint);
}
