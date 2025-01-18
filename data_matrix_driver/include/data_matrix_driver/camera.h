#ifndef PGV_CAMERA_H_
#define PGV_CAMERA_H_
#include<string>
#include<vector>
#include<rclcpp/rclcpp.hpp>
#include<geometry_msgs/msg/point.hpp>
#include<std_msgs/msg/int16.hpp>
#include "data_matrix_driver/serial.h"

namespace PGV
{
class Camera: public rclcpp::Node
{
    public:
        Camera();

        void initialize(void);

        void grab(void);


    private:
        
        Serial _cam_port;
        std::vector<unsigned char> _init = {0xEC,0x13};
        std::vector<unsigned char> _command = {0xC8,0x37};
        rclcpp::Publisher<std_msgs::msg::Int16>::SharedPtr _tag_pub;
        rclcpp::Publisher<geometry_msgs::msg::Point>::SharedPtr _pos_pub;
        float _pos_x , _pos_y ;
        int _tag;
        float _angle;
};
}
#endif
