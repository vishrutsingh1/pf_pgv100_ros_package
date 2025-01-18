#include "data_matrix_driver/camera.h"



int main(int argc ,char** argv)
{
    ros::init(argc ,argv , "camera_pgv");
    PGV::Camera obj;          
    return 0;
}
