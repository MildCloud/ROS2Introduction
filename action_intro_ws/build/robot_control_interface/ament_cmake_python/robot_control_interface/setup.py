from setuptools import find_packages
from setuptools import setup

setup(
    name='robot_control_interface',
    version='0.0.0',
    packages=find_packages(
        include=('robot_control_interface', 'robot_control_interface.*')),
)
