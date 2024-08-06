#include <cstdint>
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>


#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/if.h>
#include <linux/if_tun.h>
#include <unistd.h>


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

class TAPmodule
{
    private:
        int fileDescriptor_;
        string name_tap_;
        string name_card_;
        string name_bridge_;
        
    
    public:

        TAPmodule(const string& name_card);
        string ConnectTAP(const uint32_t& IP);
		int readTAP(uint8_t* frame, int32_t size);
		bool writeTAP(uint8_t* frame, int32_t size);
        ~TAPmodule(void);
};



