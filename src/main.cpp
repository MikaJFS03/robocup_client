#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>

const char* HOST = "127.0.0.1";  // The server's hostname or IP address
const int PORT = 8750;            // The port used by the server

int main() {
    int sock;
    struct sockaddr_in server_addr;

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        std::cerr << "Could not create socket" << std::endl;
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, HOST, &server_addr.sin_addr);

    // Connect to server
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Connection failed" << std::endl;
        return 1;
    }

    auto start = std::chrono::high_resolution_clock::now();
    bool ready = false, set = false, play = false, penalized = false, scored = false, kickoff = false;
    int message_numb = 0;

    while (true) {
        auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::high_resolution_clock::now() - start).count();

        std::string clock_data = std::to_string(message_numb) + ":CLOCK:" + std::to_string(static_cast<int>(delta * 3)) + "\n";
        std::cout << clock_data;

        send(sock, clock_data.c_str(), clock_data.length(), 0);

        if (delta > 12000 && !kickoff) {
            kickoff = true;
            send(sock, (std::to_string(message_numb) + ":SIDE_LEFT:26\n").c_str(), 0, 0);
        }

        if (delta > 3000 && !ready) {
            ready = true;
            send(sock, (std::to_string(message_numb) + ":STATE:READY\n").c_str(), 0, 0);
        }

        if (delta > 7000 && !set) {
            set = true;
            send(sock, (std::to_string(message_numb) + ":STATE:SET\n").c_str(), 0, 0);
        }

        if (delta > 12000 && !play) {
            play = true;
            send(sock, (std::to_string(message_numb) + ":STATE:PLAY\n").c_str(), 0, 0);
        }

        if (delta > 15000 && play && !penalized) {
            penalized = true;
            send(sock, (std::to_string(message_numb) + ":PENALTY:27:1:BALL_MANIPULATION\n").c_str(), 0, 0);
        }

        if (delta > 17000 && play && !scored) {
            scored = true;
            send(sock, (std::to_string(message_numb) + ":SCORE:27\n").c_str(), 0, 0);
        }

        char buffer[1024] = {0};
        int bytes_received = recv(sock, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received > 0) {
            buffer[bytes_received] = '\0'; // Null-terminate the string
            std::cout << buffer << std::endl;
        }

        message_numb++;
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    close(sock);
    return 0;
}