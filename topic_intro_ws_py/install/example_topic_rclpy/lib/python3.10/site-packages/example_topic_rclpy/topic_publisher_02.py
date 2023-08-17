#!/usr/bin/env python3

import rclpy
from rclpy.node import Node
from std_msgs.msg import String


class NodePublisher02(Node):
    def __init__(self, name):
        super().__init__(name)
        self.get_logger().info('name: %s' %name)
        self.command_publisher_ = self.create_publisher(String, "tianshu_command", 10)
        self.timer = self.create_timer(0.5, self.timer_callback)
    
    def timer_callback(self):
        msg = String()
        msg.data = 'forward'
        self.command_publisher_.publish(msg)
        self.get_logger().info(f'publish command: {msg.data}')

def main(args=None):
    rclpy.init(args=args)
    node = NodePublisher02("topic_publisher_02")
    rclpy.spin(node)
    rclpy.shutdown()
