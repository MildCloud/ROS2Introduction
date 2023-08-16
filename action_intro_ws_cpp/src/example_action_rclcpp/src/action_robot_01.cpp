#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "example_action_rclcpp/robot.h"


class ActionRobot01 : public rclcpp::Node {
    public:
        using MoveRobot = robot_control_interface::action::MoveRobot;
        using GoalHandleMoveRobot = rclcpp_action::ServerGoalHandle<MoveRobot>;
        explicit ActionRobot01(std::string name);
    private:
        Robot robot;
        rclcpp_action::Server<MoveRobot>::SharedPtr action_server_;
        rclcpp_action::GoalResponse handle_goal(const rclcpp_action::GoalUUID& uuid, std::shared_ptr<const MoveRobot::Goal> goal);
        rclcpp_action::CancelResponse handle_cancel(const std::shared_ptr<GoalHandleMoveRobot> goal_handle);
        void execute_move(const std::shared_ptr<GoalHandleMoveRobot> goal_handle);
        void handle_accepted(const std::shared_ptr<GoalHandleMoveRobot> goal_handle);
};

ActionRobot01::ActionRobot01(std::string name) : Node(name) {
    using namespace std::placeholders;
    RCLCPP_INFO(this->get_logger(), "%s node starts", name.c_str());
    this->action_server_ = rclcpp_action::create_server<MoveRobot>(
        this, "move_robot", std::bind(&ActionRobot01::handle_goal, this, _1, _2), std::bind(&ActionRobot01::handle_cancel, this, _1), std::bind(&ActionRobot01::handle_accepted, this, _1)
    );
}

rclcpp_action::GoalResponse ActionRobot01::handle_goal(const rclcpp_action::GoalUUID& uuid, std::shared_ptr<const MoveRobot::Goal> goal) {
    RCLCPP_INFO(this->get_logger(), "Received goal request with distance %f", goal->distance);
    (void)uuid;
    if (fabs(goal->distance > 100)) {
        RCLCPP_WARN(this->get_logger(), "Too far from the goal, reject");
        return rclcpp_action::GoalResponse::REJECT;
    }
    RCLCPP_INFO(this->get_logger(), "Distance is practical");
    return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
}

rclcpp_action::CancelResponse ActionRobot01::handle_cancel(const std::shared_ptr<GoalHandleMoveRobot> goal_handle) {
    RCLCPP_INFO(this->get_logger(), "Received request to cancel goal");
    (void)goal_handle;
    robot.stop_move();
    return rclcpp_action::CancelResponse::ACCEPT;
}

void ActionRobot01::execute_move(const std::shared_ptr<GoalHandleMoveRobot> goal_handle) {
    const auto goal = goal_handle->get_goal();
    RCLCPP_INFO(this->get_logger(), "start to execute moving %f", goal->distance);
    auto result = std::make_shared<MoveRobot::Result>();
    rclcpp::Rate rate = rclcpp::Rate(2);
    robot.set_goal(goal->distance);
    while (rclcpp::ok() && !robot.close_goal()) {
        robot.move_step();
        auto feedback = std::make_shared<MoveRobot::Feedback>();
        feedback->pose = robot.get_current_pose();
        feedback->status = robot.get_status();
        goal_handle->publish_feedback(feedback);
        if (goal_handle->is_canceling()) {
            result->pose = robot.get_current_pose();
            goal_handle->canceled(result);
            RCLCPP_INFO(this->get_logger(), "goal canceled");
            return;
        }
        RCLCPP_INFO(this->get_logger(), "publish feedback");
        rate.sleep();
    }
    result->pose = robot.get_current_pose();
    goal_handle->succeed(result);
    RCLCPP_INFO(this->get_logger(), "goal succeed");
}

void ActionRobot01::handle_accepted(const std::shared_ptr<GoalHandleMoveRobot> goal_handle) {
    // Handle the execution
    std::thread{std::bind(&ActionRobot01::execute_move, this, std::placeholders::_1), goal_handle}.detach();
}


int main(int argc, char** argv) {
    rclcpp::init(argc, argv);
    auto action_server = std::make_shared<ActionRobot01>("action_robot_01");
    rclcpp::spin(action_server);
    rclcpp::shutdown();
    return 0;
}
