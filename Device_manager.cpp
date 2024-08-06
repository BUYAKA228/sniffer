#include "Device_manager.hpp"
#include <iostream>


Device_manager::Device_manager()
{
    struct ifaddrs *ifaddr, *ifa;
    if (getifaddrs(&ifaddr) == -1)
    {
        perror("getifaddrs");
        exit(EXIT_FAILURE);
    }
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
    {
        struct ifreq sa;
        int fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);
        strcpy (sa.ifr_name,ifa->ifa_name);
        if (0 == ioctl(fd, SIOCGIFHWADDR, &sa))
        {
            
            for(int i=0; i<6; i++)
                Net_cards_[ifa->ifa_name][i] = 0xFF&sa.ifr_addr.sa_data[i];
        }
    }
}


vector<string> Device_manager::show_available_dev()
{
    unsigned int i =0;
    vector<string> devs_names;
    for(auto[name, mac]:Net_cards_)
       devs_names.push_back(name);
    return devs_names;
}


string Device_manager::get_NameCard_by_Num(uint32_t i)
{
    uint32_t count =0;
    for(auto[name, mac]:Net_cards_)
    {
      if(i == count) return name;
    }
    cout << "Card not found" << endl;
    return "None";
}