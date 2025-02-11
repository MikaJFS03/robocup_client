#include "game_control_client.h"
#include "rclcpp/rclcpp.hpp"

GameControlClient::GameControlClient() : Node("game_control_client") {
    // Initialize publisher
    publisher_ = this->create_publisher<GameControlReturn>("game_control_return", 10);
}

void GameControlClient::sendData() {
    auto message = GameControlReturn();
    // Fill in the message fields
    message.playerNum = 1; // Example player number
    message.teamNum = 1;   // Example team number
    message.fallen = 0;    // Robot is not fallen
    message.pose[0] = 0.0;  // x position
    message.pose[1] = 0.0;  // y position
    message.pose[2] = 0.0;  // theta (orientation)
    message.ballAge = 0.0;  // Time since last seen ball
    message.ball[0] = 0.0;   // Ball x position
    message.ball[1] = 0.0;   // Ball y position

    // Publish the message
    publisher_->publish(message);
}