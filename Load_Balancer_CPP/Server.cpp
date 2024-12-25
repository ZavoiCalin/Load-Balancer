#include "Server.h"
#include <iostream>
#include <thread>
#include <chrono>


Server::Server(std::string ip, int port): ip(ip), port(port), alive(true), numConnections(0) {
    lastCheckedTime = std::chrono::steady_clock::now();
}

std::string Server::getIp() const {
    return ip;
}

int Server::getPort() const {
    return port;
}

bool Server::isAlive() const {
    return alive.load();
}

void Server::setAlive(bool status) {
    alive.store(status);
}

int Server::getNumConnections() const {
    return numConnections.load();
}

void Server::incrementConnections(int value) {
    numConnections += value;
}

void Server::decrementConnections(int value) {
    numConnections -= value;
}

std::chrono::steady_clock::time_point Server::lastChecked() const {
    return lastCheckTime;
}

void Server::updateLastChecked() {
    lastCheckTime = std::chrono::steady_clock::now();
}

bool Sever::ping() {
    // Simulate a ping by randomly marking a server as dead or alive so modifying the atomic value of the alive boolean
    bool result = (rand() % 2) == 0;

    setAlive(result);

    return result;
}



















