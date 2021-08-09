#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <complex>
#include <vector>
#include <iomanip>
#include <cassert>
#include "times.h"
#include "consts.h"

#define MaxPacketSize 65280
using namespace std;

std::ofstream log_file("log_file_Test_Object_2.txt", std::ios_base::out);
long msec = 0;
char time_str[sizeof("1900-01-01 23:59:59")];
char time_str2[sizeof("1900-01-01 23:59:59")];
char time_str3[sizeof("1900-01-01 23:59:59")];

std::ifstream test_file("Test_signal.txt", std::ios_base::in);
std::ifstream in_file("Vector_complex_file.txt", std::ios_base::in);
std::ofstream vector_complex("Vector_complex_file.txt", std::ios_base::out);
int flag = 0, Ierror = 0;

#define Num_of_samples 8160

void Check(std::complex<float> Mybuff_from[])
{
    std::complex<float> Test_file[MaxPacketSize/8];
    std::complex<float> In_file[MaxPacketSize/8];

    for(unsigned int pop=0; pop<MaxPacketSize/8; pop++)
    {
        Test_file[pop].imag(0);
        Test_file[pop].real(0);
        In_file[pop].imag(0);
        In_file[pop].real(0);
    }

    std::string s1,s2;
    double Re_signal = 0, Re_test = 0;
    double Im_signal = 0, Im_test = 0;
    if (!test_file) {
        std::cout << "ERROR OPEN FILE1" << std::endl;
    }
    if (!in_file) {
        std::cout << "ERROR OPEN FILE2" << std::endl;
    }

    for(int i=0; i<Num_of_samples; i++)
    {
        test_file >> Test_file[i];
        in_file >> In_file[i];

        Re_test = Test_file[i].real();
        Re_signal = In_file[i].real();
        Im_test = floor(Test_file[i].imag()*1000000)/1000000;
        Im_signal = floor(In_file[i].imag()*1000000)/1000000;

        //std::cout << "Im_test = " << Im_test << " Im_signal = " << Im_signal << std::endl;
        //std::cout << "Re_test = " << Re_test << " Re_signal = " << Re_signal << std::endl;
        //std::cout << "Size Mybuff_from[i].imag() = " << sizeof(Mybuff_from[i].imag()) << " Size Test_file[i].imag() = " << sizeof(Test_file[i].imag()) << std::endl;

        msec = times(time_str, time_str2, time_str3);
        log_file << right << setfill(' ')<< setw(15) << "Test signal - " << right << setfill(' ')<< setw(15) << Test_file[i] << " = " << left << setfill(' ') << setw(15) << Mybuff_from[i] << setw(15) << " - Input signal        | "<< time_str<<"-"<< time_str2<<":"<< time_str3 << ":" << right << setfill('0') << setw(6) <<  msec;

        //if((Re_test == Re_signal) && (Im_test == Im_signal))
        if(Test_file[i] == In_file[i])
        {
            //std::cout << " All OK" << std::endl << std::endl;
            //log_file << " \t+\n";
            //std::cout << "\n I = " << i << std::endl;
            Test_file[i] = In_file[i] = 0;
        }
        else
        {
            msec = times(time_str, time_str2, time_str3);
            log_file<< endl << left << setfill(' ') << setw(70) << " Finish time for receiving data " << " | " << time_str<<"-"<< time_str2<<":"<< time_str3 << ":" << right << setfill('0') << setw(6) <<  msec << endl;

            log_file << " \n \n \t \t \t Error in " << i + 1 << " symbol\n";
            log_file << left << setfill(' ') << setw(70) << " End of the test " << " | " << time_str<<"-"<< time_str2<<":"<< time_str3 << ":" << right << setfill('0') << setw(6) <<  msec << endl;

            flag = -1;
            Ierror++;
            //break;
        }
        if(i==Num_of_samples - 1)
        {
            if(flag==0)
                std::cout << "\n Test part of signal was successful" << std::endl << std::endl;
            else
                std::cout << "\n Test part of signal ERROR" << std::endl << std::endl;

            msec = times(time_str, time_str2, time_str3);

            if(flag==0)
                log_file << "\n Test part of signal was successful" << std::endl;
            else
                log_file << "\n Test part of signal ERROR" << std::endl;

            log_file << left << setfill(' ') << setw(70) << " End of the test " << " | " << time_str<<"-"<< time_str2<<":"<< time_str3 << ":" << right << setfill('0') << setw(6) <<  msec << endl;

            flag = 1;
            std::cout << "\nSamples checked = " << i+1 << std::endl;
        }
    }
    std::cout << "\nTotal errors = " << Ierror << std::endl;
    //test_file.close();
    //in_file.close();
}

