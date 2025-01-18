#ifndef PGV_CAMERA_H_
#define PGV_CAMERA_H_
#include<string>
#include<vector>
#include<ros/ros.h>
#include<geometry_msgs/Point.h>
#include<std_msgs/Int16.h>
#include "data_matrix_driver/serial.h"
namespace PGV
{
class Camera
{
    public:
        Camera();

        void initialize(void);

        void grab(void);

    private:
        Serial _cam_port;
        std::vector<unsigned char> _init = {0xEC,0x13};
        std::vector<unsigned char> _command = {0xC8,0x37};
        ros::Publisher _tag_pub;
        ros::Publisher _pos_pub;
        ros::NodeHandle _node;
        float _pos_x , _pos_y ;
        int _tag;
        float _angle;
        
};
}
#endif
