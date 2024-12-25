#include <iostream>
#include <memory>
#include <thread>
#include "LoadBalancer.h"
#include "Server.h"
#include "ThreadPool.h"

int main() {
    LoadBalancer lb(LoadBalancingAlgorithm::LEAST_CONNECTIONS);
    
    auto server1 = std::make_shared<Server>("192.168.1.1", 8080);
    auto server2 = std::make_shared<Server>("192.168.1.2", 8080);
    auto server3 = std::make_shared<Server>("192.168.1.3", 8080);

    lb.addServer(server1);
    lb.addServer(server2);
    lb.addServer(server3);

    ThreadPool pool(4);
    
    std::vector<std::string> clientIps = { "10.0.0.1", "10.0.0.2", "10.0.0.3", "10.0.0.4" };

    for (const auto &clientIp : clientIps) {
        pool.enqueue([&lb, clientIp]() {
            lb.handleRequest(clientIp);
        });
    }

    std::this_thread::sleep_for(std::chrono::seconds(5));  // Wait for all requests to be processed

    // Periodic health check
    std::cout << "Starting health check..." << std::endl;
    lb.healthCheck();

    return 0;
}















