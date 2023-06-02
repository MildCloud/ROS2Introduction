#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "robot_control_interface/action/move_robot.hpp"


class ActionControl01 : public rclcpp::Node {
    public:
        using MoveRobot = robot_control_interface::action::MoveRobot;
        using GoalHandlerMoveRobot = rclcpp_action::ClientGoalHandle<MoveRobot>;
        explicit ActionControl01(std::string name);
        void send_goal();
    private:
        rclcpp_action::Client<MoveRobot>::SharedPtr client_ptr_;
        rclcpp::TimerBase::SharedPtr timer_;
        void goal_response_callback(GoalHandlerMoveRobot::SharedPtr goal_handle);
        void feedback_callback(GoalHandlerMoveRobot::SharedPtr, const std::shared_ptr<const MoveRobot::Feedback> feedback);
        void result_callback(const GoalHandlerMoveRobot::WrappedResult& result);
};

ActionControl01::ActionControl01(std::string name): Node(name) {
    RCLCPP_INFO(this->get_logger(), "%s node start", name.c_str());
    this->client_ptr_ = rclcpp_action::create_client<MoveRobot>(this, "move_robot");
    this->timer_ = this->create_wall_timer(std::chrono::milliseconds(500), std::bind(&ActionControl01::send_goal, this));
}

void ActionControl01::send_goal() {
    using namespace std::placeholders;
    this->timer_->cancel();
    if (!this->client_ptr_->wait_for_action_server(std::chrono::seconds(10))) {
        RCLCPP_ERROR(this->get_logger(), "Action server not available after waiting");
        rclcpp::shutdown();
        return;
    }
    auto goal_msg = MoveRobot::Goal();
    goal_msg.distance = 10;
    RCLCPP_INFO(this->get_logger(), "Sending goal");
    auto send_goal_options = rclcpp_action::Client<MoveRobot>::SendGoalOptions();
    send_goal_options.goal_response_callback = std::bind(&ActionControl01::goal_response_callback, this, _1);
    send_goal_options.feedback_callback = std::bind(&ActionControl01::feedback_callback, this, _1, _2);
    send_goal_options.result_callback = std::bind(&ActionControl01::result_callback, this, _1);
    this->client_ptr_->async_send_goal(goal_msg, send_goal_options);
}

void ActionControl01::goal_response_callback(GoalHandlerMoveRobot::SharedPtr goal_handle) {
    // Called first after log "Sending goal"
    if (!goal_handle) {
        RCLCPP_ERROR(this->get_logger(), "Goal was rejected by server");
    }
    else {
        RCLCPP_INFO(this->get_logger(), "Goal accepted by server, waiting for result");
    }
}

void ActionControl01::feedback_callback(GoalHandlerMoveRobot::SharedPtr, const std::shared_ptr<const MoveRobot::Feedback> feedback) {
    // Called seconed and continuously every 500ms?
    // FIXME change 500 to 10000 will not change performace
    RCLCPP_INFO(this->get_logger(), "Feedback current pose: %f", feedback->pose);
}

void ActionControl01::result_callback(const GoalHandlerMoveRobot::WrappedResult& result) {
    // Called last
    switch (result.code) {
      case rclcpp_action::ResultCode::SUCCEEDED:
        break;
      case rclcpp_action::ResultCode::ABORTED:
        RCLCPP_ERROR(this->get_logger(), "Goal was aborted");
        return;
      case rclcpp_action::ResultCode::CANCELED:
        RCLCPP_ERROR(this->get_logger(), "Goal was canceled");
        return;
      default:
        RCLCPP_ERROR(this->get_logger(), "Unknown result code");
        return;
    }
    RCLCPP_INFO(this->get_logger(), "Result received: %f", result.result->pose);
}


int main(int argc, char** argv) {
    rclcpp::init(argc, argv);
    auto action_client = std::make_shared<ActionControl01>("action_robot_cpp");
    rclcpp::spin(action_client);
    rclcpp::shutdown();
    return 0;
}
