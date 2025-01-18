#include "data_matrix_driver/camera.h"


PGV::Camera::Camera()
{
     std::string device_id;
    _node.param<std::string>("device_id",device_id,"/dev/ttyUSB0");
    _cam_port.set_parameter(device_id);
    int check = _cam_port.serial_initialize();
    if(!check)
    {
        ROS_WARN(" please check port and re-initialize!! ");
        return;
    }
    initialize();
    grab();
}

void PGV::Camera::initialize(void)
{   
    _tag_pub = _node.advertise<std_msgs::Int16>("pgv/tag",1);
    _pos_pub = _node.advertise<geometry_msgs::Point>("pgv/position",1);
    _cam_port.send(_init);
    sleep(1);
    ROS_INFO("!! publisher and serial port initialized !!");
}


void PGV::Camera::grab(void)
{
    while(ros::ok())
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
    
        geometry_msgs::Point position;
        std_msgs::Int16 tag;
        position.x = _pos_x/10.;
        position.y = _pos_y/10.;
        position.z = _angle/10.;
        tag.data = _tag;

        _pos_pub.publish(position);
        _tag_pub.publish(tag);

    }

}





