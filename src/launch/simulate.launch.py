import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, Shutdown
from launch_ros.actions import Node


def generate_launch_description():
    config = os.path.join(
        get_package_share_directory("quad"),
        'config',
        'simulate_cfg.yaml'
    )

    # config = '/home/guo/HITCRT_QUAD/mjsim_ws/src/config/simulate_cfg.yaml'
    return LaunchDescription([
        Node(
            package="simulate",
            executable="simulate_node",
            name="sim",
            namespace="quad",
            parameters=[
                config
            ]
        ),
    ])
