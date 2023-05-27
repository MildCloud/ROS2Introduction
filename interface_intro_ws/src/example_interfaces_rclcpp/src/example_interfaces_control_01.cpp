#include "rclcpp/rclcpp.hpp"
#include "example_ros2_interfaces/srv/move_robot.hpp"
#include "example_ros2_interfaces/msg/robot_status.hpp"


class ExampleInterfacesControl : public rclcpp::Node
{
private:
    rclcpp::Client<example_ros2_interfaces::srv::MoveRobot>::SharedPtr client_;
    rclcpp::Subscription<example_ros2_interfaces::msg::RobotStatus>::SharedPtr robot_status_subscribe_;
    void result_callback_(rclcpp::Client<example_ros2_interfaces::srv::MoveRobot>::SharedFuture result_future);
    void robot_status_callback_(const example_ros2_interfaces::msg::RobotStatus::SharedPtr msg);
public:
    ExampleInterfacesControl(std::string name);
    void move_robot(float distance);
};

void ExampleInterfacesControl::result_callback_(rclcpp::Client<example_ros2_interfaces::srv::MoveRobot>::SharedFuture result_future)
{
    auto response = result_future.get();
    RCLCPP_INFO(this->get_logger(), "move result = %f", response->pose);
}

void ExampleInterfacesControl::robot_status_callback_(const example_ros2_interfaces::msg::RobotStatus::SharedPtr msg)
{
    RCLCPP_INFO(this->get_logger(), "receive positioin = %f, status = %d", msg->pose ,msg->status);
}
    
ExampleInterfacesControl::ExampleInterfacesControl(std::string name) : Node(name)
{
    RCLCPP_INFO(this->get_logger(), "%s node start", name.c_str());
    client_ = this->create_client<example_ros2_interfaces::srv::MoveRobot>("move_robot");
    robot_status_subscribe_ = this->create_subscription<example_ros2_interfaces::msg::RobotStatus>("robot_status", 10, std::bind(&ExampleInterfacesControl::robot_status_callback_, this, std::placeholders::_1));
}

void ExampleInterfacesControl::move_robot(float distance)
{
    RCLCPP_INFO(this->get_logger(), "request to move %f", distance);
    while (!client_->wait_for_service(std::chrono::seconds(1)))
    {
        if (!rclcpp::ok())
        {
            RCLCPP_ERROR(this->get_logger(), "interrupted while waiting for service");
            return;
        }
        RCLCPP_INFO(this->get_logger(), "waiting server to setup");
    }
    auto request = std::make_shared<example_ros2_interfaces::srv::MoveRobot::Request>();
    request->distance = distance;
    client_->async_send_request(request, std::bind(&ExampleInterfacesControl::result_callback_, this, std::placeholders::_1));
}


int main(int argc, char** argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<ExampleInterfacesControl>("example_interfaces_control_01");
    node->move_robot(5.0);
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
