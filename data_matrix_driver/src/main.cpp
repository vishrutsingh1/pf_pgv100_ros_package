#include "data_matrix_driver/camera.h"



int main(int argc ,char* argv[])
{
    rclcpp::init(argc ,argv);
    PGV::Camera obj;          
    rclcpp::shutdown();
    return 0;
}