#include "Device_manager.hpp"
#include "tap.hpp"

#include <string>
#include <iostream>
#include <chrono>
#include <thread>
#include <atomic>
#include <utility>
#include <functional>
#include <sstream>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <vector>

char title_devices[] = "Available devices";
char title_rx[] = "Received messages";
char title_serv[] = "Service info";


string name_card;

using namespace std;


/*
            cout << endl;
            chrono::time_point<chrono::system_clock> now = chrono::system_clock::now();
            auto duration = now.time_since_epoch();
            auto seconds  = chrono::duration_cast<chrono::seconds>(duration);
                duration -= seconds;
            auto milliseconds = chrono::duration_cast<chrono::milliseconds>(duration);
                duration -= milliseconds;
            auto microseconds = chrono::duration_cast<chrono::microseconds>(duration);
                duration -= microseconds;
            auto nanoseconds = chrono::duration_cast<chrono::nanoseconds>(duration);
                cout << dec  << seconds.count()         << "(s)." 
                             << milliseconds.count()    << "(ms)."   
                             << microseconds.count()    << "(mcs)."
                             << nanoseconds.count()     << "(ns)." <<endl;
        
            this_thread::sleep_for(chrono::milliseconds(10));
*/

int main() 
{
    WINDOW *rx    ;
    WINDOW *devs  ;
    WINDOW *serv  ;

    Device_manager sniff_dev;
    vector<string> devs_names = sniff_dev.show_available_dev();
    
    initscr();
    curs_set(0);
    refresh();
    

    unsigned choice = 0; 

    curs_set(0); 
    keypad(stdscr, true); 
    
    while(1)
    {
        rx          = newwin(20, 50, 0,0);
        devs        = newwin(10, 20, 0,50);
        serv        = newwin(10, 20, 10,50);
        mvaddstr(0, 52, title_devices);
        mvaddstr(0, 2, title_rx);
        mvaddstr(10, 52, title_serv);

        box(rx, 0, 0);
        box(devs, 0, 0);
        box(serv, 0, 0);

        wrefresh(rx);
        wrefresh(devs);
        wrefresh(serv);

        uint32_t k =2;
        for ( unsigned i = 0; i < devs_names.size(); i++ ) 
        {
            mvprintw(k++, 52,"%s", &devs_names[i][0]);

            if ( i == choice ) 
            {
                addch('<');
                name_card = devs_names[i];
            }
            else          
                addch(' '); 

            
            
        }

        switch ( getch() )
        {
            case KEY_UP:
                if ( choice ) 
                    choice--; 
                break;
            case KEY_DOWN:
                if ( choice != 3 ) 
                    choice++;
                break;

            case KEY_LEFT:
                goto card_selected; 
        }

    }

    card_selected:

    TAPmodule tap(name_card);
    string name_bridge = tap.ConnectTAP(0x0A00A8C0);
    mvaddstr(12, 52, &name_bridge[0]);
    getch();
    endwin();
    system("clear");

    delwin(rx);
    delwin(devs);
    delwin(serv);
    
    return 0;
}


