#ifndef GAME_CONTROL_CLIENT_H
#define GAME_CONTROL_CLIENT_H

#include "rclcpp/rclcpp.hpp"
#include "msg/GameControlReturn.msg"

class GameControlClient : public rclcpp::Node {
public:
    GameControlClient();
    void sendData();

private:
    rclcpp::Publisher<GameControlReturn>::SharedPtr publisher_;
};

#endif // GAME_CONTROL_CLIENT_H
