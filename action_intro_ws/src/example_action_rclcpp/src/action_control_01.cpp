#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "robot_control_interface/action/move_robot.hpp"


class ActionControl01 : public rclcpp::Node {
    public:
        using MoveRobot = robot_control_interface::action::MoveRobot;
        using GoalHandlerMoveRobot = rclcpp_action::ClientGoalHandle<MoveRobot>;
        explicit ActionControl01(std::string name);
};

explicit ActionControl01::ActionControl01(std::string name): Node(name) {
    RCLCPP_INFO(this->get_logger(), "%s node start", name.c_str());
}


int main(int argc, char** argv) {
    rclcpp::init(argc, argv);
    auto action_client = std::make_shared<ActionControl01>("action_robot_cpp");
    rclcpp::spin(action_client);
    rclcpp::shutdown();
    return 0;
}
