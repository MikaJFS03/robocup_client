#include <iostream>
#include <cstring>
#include <thread>
#include <chrono>
#include <arpa/inet.h>
#include <unistd.h>

const int GAME_CONTROLLER_LISTEN_PORT = 3838;
const int GAME_CONTROLLER_ANSWER_PORT = 3939;

class GameStateReceiver {
public:
    GameStateReceiver(int team, int player) : team(team), player(player), running(true) {
        openSocket();
    }

    void receiveForever() {
        while (running) {
            receiveOnce();
        }
    }

    void stop() {
        running = false;
    }

private:
    int team;
    int player;
    bool running;
    int sock;

    void openSocket() {
        sock = socket(AF_INET, SOCK_DGRAM, 0);
        if (sock < 0) {
            std::cerr << "Socket creation failed" << std::endl;
            exit(EXIT_FAILURE);
        }

        sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons(GAME_CONTROLLER_LISTEN_PORT);

        if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
            std::cerr << "Bind failed" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    void receiveOnce() {
        char buffer[1024] = {0};
        sockaddr_in peer;
        socklen_t peer_len = sizeof(peer);
        int bytes_received = recvfrom(sock, buffer, sizeof(buffer) - 1, 0, (struct sockaddr*)&peer, &peer_len);
        if (bytes_received > 0) {
            buffer[bytes_received] = '\0'; // Null-terminate the string
            std::cout << "Received: " << buffer << std::endl;
            answerToGameController(peer);
        }
    }

    void answerToGameController(sockaddr_in peer) {
        char response[256];
        snprintf(response, sizeof(response), "Response from team %d, player %d", team, player);
        sendto(sock, response, strlen(response), 0, (struct sockaddr*)&peer, sizeof(peer));
    }
};

int main(int argc, char* argv[]) {
    int team = 1; // Default team ID
    int player = 1; // Default player ID

    GameStateReceiver receiver(team, player);
    std::thread receiverThread(&GameStateReceiver::receiveForever, &receiver);

    // Simulate running for a while
    std::this_thread::sleep_for(std::chrono::seconds(30));

    receiver.stop();
    receiverThread.join();

    return 0;
}