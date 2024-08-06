#include "tap.hpp"
#include <iostream>


/* Flags: IFF_TUN   - TUN device (no Ethernet headers)
 *        IFF_TAP   - TAP device
 *        IFF_NO_PI - Do not provide packet information
*/     
TAPmodule::TAPmodule(const string& name_card)
{
    struct ifreq ifr;
    int32_t  fd, err;
    char* clonedev = "/dev/net/tun";
    if ((fd = open(clonedev, O_RDWR)) < 0) 
      return;
    memset(&ifr, 0, sizeof(ifr));
    ifr.ifr_flags = IFF_TAP | IFF_NO_PI;
    if ((err = ioctl(fd, TUNSETIFF, (void *) &ifr)) < 0) 
    {
        close(fd);
        return;
    }
    name_card_ = name_card;
    fileDescriptor_ = fd;
    name_tap_ = ifr.ifr_name; 
}



TAPmodule::~TAPmodule(void)
{
    string downTAP ="sudo ifconfig "s;
    downTAP += name_tap_ + " ";
    downTAP += "down";
    char *dwnTap = new char[downTAP.length() + 1];
    strcpy(dwnTap, downTAP.c_str());
    system (dwnTap);
    delete [] dwnTap;
    cout << "turned off tap" <<endl; 
    
    string downCard ="sudo ifconfig "s;
    downCard += name_card_ + " ";
    downCard += "down";
    char *dwnCard = new char[downCard.length() + 1];
    strcpy(dwnCard, downCard.c_str());
    system (dwnCard);
    delete [] dwnCard;
    cout << "turned off NetCard" <<endl;
    
    string downBR ="sudo ifconfig "s;
    downBR += name_bridge_ + " ";
    downBR += "down";
    char *dwnbr = new char[downBR.length() + 1];
    strcpy(dwnbr, downBR.c_str());
    system (dwnbr);
    delete [] dwnbr;
    cout << "turned off Bridge" <<endl;
        
    string delifNetCard = "sudo brctl delif bridge"s;
    delifNetCard  += name_card_ + " ";
    delifNetCard  += name_card_;
    char *delifNtcard = new char[delifNetCard.length() + 1];
    strcpy(delifNtcard, delifNetCard.c_str());
    system (delifNtcard);
    delete [] delifNtcard;
    cout << "deleted netcard to bridge" <<endl;
    
    string delifTAP = "sudo brctl delif bridge"s;
    delifTAP += name_card_ + " ";
    delifTAP += name_tap_;
    char *delifTp = new char[delifTAP.length() + 1];
    strcpy(delifTp, delifTAP.c_str());
    system (delifTp);
    delete [] delifTp;
    cout << "deleted tap to bridge" <<endl;

    string delifBR = "sudo brctl delbr "s;
    delifBR += name_bridge_;
    char *delifbr = new char[delifBR.length() + 1];
    strcpy(delifbr, delifBR.c_str());
    system (delifbr);
    delete [] delifbr;
    cout << "deleted bridge" <<endl;
    
}

string TAPmodule::ConnectTAP(const uint32_t& IP)
{
    string downTAP ="sudo ifconfig "s;
    downTAP += name_tap_ + " ";
    downTAP += "down";
    char *dwnTap = new char[downTAP.length() + 1];
    strcpy(dwnTap, downTAP.c_str());
    system (dwnTap);
    delete [] dwnTap;
    
    string downCard ="sudo ifconfig "s;
    downCard += name_card_ + " ";
    downCard += "down";
    char *dwnCard = new char[downCard.length() + 1];
    strcpy(dwnCard, downCard.c_str());
    system (dwnCard);
    delete [] dwnCard;
    
    string addbridge = "sudo brctl addbr bridge"s;
    addbridge += name_card_;
    name_bridge_ = "bridge" + name_card_;
    char *addbr = new char[addbridge.length() + 1];
    strcpy(addbr, addbridge.c_str());
    system (addbr);
    delete [] addbr;

    string addifNetCard = "sudo brctl addif bridge"s;
    addifNetCard  += name_card_ + " ";
    addifNetCard  += name_card_;
    char *addifNtcard = new char[addifNetCard.length() + 1];
    strcpy(addifNtcard, addifNetCard.c_str());
    system (addifNtcard);
    delete [] addifNtcard;
    
    string addifTAP = "sudo brctl addif bridge"s;
    addifTAP += name_card_ + " ";
    addifTAP += name_tap_;
    char *addifTp = new char[addifTAP.length() + 1];
    strcpy(addifTp, addifTAP.c_str());
    system (addifTp);
    delete [] addifTp;
    
    string ifconfigIP = "sudo ifconfig " + name_bridge_ + " "+ to_string(0xFF&(IP)) + "." + to_string(0xFF&(IP>>8)) + "." + to_string(0xFF&(IP>>16))+ "." + to_string(0xFF&(IP>>24));
    char *ifconfIP = new char[ifconfigIP.length() + 1];
    strcpy(ifconfIP, ifconfigIP.c_str());
    system (ifconfIP);
    delete [] ifconfIP;
    
    string upTAP ="sudo ifconfig "s;
    upTAP += name_tap_ + " ";
    upTAP += "up";
    char *upTap = new char[upTAP.length() + 1];
    strcpy(upTap, upTAP.c_str());
    system (upTap);
    delete [] upTap;
    
    string upCard ="sudo ifconfig "s;
    upCard += name_card_ + " ";
    upCard += "up";
    char *upCARD = new char[upCard.length() + 1];
    strcpy(upCARD, upCard.c_str());
    system (upCARD);
    delete [] upCARD;
    
    return name_bridge_;
}

int TAPmodule::readTAP(uint8_t* frame, int32_t size)
{
		int n_read = read(fileDescriptor_, frame, size);
        if (n_read < 0) 
        {
            cout << "Error read TAP" << endl;
            close(fileDescriptor_);
            exit(1);
        }
		return n_read;
}

bool TAPmodule::writeTAP(uint8_t* frame, int32_t size)
{
		int n_read = write(fileDescriptor_, frame, size);
        if (n_read < 0) 
			return false;
		else
			return true;
}










