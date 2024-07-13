#ifndef TRAFFICSIG_H
#define TRAFFICSIG_H

#include "CppThread.h"
#include "Readmsg.h"
#include "Server.h"
#include <iostream>

class Trafficsig : public CppThread {
private:
    Readmsg& rdmsg;
    Server& ser;
    bool traffic_signal;
    int count = 0;
    const int timer_id = 0;

    static void staticTrafficSignalHandler(void* userdata) {
        
        // Cast userdata to Trafficsig instance and call the member function
        Trafficsig* instance = static_cast<Trafficsig*>(userdata);
        instance->trafficSignalHandler();
    }

    void trafficSignalHandler() {
        
        ser.sendtrafsig(traffic_signal);
        traffic_signal = !traffic_signal;
        ser.sendtrafsig(traffic_signal);
        std::cout << "The traffic signal is " << traffic_signal << std::endl;
    }
void run(){
     
    traffic_signal=0;
    while(1){
        if(strcmp(rdmsg.rdmsg,"Entered")==0)
        {
        
        for(int i=0; i<2;i++){
            
            gpioSetTimerFuncEx(timer_id, 5000, staticTrafficSignalHandler, this);
        }
        
        }else if (strcmp(rdmsg.rdmsg,"Exited")==0)
        {
            traffic_signal=0;
            std::cout<<"The traffic signal is "<<traffic_signal<<std::endl;
         }
        
    }
}
public:
    Trafficsig(Readmsg& rdmsg, Server& server): rdmsg(rdmsg), ser(server), traffic_signal(false) {}
};

#endif // TRAFFICSIG_H
