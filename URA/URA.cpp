#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <cstdlib>
#include <unistd.h>
#include <iostream>
#include <termios.h>
#include <cstring>
#include <fstream>
#include <iomanip>
#include "times.h"
#include "DopplerChannel/FileUtils.h"
#define BUF_SIZE 4096
//#define BUF_SIZE 65500

int PACKET_SIZE = 2617;

using namespace std;
std::ofstream log_file("log_file_URA.txt", std::ios_base::out);
long msec = 0;
char time_str[sizeof("1900-01-01 23:59:59")];
char time_str2[sizeof("1900-01-01 23:59:59")];
char time_str3[sizeof("1900-01-01 23:59:59")];

int main(int argc, char* argv[])
{
    msec = times(time_str, time_str2, time_str3);
    log_file << left << setfill(' ') << setw(70) << " Start URA " << " | " << time_str<<"-"<< time_str2<<":"<< time_str3 << ":" << right << setfill('0') << setw(6) <<  msec << endl;
    cout << "Start URA\n";
    if(argc != 3)
    {
        cout << "Incorrect URA using!\n";
        msec = times(time_str, time_str2, time_str3);
        log_file << left << setfill(' ') << setw(70) << " End URA " << " | " << time_str<<"-"<< time_str2<<":"<< time_str3 << ":" << right << setfill('0') << setw(6) <<  msec << endl;
        return -1;
    }

    char *param = "Configuration.cfg";
    int packet_size=0, data_rate=0;
    ReadInt("General", "packet_size", param, "decimal", &packet_size);
    ReadInt("General", "data_rate", param, "decimal", &data_rate);
    cout << "packet_size = " << packet_size << endl;
    cout << "data_rate = " << data_rate << endl;

    if ( data_rate == 6 )
    {
        PACKET_SIZE = packet_size;
        if(packet_size > 288)
            PACKET_SIZE = 288;
    }
    else if ( data_rate == 9 )
    {
        PACKET_SIZE = packet_size;
        if(packet_size > 433)
            PACKET_SIZE = 433;
    }
    else if ( data_rate == 12 )
    {
        PACKET_SIZE = packet_size;
        if(packet_size > 579)
            PACKET_SIZE = 579;
    }
    else if ( data_rate == 18 )
    {
        PACKET_SIZE = packet_size;
        if(packet_size > 870)
            PACKET_SIZE = 870;
    }
    else if ( data_rate == 24 )
    {
        PACKET_SIZE = packet_size;
        if(packet_size > 1161)
            PACKET_SIZE = 1161;
    }
    else if ( data_rate == 36 )
    {
        PACKET_SIZE = packet_size;
        if(packet_size > 1743)
            PACKET_SIZE = 1743;
    }
    else if ( data_rate == 48 )
    {
        PACKET_SIZE = packet_size;
        if(packet_size > 2325)
            PACKET_SIZE = 2325;
    }
    else if ( data_rate == 54 )
    {
        PACKET_SIZE = packet_size;
        if(packet_size > 2616)
            PACKET_SIZE = 2616;
    }
    else
    {
        std::cout << "Invalid data_rate in Configuration.cfg" << std::endl;
        exit(1);
    }

    int port_transmitter;
    sscanf(argv[1], "%d", &port_transmitter); //Port number translating from char* to int
    int port_receiver;
    sscanf(argv[2], "%d", &port_receiver); //Port number translating from char* to int

    int sock_transmitter;
    struct sockaddr_in addr_transmitter;
    int sock_receiver;
    struct sockaddr_in addr_receiver;
    char buff[BUF_SIZE];
    socklen_t serAddrLenTransmitter;
    socklen_t serAddrLenReceiver;
    int cliSockDes, readStatus;

    struct sockaddr_in addr, cliAddr;
    socklen_t cliAddrLen;

    sock_transmitter = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    sock_receiver = socket(AF_INET, SOCK_DGRAM, 0);

    if(sock_transmitter < 0)
    {
        perror("socket_transmitter");
        return -2;
    }
    if(sock_receiver < 0)
    {
        perror("socket_receiver");
        return -2;
    }

    addr_receiver.sin_family = AF_INET;
    addr_receiver.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    unsigned int port;
    for(port = port_receiver; port <= port_receiver + 100; port++) //Free ports checking
    {
        std::cout << "1\n" <<  std::endl;
        addr_receiver.sin_port = htons(port);
        if(bind(sock_receiver, (struct sockaddr*)&addr_receiver, sizeof(addr_receiver)) == 0) {
            std::cout << "OK" << std::endl;
            break;
        }
    }

    addr_transmitter.sin_family = AF_INET;
    addr_transmitter.sin_port = htons(port_transmitter); //Destination port for transfer
    addr_transmitter.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    std::ofstream fout("Pids_Storage.txt", std::ios_base::out);
    fout << getpid(); //Writing the PID...
    fout << "\n";
    fout.close();

    char buf[BUF_SIZE];

    serAddrLenReceiver = sizeof(addr_receiver);
    serAddrLenTransmitter = sizeof(addr_transmitter);

    cliAddrLen = sizeof(cliAddr);
    int sizebuf = 0;
    long str;
    int ci = 0, ci2 = 0, ci3 = 0;
    ifstream file2("DATA.txt");
    file2.seekg (0, std::ios::end);
    int size_file = 0;
    size_file = file2.tellg();
    cout << "Size of file: " << size_file << " byte" << endl;
    file2.close();

    ifstream file("DATA.txt");
    std::ofstream in_file("DATA_FROM.txt", std::ios_base::out);
    int in=0;
    //while(1) //Endless transfer
    while(ci<size_file)
    {
        cout << "Want to send: " << endl;
        memset(buf, 0, BUF_SIZE);
        while (ci < PACKET_SIZE + ci2)
        {
            file.get(buf[ci3]);
            cout << buf[ci3];
            ci++;
            ci3++;
            if(file.eof())
                break;
        }
        ci2 = ci;

        sizebuf = sizeof(buf);
        cout << endl << "Size buf = " << sizebuf << endl;
        cout << "Total bytes processed = " << ci << endl;
        cout << "Bytes processed in the loop = " << ci3 << endl;

        msec = times(time_str, time_str2, time_str3);
        log_file << left << setfill(' ') << setw(70) << " Start time for sending data to the transmitter " << " | " << time_str<<"-"<< time_str2<<":"<< time_str3 << ":" << right << setfill('0') << setw(6) <<  msec << endl;

        //if (sendto(sock_transmitter, buf, BUF_SIZE, 0, (struct sockaddr*)&addr_transmitter, sizeof(addr_transmitter)) < 0) {
        if (sendto(sock_transmitter, buf, ci3, 0, (struct sockaddr*)&addr_transmitter, sizeof(addr_transmitter)) < 0) {
            perror("sending error...\n");
            close(sock_transmitter);
            exit(-1);
        }

        msec = times(time_str, time_str2, time_str3);
        log_file << left << setfill(' ') << setw(70) << " Finish time for sending data to the transmitter " << " | " << time_str<<"-"<< time_str2<<":"<< time_str3 << ":" << right << setfill('0') << setw(6) <<  msec << endl;
        msec = times(time_str, time_str2, time_str3);
        log_file << left << setfill(' ') << setw(70) << " Start time for receiving data " << " | " << time_str<<"-"<< time_str2<<":"<< time_str3 << ":" << right << setfill('0') << setw(6) <<  msec << endl;

        //readStatus = recvfrom(sock_receiver , buff , BUF_SIZE, 0 , (struct sockaddr*)&cliAddr, &cliAddrLen);
        readStatus = recvfrom(sock_receiver , buff , ci3, 0 , (struct sockaddr*)&cliAddr, &cliAddrLen);

        msec = times(time_str, time_str2, time_str3);
        log_file << left << setfill(' ') << setw(70) << " Finish time for receiving data " << " | " << time_str<<"-"<< time_str2<<":"<< time_str3 << ":" << right << setfill('0') << setw(6) <<  msec << endl;

        sizebuf = sizeof(readStatus);
        cout << "Size buf readStatus = " << sizebuf << endl;
        cout << "ReadStatus = " << readStatus << endl;
        if (readStatus < 0) {
            perror("reading error...\n");
            close(sock_receiver);
            exit(-1);
        }
        cout << endl<< "Poluchili: " << endl;
        cout.write(buff, readStatus);
        cout << endl << endl;
        while(in<readStatus)
        {
            in_file << buff[in];
            in++;
        }
        in = 0;
        ci3 = 0;
        //log_file.close();
        //break;
    }
    in_file.close(); // закрываем файл
    file.close(); // закрываем файл
    msec = times(time_str, time_str2, time_str3);
    log_file << left << setfill(' ') << setw(70) << " End URA " << " | " << time_str<<"-"<< time_str2<<":"<< time_str3 << ":" << right << setfill('0') << setw(6) <<  msec << endl;

    int n = size_file;
    bool verification = true;
    char *buffile = new char[n+1];
    buffile[n] = 0;
    char *buffile2 = new char[n+1];
    buffile2[n] = 0;

    unsigned int ci4 = 0;
    std::ifstream i1("DATA_FROM.txt"), i2("DATA.txt");
    if(!i1||!i2)
    {
        std::cerr<<"Files not found";
        return -1;
    }
    while(!i2.eof() && !i1.eof())
    {
        i1.get(buffile[ci4]);
        i2.get(buffile2[ci4]);

        if(buffile[ci4] != buffile2[ci4])
            verification = false;
        ci4++;
    }
    delete [] buffile;
    delete [] buffile2;
    ci4=0;
    cout << "\n!\n";
    if(verification == true)
        cout << "Same files" << endl;
    else
        cout << "Not same files" << endl;

    i1.close();
    i2.close();

    log_file.close();
}