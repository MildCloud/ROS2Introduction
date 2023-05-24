#include "rclcpp/rclcpp.hpp"


int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    std::shared_ptr<rclcpp::Node> node = std::make_shared<rclcpp::Node>("node_01");
    RCLCPP_INFO(node->get_logger(), "start node_01");
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
