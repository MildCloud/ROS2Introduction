#include "example_action_rclcpp/robot.h"

float Robot::move_step() {
    int direct = move_distance_ / fabs(move_distance_);
    float step = direct * fabs(target_pose_ - current_pose_) * 0.1;
    current_pose_ += step;
    std::cout << "move: " << step << " with current position " << current_pose_ << std::endl;
    return current_pose_;
}

bool Robot::set_goal(float distance) {
    move_distance_ = distance;
    target_pose_ += move_distance_;
    if (close_goal()) {
        status_ = MoveRobot::Feedback::STATUS_STOP;
        return false;
    }
    status_ = MoveRobot::Feedback::STATUS_MOVEING;
    return false;
}

float Robot::get_current_pose() {
    return current_pose_;
}

float Robot::get_status() {
    return status_;
}

bool Robot::close_goal() {
    return fabs(target_pose_ - current_pose_) < 0.01;
}

void Robot::stop_move() {
    status_ = MoveRobot::Feedback::STATUS_STOP;
}
