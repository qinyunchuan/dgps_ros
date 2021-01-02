#include <ros/ros.h>
#include <thread>
#include <string>
#include "ntrip.h"


int main(int argc, char **argv)
{
    ros::init(argc, argv, "dgps_node");
    ros::NodeHandle nh("~");


    std::string serverName,userName,password,serverPort,serialPort;
    nh.param<std::string>("server",serverName, "rtk.ntrip.qxwz.com");
    nh.param<std::string>("port",serverPort, "8002");
    nh.param<std::string>("username",userName, "qxsskx001");
    nh.param<std::string>("password",password, "e513f03");
    nh.param<std::string>("serialPort",serialPort, "");

    struct Args args;
    args.server = serverName.c_str();
    args.port = "8002";
    args.user = userName.c_str();
    args.password = password.c_str();
    args.nmea = "$GNGGA,034458.00,2810.79928,N,11256.54467,E,2,12,0.64,36.0,M,-12.7,M,1.0,0773*7D";
    args.data = "RTCM32_GGB";
    args.bitrate = 0;
    args.proxyhost = 0;
    args.proxyport = "2101";
    args.mode = NTRIP1;
    args.initudp = 0;
    args.udpport = 0;
    args.protocol = SPAPROTOCOL_NONE;
    args.parity = SPAPARITY_NONE;
    args.stopbits = SPASTOPBITS_1;
    args.databits = SPADATABITS_8;
    args.baud = SPABAUD_115200;
    if( serialPort.empty())
        args.serdevice = 0;//"/dev/ttyUSB0";  
    else
        args.serdevice = serialPort.c_str();
    args.serlogfile = 0;
    args.stop = false;

    
    ROS_INFO("Username= %s",args.user); 
    ROS_INFO("password= %s",args.password); 

    
    std::thread ntrip_thread(ntrip_client,&args);
    ros::Rate loop_rate(10);
    while (ros::ok())
    {
        Location loc = getGNGGA();
        if( loc.lat.empty() )
        {
        }
        else
        {
           // cout<<s;
            ROS_INFO("Talker_____:GPS:x = %s",loc.nmea.c_str()); 
        }
        ros::spinOnce();
        loop_rate.sleep();
    }
    args.stop = true;
    ROS_INFO("Waiting to Quit");
    ntrip_thread.join();
    
}