int main(int argc, char* argv[])
{
    msec = times(time_str, time_str2, time_str3);
    log_file << left << setfill(' ') << setw(70) << " Start Test Object 2 " << " | " << time_str<<"-"<< time_str2<<":"<< time_str3 << ":" << right << setfill('0') << setw(6) <<  msec << endl;

    if(argc != 2)
    {
        cout << "Incorrect Test Object 2 using! (argc != 2)\n";
        log_file << " Incorrect Test Object 2 using! (argc != 2) " << endl;
        msec = times(time_str, time_str2, time_str3);
        log_file << left << setfill(' ') << setw(70) << " End Test Object 2 " << " | " << time_str<<"-"<< time_str2<<":"<< time_str3 << ":" << right << setfill('0') << setw(6) <<  msec << endl;
        log_file.close();
        return -1;
    }
    int PORT_RECEIVER;
    sscanf(argv[1], "%d", &PORT_RECEIVER);

    int sock_receiver;
    struct sockaddr_in addr_receiver;
    sock_receiver = socket(AF_INET, SOCK_DGRAM, 0);

    if(sock_receiver < 0)
        {perror("socket_Receiver");return -4;}

    addr_receiver.sin_family = AF_INET;
    addr_receiver.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    addr_receiver.sin_port = htons(PORT_RECEIVER); //Destination port for transfer

    int sock;
    struct sockaddr_in addr, cliAddr;
    socklen_t cliAddrLen;

    std::complex<float> Mybuff_from[MaxPacketSize/8];
    std::complex<float> Mybuff_in[MaxPacketSize/8];
    std::vector<std::complex<float>> Mybuff_from_in(MaxPacketSize/8);

    std::cout << "sizeof Mybuff_from_in = " << sizeof Mybuff_from_in <<  std::endl;
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock < 0)
    {
        perror("socket");
        return -1;
    }

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    unsigned int port;
    for(port = 3555; port <= 3666; port++) //Free ports checking
    {
        addr.sin_port = htons(port);
        if(bind(sock, (struct sockaddr*)&addr, sizeof(addr)) == 0)
            std::cout << "OK" <<  std::endl;
            break;
    }

    std::ofstream fout("Pids_Storage.txt", std::ios_base::out);
    fout << getpid(); //Writing the PID...
    fout << "\n";
    std::cout << " Port for Transmitter: " << port << std::endl;
    fout << port; //and its port number to file!
    fout.close();
    std::clog << " Working Test Object 2\n";

    cliAddrLen = sizeof(cliAddr);
    long int readStatus;
    while(true)
    {
        msec = times(time_str, time_str2, time_str3);
        log_file << left << setfill(' ') << setw(70) << " Start time for receiving data " << " | " << time_str<<"-"<< time_str2<<":"<< time_str3 << ":" << right << setfill('0') << setw(6) <<  msec << endl;

        readStatus = recvfrom(sock , Mybuff_from , MaxPacketSize, 0 , (struct sockaddr*)&cliAddr, &cliAddrLen);

        msec = times(time_str, time_str2, time_str3);
        log_file << left << setfill(' ') << setw(70) << " Finish time for receiving data " << " | " << time_str<<"-"<< time_str2<<":"<< time_str3 << ":" << right << setfill('0') << setw(6) <<  msec << endl;

        if(readStatus > 0 )
        {
            std::cout << "sizeof input data = " << sizeof Mybuff_from<<  std::endl;
            std::cout << "size readStatus = " << readStatus <<  std::endl;

            for (int i = 0; i < Num_of_samples; i++)
            {
                Mybuff_from_in[i] = Mybuff_from[i];
                vector_complex << Mybuff_from_in[i];
                vector_complex << endl;
            }

            Check(Mybuff_from);         // Checking the input signal with the reference

            for (int i = 0; i < Num_of_samples; i++)
                Mybuff_in[i] = Mybuff_from[i];
        }
        if (readStatus < 0) {
            perror("reading error...\n");
            close(sock);
            exit(-1);
        }

        if(flag == 1)
        {
            msec = times(time_str, time_str2, time_str3);
            log_file << left << setfill(' ') << setw(70) << " Start time for sending data to the receiver " << " | " << time_str<<"-"<< time_str2<<":"<< time_str3 << ":" << right << setfill('0') << setw(6) <<  msec << endl;

            if (sendto(sock_receiver, Mybuff_in, MaxPacketSize, 0, (struct sockaddr *) &addr_receiver, sizeof(addr_receiver)) < 0) // к RRS_Receiver
            {
                perror("sending error...\n");
                exit(-1);
            }
            msec = times(time_str, time_str2, time_str3);
            log_file << left << setfill(' ') << setw(70) << " Finish time for sending data to the receiver " << " | " << time_str<<"-"<< time_str2<<":"<< time_str3 << ":" << right << setfill('0') << setw(6) <<  msec << endl;

            msec = times(time_str, time_str2, time_str3);
            log_file << left << setfill(' ') << setw(70) << " Start time for sending information message to the transmitter " << " | " << time_str<<"-"<< time_str2<<":"<< time_str3 << ":" << right << setfill('0') << setw(6) <<  msec << endl;

            if (sendto(sock, "Test OK", 10, 0, (struct sockaddr*)&cliAddr, sizeof(cliAddr)) < 0) // к RRS_Transmitter
            {
                perror("sending error2...\n");
                exit(-1);
            }
            msec = times(time_str, time_str2, time_str3);
            log_file << left << setfill(' ') << setw(70) << " Finish time for sending information message to the transmitter " << " | " << time_str<<"-"<< time_str2<<":"<< time_str3 << ":" << right << setfill('0') << setw(6) <<  msec << endl;
        }
        else if (flag == -1)
        {
            msec = times(time_str, time_str2, time_str3);
            log_file << left << setfill(' ') << setw(70) << " Start time for sending information message to the transmitter " << " | " << time_str<<"-"<< time_str2<<":"<< time_str3 << ":" << right << setfill('0') << setw(6) <<  msec << endl;

            if (sendto(sock, "Error", 10, 0, (struct sockaddr *) &cliAddr, sizeof(cliAddr)) < 0)    // к RRS_Transmitter
            {
                perror("sending error3...\n");
                exit(-1);
            }
            msec = times(time_str, time_str2, time_str3);
            log_file << left << setfill(' ') << setw(70) << " Finish time for sending information message to the transmitter " << " | " << time_str<<"-"<< time_str2<<":"<< time_str3 << ":" << right << setfill('0') << setw(6) <<  msec << endl;
        }
        flag = 0;
        msec = times(time_str, time_str2, time_str3);
        log_file << left << setfill(' ') << setw(70) << " Finish Test Object 2 " << " | " << time_str<<"-"<< time_str2<<":"<< time_str3 << ":" << right << setfill('0') << setw(6) <<  msec << endl;
        //log_file.close();
        //break;
    }
}