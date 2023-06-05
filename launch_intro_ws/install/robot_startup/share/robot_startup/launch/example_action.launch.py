from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    """Launch content description, called by ros2 launch"""
    action_robot_01 = Node(package="example_action_rclcpp", executable="action_robot_01")
    action_control_01 = Node(package="example_action_rclcpp", executable="action_control_01")
    launch_description = LaunchDescription([action_robot_01, action_control_01])
    return launch_description

"""----------------Usage--------------"""
# source ../../chapt5/chapt5_ws/install/setup.bash
# source install/setup.bash
# ros2 launch robot_startup example_action.launch.py