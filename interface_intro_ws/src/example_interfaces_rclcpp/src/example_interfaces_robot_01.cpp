#include "rclcpp/rclcpp.hpp"
#include "example_ros2_interfaces/msg/robot_status.hpp"
#include "example_ros2_interfaces/srv/move_robot.hpp"


class Robot
{
private:
  float current_pose_ = 0.0;
  float target_pose_ = 0.0;
  int status_ = example_ros2_interfaces::msg::RobotStatus::STATUS_STOP;
public:
  Robot(/* args */) = default;
  ~Robot() = default;
  float get_current_pose();
  int get_status();
  float move_distance(float distance);
};

float Robot::get_current_pose()
{
  return current_pose_;
}

int Robot::get_status()
{
  return status_;
}

float Robot::move_distance(float distance)
{
  status_ = example_ros2_interfaces::msg::RobotStatus::STATUS_MOVEING;
  target_pose_ += distance;
  while (fabs(target_pose_ - current_pose_) > 0.01)
  {
    float step = distance / fabs(distance) * fabs(target_pose_ - current_pose_) * 0.1;
    current_pose_ += step;
    std::cout << "move " << step << " with current position = " << current_pose_ << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }
  status_ = example_ros2_interfaces::msg::RobotStatus::STATUS_STOP;
  return current_pose_;
}

class ExampleInterfaceRobot : public rclcpp::Node
{
private:
  Robot robot;
  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::Service<example_ros2_interfaces::srv::MoveRobot>::SharedPtr move_robot_server_;
  rclcpp::Publisher<example_ros2_interfaces::msg::RobotStatus>::SharedPtr robot_status_publisher_;
  void timer_callback();
  void handle_move_robot(const std::shared_ptr<example_ros2_interfaces::srv::MoveRobot::Request> request, std::shared_ptr<example_ros2_interfaces::srv::MoveRobot::Response> response);
public:
  ExampleInterfaceRobot(std::string name);
  ~ExampleInterfaceRobot();
};

void ExampleInterfaceRobot::timer_callback()
{
  example_ros2_interfaces::msg::RobotStatus message;
  message.status = robot.get_status();
  message.pose = robot.get_current_pose();
  RCLCPP_INFO(this->get_logger(), "Publishing %f", robot.get_current_pose());
  robot_status_publisher_->publish(message);
}

void ExampleInterfaceRobot::handle_move_robot(const std::shared_ptr<example_ros2_interfaces::srv::MoveRobot::Request> request, std::shared_ptr<example_ros2_interfaces::srv::MoveRobot::Response> response)
{
  RCLCPP_INFO(this->get_logger(), "receive distance = %f, current position = %f", request->distance, robot.get_current_pose());
  robot.move_distance(request->distance);
  response->pose = robot.get_current_pose();
}

ExampleInterfaceRobot::ExampleInterfaceRobot(std::string name) : Node(name)
{
  RCLCPP_INFO(this->get_logger(), "%s node start", name.c_str());
  move_robot_server_ = this->create_service<example_ros2_interfaces::srv::MoveRobot>("move_robot", std::bind(&ExampleInterfaceRobot::handle_move_robot, this, std::placeholders::_1, std::placeholders::_2));
  robot_status_publisher_ = this->create_publisher<example_ros2_interfaces::msg::RobotStatus>("robot_status", 10);
  timer_ = this->create_wall_timer(std::chrono::milliseconds(500), std::bind(&ExampleInterfaceRobot::timer_callback, this));
}

ExampleInterfaceRobot::~ExampleInterfaceRobot()
{
}


int main(int argc, char** argv)
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<ExampleInterfaceRobot>("example_interface_robot_01");
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}

