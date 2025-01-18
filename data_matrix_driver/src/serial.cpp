#include "data_matrix_driver/serial.h"
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstring>

PGV::Serial::Serial(const std::string &device, const Baud baud) : _bufferSize(1024)
{

    
    _port = device;
    _baud = baud;
   
}

PGV::Serial::Serial(): _bufferSize(1024){}

PGV::Serial::~Serial(void)
{
    ::close(_fd);
}

void PGV::Serial::set_parameter(const std::string &device, const Baud baud)
{
    _port = device;
    _baud = baud;
}

void PGV::Serial::send(const std::vector<unsigned char> &bytes)
{
    ::write(_fd, bytes.data(), bytes.size());
}

bool PGV::Serial::receive(std::vector<unsigned char> &data, const unsigned int bytes, const int timeout)
{
    data.resize(bytes);

    int n = ::read(_fd, data.data(), data.size());

    return n == data.size();
}

bool PGV::Serial::serial_initialize()
{
     _fd = ::open(_port.c_str(), O_RDWR);

    if (_fd < 0)
    {
        std::cout << "Serial: can't open device " << _port << "." << std::endl;
        return false;
    }

    struct termios tty;

    if (tcgetattr(_fd, &tty))
    {
        std::cout << "Serial: can't get the attributes from the device " << _port << "." << std::endl;
        return false;
    }

    cfsetospeed(&tty, _baud);
    cfsetispeed(&tty, _baud);
    tty.c_cflag |= PARENB;
    tty.c_cflag &= ~PARODD;

    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;
    tty.c_cflag &= ~CRTSCTS;
    tty.c_cflag |= CREAD | CLOCAL;

    tty.c_lflag &= ~ICANON;
    tty.c_lflag &= ~ECHO;
    tty.c_lflag &= ~ECHOE;
    tty.c_lflag &= ~ECHONL;
    tty.c_lflag &= ~ISIG;
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL);

    tty.c_oflag &= ~OPOST;
    tty.c_oflag &= ~ONLCR;
    tty.c_cc[VTIME] = 5;
    tty.c_cc[VMIN] = 0;

    if (tcsetattr(_fd, TCSANOW, &tty))
    {
        std::cout << "Rs485: can't set the attributes to the device " << _port << "." << std::endl;
        return false;
    }
    return true;
}

void PGV::Serial::flush() {
            if (tcflush(_fd, TCIOFLUSH) != 0) {
                perror("tcflush failed");
            }
        }
