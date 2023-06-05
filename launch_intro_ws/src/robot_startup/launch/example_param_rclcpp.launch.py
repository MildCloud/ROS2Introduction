from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    """Launch content description, called by ros2 launch"""
    parameter_basic1 = Node(package="example_parameters_rclcpp", namespace="rclcpp", executable="parameter_basic", parameters=[{'rcl_log_level': 10}])
    launch_description = LaunchDescription([parameter_basic1])
    return launch_description
