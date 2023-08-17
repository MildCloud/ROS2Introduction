#!/usr/bin/env python3

import rclpy
from rclpy.node import Node
from std_msgs.msg import String


class NodeSubscribe02(Node):
    def __init__(self, name):
        super().__init__(name)
        self.get_logger().info('name: %s' %name)
        self.command_subscribe_ = self.create_subscription(String, "tianshu_command", self.command_callback, 10)

    def command_callback(self, msg):
        speed = 0.0
        if msg.data == 'forward':
            speed = 0.2
        self.get_logger().info(f'receive {msg.data}, set speed to {speed}')

def main(args=None):
    rclpy.init(args=args)
    node = NodeSubscribe02("topic_Subscribe_02")
    rclpy.spin(node)
    rclpy.shutdown()
