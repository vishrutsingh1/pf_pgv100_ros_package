from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    return LaunchDescription([Node(

        package="data_matrix_driver",
        executable="camera_driver_node",
        output="screen",
        emulate_tty=True,
        parameters = [{'device_id': '/dev/Camera'}]

 )])