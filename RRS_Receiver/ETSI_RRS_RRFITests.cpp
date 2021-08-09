/* File History
* *****************  Version 2  *****************
* User: Eegorov      Date: 30.07.21   Time: 11:00
*
*/

#define BOARD 0         // 1 - Activate; 0 - Inactive
#define BUF_SIZE 8200
#define Num_of_samples 8160
#define MaxSizeUDP 65280

int DATAURA = 2617;
#include <boost/test/unit_test.hpp>
#include "out/WLANT/MyTransmitter.cpp"
#include <fstream>
#include <cstdio>
#include <sys/socket.h>
#include <netinet/in.h>

#if BOARD == 1
#include "ETSI_RRS_USRP_Device.h"
#include "ETSI_RRS_RRFI.h"
struct TestFixture {
    ETSI_RRS_USRP_Device usrpDevice;
    ETSI_RRS_RRFI rrfi;
    TestFixture() : rrfi(usrpDevice) {}
    ~TestFixture() = default;
};
TestFixture a; // для работы с платой
        const double freq = 2480000000;
        const double bw = 35000000;
        const double sr = 2000000;
        const double gain = 50;

#endif

int main(int argc, char* argv[])
{
    msec = times(time_str, time_str2, time_str3);
    log_file << left << setfill(' ') << setw(89) << " Start Receiver " << " | " << time_str<<"-"<< time_str2<<":"<< time_str3 << ":" << right << setfill('0') << setw(6) <<  msec << endl;
    log_file << "Board IN_ACTIVATE" << endl;
    cout << " Receiver\n";

    if(argc != 1)
    {cout << "Incorrect Receiver using!\n";
        msec = times(time_str, time_str2, time_str3);
        log_file << left << setfill(' ') << setw(89) << " End Receiver " << " | " << time_str<<"-"<< time_str2<<":"<< time_str3 << ":" << right << setfill('0') << setw(6) <<  msec << endl;
        cout << "End Receiver\n";
        return -1;}

    char *param = "Configuration.cfg";
    int packet_size=0, data_rate=0;
    ReadInt("General", "packet_size", param, "decimal", &packet_size);
    ReadInt("General", "data_rate", param, "decimal", &data_rate);
    cout << "packet_size = " << packet_size << endl;
    cout << "data_rate = " << data_rate << endl;

    if ( data_rate == 6 )
    {
        DATAURA = packet_size;
        if(packet_size > 288)
            DATAURA = 288;
    }
    else if ( data_rate == 9 )
    {
        DATAURA = packet_size;
        if(packet_size > 433)
            DATAURA = 433;
    }
    else if ( data_rate == 12 )
    {
        DATAURA = packet_size;
        if(packet_size > 579)
            DATAURA = 579;
    }
    else if ( data_rate == 18 )
    {
        DATAURA = packet_size;
        if(packet_size > 870)
            DATAURA = 870;
    }
    else if ( data_rate == 24 )
    {
        DATAURA = packet_size;
        if(packet_size > 1161)
            DATAURA = 1161;
    }
    else if ( data_rate == 36 )
    {
        DATAURA = packet_size;
        if(packet_size > 1743)
            DATAURA = 1743;
    }
    else if ( data_rate == 48 )
    {
        DATAURA = packet_size;
        if(packet_size > 2325)
            DATAURA = 2325;
    }
    else if ( data_rate == 54 )
    {
        DATAURA = packet_size;
        if(packet_size > 2616)
            DATAURA = 2616;
    }
    else
    {
        std::cout << "Invalid data_rate in Configuration.cfg" << std::endl;
        exit(1);
    }

    int sock_app;
    int sock_test_object2;
    struct sockaddr_in addrapp;
    struct sockaddr_in addr_transmitter, cliAddr_transmitter;
    socklen_t cliAddrLentransmitter;

    sock_app = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    sock_test_object2 = socket(AF_INET, SOCK_DGRAM, 0);

    if(sock_app < 0)
        {perror("socket_App");return -2;}
    if(sock_test_object2 < 0)
        {perror("socket_Test_Object2");return -3;}

    addrapp.sin_family = AF_INET;
    addrapp.sin_addr.s_addr = htonl(INADDR_ANY);
    unsigned int PORT_PRIL = 5566;
    addrapp.sin_port = htons(PORT_PRIL);

    addr_transmitter.sin_family = AF_INET;
    addr_transmitter.sin_addr.s_addr = htonl(INADDR_ANY);
    unsigned int PORT_TRANSMITTER;
    for(PORT_TRANSMITTER = 4455; PORT_TRANSMITTER <= 4555; PORT_TRANSMITTER++) //Free ports checking
    {
        addr_transmitter.sin_port = htons(PORT_TRANSMITTER);
        if(bind(sock_test_object2, (struct sockaddr*)&addr_transmitter, sizeof(addr_transmitter)) == 0)
            break;
    }

    std::ofstream fout("Pids_Storage.txt", std::ios_base::out);
    fout << getpid(); //Writing the PID...
    fout << "Port for Application: ";
    std::cout << "Port for Application: " << PORT_PRIL << std::endl;
    fout << PORT_PRIL << "\n"; // Му and its port number to file!
    fout << "Port for Test_Object2: ";
    std::cout << "Port for Test_Object2: " << PORT_TRANSMITTER << std::endl;
    fout << PORT_TRANSMITTER; // Му and its port number to file!
    fout.close();

    char file_name[6];
    sprintf(file_name, "%d", getpid());

    char path_to_file[100];
    strcpy(path_to_file, "../");
    strcat(path_to_file, file_name);
    strcat(path_to_file, ".txt");
    long int readStatus;
    cliAddrLentransmitter = sizeof(cliAddr_transmitter);

    char bufout[BUF_SIZE];
    memset(bufout, 0, BUF_SIZE);
    std::complex<float> Mybuff_from[Num_of_samples];

    while(true)
    {
        cout << "Waiting data in Transmitter . . .\n";

        /*auto *Mybuff_from = new std::complex<float> [(1 << MaxSamplingPower) *
                                                     (PreambleLength + MaxNumOfSymbols *
                                                                       (CyclicPrefixLength + NumOfSubcarriers))];*/

        msec = times(time_str, time_str2, time_str3);
        log_file << left << setfill(' ') << setw(89) << " Start time of waiting for receiving data from Test Object 2 " << " | " << time_str<<"-"<< time_str2<<":"<< time_str3 << ":" << right << setfill('0') << setw(6) <<  msec << endl;

        readStatus = recvfrom(sock_test_object2, Mybuff_from, MaxSizeUDP, 0,(struct sockaddr *) &cliAddr_transmitter, &cliAddrLentransmitter); // Му While there is some data in the socket...
        if(readStatus > 0 )
        {
            msec = times(time_str, time_str2, time_str3);
            log_file << left << setfill(' ') << setw(89) << " Finish time of waiting for receiving data from Test Object 2 " << " | " << time_str<<"-"<< time_str2<<":"<< time_str3 << ":" << right << setfill('0') << setw(6) <<  msec << endl;

            std::cout << "sizeof input data = " << sizeof Mybuff_from <<  std::endl;
            std::cout << "size readStatus = " << readStatus <<  std::endl;

            cout << " Send data...\n";
            std::vector<std::complex<float>> buff_from(Num_of_samples);


            std::cout << "Sizeof buff_from = " << sizeof buff_from <<  std::endl;

            std::cout << "\nRecvfrom\n";
            for (int i = 0; i < Num_of_samples; i++) {
                buff_from[i] = Mybuff_from[i];
            }
            #pragma clang diagnostic push
            #pragma clang diagnostic ignored "-Wwritable-strings"
            char *file = "Configuration.cfg";
            #pragma clang diagnostic pop

            cout << " Open Configuration file...\n";
            std::vector<std::complex<float>> buff(MaxPacketSize);

            cout << "Num_of_samples_constanta = " << Num_of_samples << endl;
            #if BOARD == 1
            buff2 = a.rrfi.rx_to_buff(MaxPacketSize, 30, num_of_samples); // РАБОТАЕТ
            int sizebuff2 = buff2.size();
            FILE *fil = NULL; fil = fopen("reseived1.bin", "wb"); fclose(fil);
            //MyReceiver(buff2, num_of_samples, file);   // Для платы
            MyReceiver(buff2, num_of_samples, file, bufout);   // Для платы + Test2
            #else
            //MyReceiver1(buff, Num_of_samples, file, bufout);      //Inactive board
            MyReceiver(buff_from, Num_of_samples, file, bufout);      //Inactive board + Test2
            #endif

            cout << "\n-----------------------------------------------------------------------\n";

            for (int i = 0; i<DATAURA; i++)
            {
                std::cout << bufout[i];
                if(bufout[i] == '\0')
                    break;
            }
            cout << " \nData is taken.\n\n\n\n\n";
        }
        msec = times(time_str, time_str2, time_str3);
        log_file << left << setfill(' ') << setw(89) << " Start send data to URA "<<" | " << time_str<<"-"<< time_str2<<":"<< time_str3 << ":" << right << setfill('0') << setw(6) <<  msec << endl;

        if (sendto(sock_app, bufout, DATAURA, 0, (struct sockaddr*)&addrapp, sizeof(addrapp)) < 0)
        {
            perror("sending error1...\n");
            exit(-1);
        }
        msec = times(time_str, time_str2, time_str3);
        log_file << left << setfill(' ') << setw(89) << " Finish send data to URA "<<" | " << time_str<<"-"<< time_str2<<":"<< time_str3 << ":" << right << setfill('0') << setw(6) <<  msec << endl;

        cout << " \nData Sand.\n";
        msec = times(time_str, time_str2, time_str3);
        log_file << left << setfill(' ') << setw(89) << " End Receiver " << " | " << time_str<<"-"<< time_str2<<":"<< time_str3 << ":" << right << setfill('0') << setw(6) <<  msec << endl;
        //log_file.close();
        //break;
    }
}