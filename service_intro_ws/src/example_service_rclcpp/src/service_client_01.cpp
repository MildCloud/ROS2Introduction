#include "rclcpp/rclcpp.hpp"
#include "example_interfaces/srv/add_two_ints.hpp"


class ServiceClient01: public rclcpp::Node
{
    public:
        ServiceClient01(std::string name): Node(name) {
            RCLCPP_INFO(this->get_logger(), "%s node start", name.c_str());
        }
    private:
};


int main(int argc, char** argv) {
    rclcpp::init(argc, argv);
    auto node = std::make_shared<ServiceClient01>("service_client_01");
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
