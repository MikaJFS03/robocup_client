#include "rclcpp/rclcpp.hpp"
#include "game_control_client.h"

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    auto client = std::make_shared<GameControlClient>();
    client->sendData();
    rclcpp::spin(client);
    rclcpp::shutdown();
    return 0;
}