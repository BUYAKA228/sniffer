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

#define SIZE_BUFFER 1500

char title_devices[] = "Available devices";
char title_rx[] = "Received messages";
char title_serv[] = "Service info";
char clear_str[] = "______________________________________________________________";


uint8_t buffer_read[SIZE_BUFFER] ={0};


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
        rx          = newwin(40, 70, 0,0);
        devs        = newwin(10, 20, 0,70);
        serv        = newwin(10, 20, 10,70);
        mvaddstr(0, 72, title_devices);
        mvaddstr(0, 2, title_rx);
        mvaddstr(10, 72, title_serv);

        box(rx, 0, 0);
        box(devs, 0, 0);
        box(serv, 0, 0);

        wrefresh(rx);
        wrefresh(devs);
        wrefresh(serv);

        uint32_t k =2;
        for ( unsigned i = 0; i < devs_names.size(); i++ )
        {
            mvprintw(k++, 72,"%s", &devs_names[i][0]);

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
    mvaddstr(12, 72, &name_bridge[0]);
    mvaddstr(13, 72, "IP:192.168.0.10");
    int count =0;
    int size =0;
    int top_offset = 2;
    int left_offset = 2;
    while(count<20)
    {
      size = tap.readTAP(buffer_read, SIZE_BUFFER);
      if(size)
      {

        if(count%5==0 && count!=0)
        {
          for(int i=2; i<38; i++)
            mvwprintw(stdscr, i,  2, "%s", clear_str);
          top_offset=2;
        }

        mvwprintw(stdscr, top_offset,  2, "%i) size: %i ->", count, size);
        for(int i=0; i<size; i++)
        {
            mvwprintw(stdscr, top_offset+1,  left_offset, "%X ", buffer_read[i]);
            left_offset+=3;
            if(i%21==0 && i!=0)
            {
              top_offset++;
              left_offset=2;
            }
        }
        left_offset =2;
        count++;
        top_offset+=2;
      }
    }
    getch();
    endwin();
    system("clear");

    delwin(rx);
    delwin(devs);
    delwin(serv);

    return 0;
}
