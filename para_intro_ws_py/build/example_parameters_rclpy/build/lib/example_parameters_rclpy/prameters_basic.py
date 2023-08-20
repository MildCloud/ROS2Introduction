#!/usr/bin/env python3

import rclpy
from rclpy.node import Node
from std_msgs.msg import String


class ParameterBasicNode(Node):

    def __init__(self, name):
        super().__init__(name)
        self.get_logger().info('name: %s' %name)
        self.declare_parameter('rcl_log_level', 10)
        log_level = self.get_parameter('rcl_log_level').value
        self.get_logger().set_level(log_level)
        self.timer = self.create_timer(0.5, self.timer_callback)

    def timer_callback(self):
        log_level = self.get_parameter("rcl_log_level").value
        self.get_logger().set_level(log_level)
        print(f"========================{log_level}=============================")
        self.get_logger().debug("DEBUG")
        self.get_logger().info("INFO")
        self.get_logger().warn("WARN")
        self.get_logger().error("ERROR")
        self.get_logger().fatal("FATAL")

def main(args=None):
    rclpy.init(args=args)
    node = ParameterBasicNode("parameters_basic")
    rclpy.spin(node)
    rclpy.shutdown()
