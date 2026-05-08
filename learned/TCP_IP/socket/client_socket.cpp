#include "client_socket.h"

using namespace akai::socket;

ClientSocket::ClientSocket(const string& ip, int port) :Socket() {
	connect(ip, port);
}