#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fstream>
#include <thread>
#include <pigpio.h>

using namespace std;


class CppThread {

public:
	inline void start() {
		uthread = std::thread(&CppThread::run, this);
	}

	inline void join() {
		uthread.join();
	}

protected:
	virtual void run() = 0;	

private:
	// pointer to the thread
	std::thread uthread;
};

class Server{
private:
    char bff[8] ={0};
    char* rddmsg[8]={0};
    int bffsize = 8;
public:
   
    int clt_soc;

    int byteno(){
        return read(clt_soc, bff,8);
    }
    
    void setupServer(int& server_fd, struct sockaddr_in& address, int port) {
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        std::cout<<"Socket creation failed"<<std::endl;
        exit(EXIT_FAILURE);
    } else if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) != 0)
    {
        std::cout<<"Socket creation succeed!!"<<std::endl;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    std::cout<<"After socket creation"<<std::endl;

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        std::cout<<"Bind failed"<<std::endl;
        exit(EXIT_FAILURE);
    } else if(bind(server_fd, (struct sockaddr *)&address, sizeof(address)) >= 0)
    {
        std::cout<<"Bind success"<<std::endl;
    }

    if (listen(server_fd, 3) < 0) {
        std::cout<<"Listen"<<std::endl;
        exit(EXIT_FAILURE);
    }}

    void acceptConnection(int server_fd, struct sockaddr_in& address) {
    int addrlen = sizeof(address);
     clt_soc = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    std::cout<<"after socket accept"<<endl;
    if (clt_soc > 0)
    {
        std::cout<<"Accept"<<std::endl;
        //exit(EXIT_FAILURE);
    }

    }

    char* readctmsg(){
        char* buffer = new char[bffsize];
        memset(buffer, 0, bffsize);
        ssize_t bytesRead = read(clt_soc,buffer,bffsize);
       // Check if read was successful
        if (bytesRead < 0) {
        // Error handling
            std::cerr << "Error reading from socket" << std::endl;
            delete[] buffer; // Free allocated memory
            return nullptr;
        } else if (bytesRead == 0) {
        // Connection closed
            std::cerr << "Connection closed by peer" << std::endl;
            delete[] buffer; // Free allocated memory
            return nullptr;
        } else {
        // Read successful
            return buffer;
        }
    }

    void sendtrafsig(bool tfsig){
        char tfmsg[2]={0} ;
        if (tfsig==0)
        {
            strcpy(tfmsg, "R");
             write(clt_soc, tfmsg, 2);
        }else
        { 
            strcpy(tfmsg, "G");
            write(clt_soc, tfmsg, 2);

        }
       
    }
};


class Readmsg: public CppThread{
    private:

    Server& ser; 

    void run(){
                

        while(1)
        {   if(ser.byteno()!=0)
            {
            
            strcpy(rdmsg, ser.readctmsg());
            std::cout<<rdmsg<<std::endl;    
            //std::this_thread::sleep_for(200ms);
        }
        
    };
}
public:
 Readmsg(Server& serversig):ser(serversig){};
bool trigger;
char rdmsg[9]={0};
};

class Trafficsig: public CppThread{
 private:
    Readmsg& rdmsg;
    Server& ser;
    bool traffic_signal;
    int count= 0;
    const int timer_id = 0;


void trafficSignalHandler(int timer_id, void* userdata){
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
            
            gpioSetTimerFuncEx(timer_id, 5000, trafficSignalHandler, nullptr);
        }
         count=0;
        count++;
        std::cout<<count<<std::endl;
        
        }else if (strcmp(rdmsg.rdmsg,"Exited")==0)
        {
            num=0;
            std::cout<<"The traffic signal is "<<num<<std::endl;
         }
        
    }
 }   
public:

Trafficsig(Readmsg& Rdmsg, Server& svr) : rdmsg(Rdmsg), ser(svr), num(false) {}
};


int main() {
    int server_fd;
    struct sockaddr_in address;
    int port = 5560;
    char keyin = 0;
    Server server;
    Readmsg rdmsg(server);
    Trafficsig trafsig(rdmsg,server);

    server.setupServer(server_fd, address, port);
    std::cout<<"aaa"<<std::endl;
    server.acceptConnection(server_fd, address);
    trafsig.start();
    rdmsg.start();
    trafsig.join();
    rdmsg.join();
    std::cout<<"End connection?(y/n)"<<std::endl;
    std::cin>>keyin;
    if(keyin=='y')
       { close(server.clt_soc);
        close(server_fd);
       }
    
return 0;
}
