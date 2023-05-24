#include "rclcpp/rclcpp.hpp"


class Node02: public rclcpp::Node {
    public:
        Node02(std::string name): Node(name) {
            RCLCPP_INFO(this->get_logger(), "%s",name.c_str());
        }
};


int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    /*产生一个node_03的节点*/
    auto node = std::make_shared<Node02>("node_02");
    /* 运行节点，并检测退出信号*/
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
