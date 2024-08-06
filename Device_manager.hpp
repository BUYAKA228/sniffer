#include <cstdint>
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <utility>


#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <sys/ioctl.h>
#include <linux/if.h>
#include <netdb.h>
#include <string.h>
#include <cstring>


#pragma once

using namespace std;


class Device_manager
{
    private:
        map<string, unsigned int[6]> Net_cards_;
    
    public:
        Device_manager();
        vector<string>  show_available_dev();
        string          get_NameCard_by_Num(uint32_t i);
		
};
