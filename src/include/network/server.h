#pragma once
#include "network/protocol.h"
#include <thread>
#include <atomic>

namespace mokshith {

class Server {
public:
    Server(int port, Database* database);
    ~Server();
    
    void Start();
    void Stop();
    
private:
    int port_;
    Database* database_;
    int listen_fd_;
    std::atomic<bool> running_;
    std::thread* accept_thread_;
    std::vector<std::thread*> worker_threads_;
    
    void AcceptConnections();
    void HandleClient(int client_fd);
    Message ProcessMessage(const Message& request, Session* session);
};

} // namespace mokshith