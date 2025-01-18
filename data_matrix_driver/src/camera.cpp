#include "data_matrix_driver/camera.h"


PGV::Camera::Camera():Node("camera_node")
{
    
    std::string device_id;
    this->declare_parameter<std::string>("device_id",device_id);
    this->get_parameter("device_id",device_id);
    _cam_port.set_parameter(device_id);
    bool check = _cam_port.serial_initialize();
    if(!check)
    {
        RCLCPP_WARN(this->get_logger(), "please check port and connect again!!");
        return;

    }
    initialize();
    grab();
}

void PGV::Camera::initialize(void)
{   
    
    _tag_pub = this->create_publisher<std_msgs::msg::Int16>("pgv/tag",1);
    _pos_pub = this->create_publisher<geometry_msgs::msg::Point>("pgv/position",1);
    
    _cam_port.send(_init);
    sleep(1);
    RCLCPP_INFO(this->get_logger(),"!! publisher and serial port initialized !!");

}


void PGV::Camera::grab(void)
{
    
    auto pos_message = std::make_shared<geometry_msgs::msg::Point>();
    auto tag_message = std::make_shared<std_msgs::msg::Int16>();
    
    while(rclcpp::ok())
    {

        std::vector<unsigned char > data;
	    _cam_port.send(_command);

	    int check = _cam_port.receive(data,21);
	

        _angle = (static_cast<int>((static_cast<unsigned int>(data[11]) <<  0) |
                                          (static_cast<unsigned int>(data[10]) <<  7))); 
	
	    _pos_y  = static_cast<float>((static_cast<signed int>(data[7]) <<  0) |
                                            (static_cast<signed int>(data[6]) <<  7));
                                            
        _pos_x  =  static_cast<float>((static_cast<signed int>(data[5]) <<  0) |
                                                (static_cast<signed int>(data[4]) <<  7));
                                                
        _tag =   static_cast<int>((static_cast<unsigned int>(data[17]) <<  0) |
                                                (static_cast<unsigned int>(data[16]) <<  7) |
                                                (static_cast<unsigned int>(data[15]) << 14) | 
                                                (static_cast<unsigned int>(data[14]) << 21));
        if(_pos_y > 1000)
        {
            _pos_y = -(16383 - _pos_y);
        }
                
        if(_pos_x > 1000)
        {
            _pos_x = -(16383 - _pos_x);
        }
    
        
        pos_message->x = _pos_x/10.;
        pos_message->y = _pos_y/10.;
        pos_message->z = _angle/10.;
        tag_message->data = _tag;

        _pos_pub->publish(*pos_message);
        _tag_pub->publish(*tag_message);

    }

}






