#include "LoadBalancer.h"
#include <iostream>
#include <algorithm>
#include <functional>

LoadBalancer::LoadBalancer(LoadBalancingAlgorithm algo) : algorithm(algo), currentIndex(0) {}


void LoadBalancer::addServer(std::shared_ptr<Server> server) {
    std::lock_guard<std::mutex> lock(lbMutex);
    servers.push_back(server);
}


std::shared_ptr<Server> LoadBalancer::getServer() {
    std::lock_guard<std::mutex> lock(lbMutex);

    switch(algorithm) {
        case LoadBalancingAlgorithm::ROUND_ROBIN:
            return getRoundRobinServer();
        case LoadBalancingAlgorithm::LEAST_CONNECTIONS:
            return getLeastConnectionsServer();
        case LoadBalancingAlgorithm::IP_HASH:
            return nullptr;
    }

    return nullptr;
}

std::shared_ptr<Server> LoadBalancer::getRoundRobinServer() {
    if (servers.empty())
        return nullptr;

    std::shared_ptr<Server> server = servers[currentIndex];
    currentIndex = (currentIndex + 1) % servers.size();

    return server;
}


std::shared_ptr<Server> LoadBalancer::getLeastConnectionsServer() {
    if (servers.empty()) 
        return nullptr;

    return *std::min_element(servers.begin(), servers.end(), [](const auto &a, const auto &b) {
        return a -> getNumConnections() < b -> getNumConnections();
    });
}


std::shared_ptr<Server> LoadBalancer::getIPHashServer(const std::string &clientIp) {
    if (servers.empty())
        return nullptr;

    size_t hashValue = std::hash<std::string>{}(clientIp);

    return servers[hashValue % servers.size()];
}


void LoadBalancer::handleRequest(const std::string &clientIp) {
    std::shared_ptr<Server> server;

    if(algorithm == LoadBalancingAlgorithm::IP_HASH) {
        server = getIPHashServer(clientIp);
    }
    else {
        server = getServer();
    }

    if(server != nullptr && server -> isAlive()) {
        server -> incrementConnections();
        std::cout << "Forwarding request from " << clientIp << " to " << server -> getIp() << ":" << server -> getPort() << std::endl;

        std::this_thread::sleep_for(std::chrono::seconds(1)); // Simulate processing time

        server -> decrementConnections(); 
    }
    else {
        std::cerr << "No aliver servers for client: " << clientIp << std::endl;
    }
    
}


void LoadBalancer::healthCheck() {
    for(auto &server : servers) {
        std::cout << "Pinging server: " << server -> getIp() << std::endl;

        server -> ping();

        std::cout << "Server status: " << (server -> isAlive() ? "Alive" : "Dead") << std::endl;

    }
}













