/* File History
* *****************  Version 1  *****************
* User: Eegorov      Date: 30.07.21   Time: 11:00
*
*/

#define BOARD 0         // 1 - Activate; 0 - Inactive
#define MaxSizeUDP 65280
int DATAURA = 2617;

#include <boost/test/unit_test.hpp>
#include "out/WLANT/MyTransmitter.cpp"
#include <fstream>
#include <cstdio>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iomanip>
#include "out/WLANT/times.h"

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
    log_file << left << setfill(' ') << setw(89) << " Start Transmitter " << " | " << time_str<<"-"<< time_str2<<":"<< time_str3 << ":" << right << setfill('0') << setw(6) <<  msec << endl;
    log_file << "Board IN_ACTIVATE" << endl;
    cout << "Transmitter\n";

    if(argc != 2)
    {
        cout << "Incorrect Transmitter using!\n";
        msec = times(time_str, time_str2, time_str3);
        log_file << left << setfill(' ') << setw(89) << " End Transmitter "<<" | " << time_str<<"-"<< time_str2<<":"<< time_str3 << ":" << right << setfill('0') << setw(6) <<  msec << endl;
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

    int PORT_TEST2;
    sscanf(argv[1], "%d", &PORT_TEST2); //Port number translating from char* to int NOLINT(cert-err34-c)

    int sock1;
    int socktest2;
    struct sockaddr_in addr1, cliAddr1;
    struct sockaddr_in addrtest2;
    socklen_t cliAddrLen1;
    socklen_t cliAddrLentest2;
    char bufin[DATAURA];
    char bufout[10];
    sock1 = socket(AF_INET, SOCK_DGRAM, 0);
    socktest2 = socket(AF_INET, SOCK_DGRAM, 0);

    if(sock1 < 0)
        {perror("socket1");return -2;}
    if(socktest2 < 0)
        {perror("socketTestObject2");return -3;}

    addr1.sin_family = AF_INET;
    addr1.sin_addr.s_addr = htonl(INADDR_ANY);
    unsigned int PORT_TEST1;
    for(PORT_TEST1 = 3344; PORT_TEST1 <= 3444; PORT_TEST1++) //Free ports checking
    {
        addr1.sin_port = htons(PORT_TEST1);
        if(bind(sock1, (struct sockaddr*)&addr1, sizeof(addr1)) == 0)
            break;
    }

    addrtest2.sin_family = AF_INET;
    addrtest2.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    addrtest2.sin_port = htons(PORT_TEST2); //Destination port for transfer

    std::ofstream fout("Pids_Storage.txt", std::ios_base::out);
    fout << getpid(); //Writing the PID...
    fout << "Port for URA: ";
    std::cout << "Port for URA: " << PORT_TEST1 << std::endl;
    fout << PORT_TEST1; // Му and its port number to file!
    fout.close();

    char file_name[6];
    sprintf(file_name, "%d", getpid());

    char path_to_file[100];
    strcpy(path_to_file, "../");
    strcat(path_to_file, file_name);
    strcat(path_to_file, ".txt");
    long int readStatus;
    long int readStatus2;
    cliAddrLen1 = sizeof(cliAddr1);
    string okey="okey";
    unsigned int num_of_samples = 0;
    while(true)
    {
        cout << "Waiting data from URA . . .\n";

        msec = times(time_str, time_str2, time_str3);
        log_file << left << setfill(' ') << setw(89) << " Start time of waiting for receiving data from URA "<<" | " << time_str<<"-"<< time_str2<<":"<< time_str3 << ":" << right << setfill('0') << setw(6) <<  msec << endl;

        readStatus = recvfrom(sock1 , bufin , DATAURA, 0 , (struct sockaddr*)&cliAddr1, &cliAddrLen1); // Му While there is some data in the socket...

        msec = times(time_str, time_str2, time_str3);
        log_file << left << setfill(' ') << setw(89) << " Finish time of waiting for receiving data from URA "<<" | " << time_str<<"-"<< time_str2<<":"<< time_str3 << ":" << right << setfill('0') << setw(6) <<  msec << endl;

        if(readStatus > 0 )
        {
            std::cout << "size readStatus = " << readStatus <<  std::endl;
            cout << " Received data from URA.\n";

            std::vector<std::complex<float>> buff((1 << MaxSamplingPower) *
                                                  (PreambleLength + MaxNumOfSymbols *
                                                                    (CyclicPrefixLength + NumOfSubcarriers)));

            #pragma clang diagnostic push
            #pragma clang diagnostic ignored "-Wwritable-strings"
            char *fp = "Configuration.cfg";
            #pragma clang diagnostic pop

            cout << "Open Configuration file..." << endl;
            buff = MyTransmitter(fp, &num_of_samples, bufin);
            cout << "\n Num_of_samples = " << num_of_samples << endl;
            cout << "Buffer fulled" << endl;

            std::vector<std::complex<float>> buff2(MaxPacketSize);
            // было std::complex<float> вместо auto
            auto *Mybuff_to = new std::complex<float> [(1 << MaxSamplingPower) *
                                                                     (PreambleLength + MaxNumOfSymbols *
                                                                                       (CyclicPrefixLength + NumOfSubcarriers))];

            memset(Mybuff_to, 0, (1 << MaxSamplingPower) * (PreambleLength + MaxNumOfSymbols * (CyclicPrefixLength + NumOfSubcarriers)));
            auto *Mybuff_from = new std::complex<float> [(1 << MaxSamplingPower) *
                                                                      (PreambleLength + MaxNumOfSymbols *
                                                                                        (CyclicPrefixLength + NumOfSubcarriers))];
            #if BOARD == 1
            a.rrfi.tx_from_buff(buff, num_of_samples); // вот так
            msec = times(time_str, time_str2, time_str3);
            log_file << "Board ACTIVATE" << endl;
            //rrfi.tx_from_buff(buff, num_of_samples); // старая версия Леонид
            #endif
            cout << "Data sent " << endl;

            //[   Working Test Object 2
            std::ofstream fil("Vector_complex.txt", std::ios_base::out);
            std::ofstream filcomplex("Complex.txt", std::ios_base::out);
            FILE * filcomplex2;
            filcomplex2 = fopen ("Ald.txt","w");

            for (int i = 0; i < num_of_samples; i++)
            {
                Mybuff_to[i] = buff[i];
                fil << buff[i] << "\n";


                filcomplex <<  Mybuff_to[i].real() << "\n";
                filcomplex <<  buff[i].real() << "\n";
                filcomplex <<  Mybuff_to[i].imag() << "\n";
                filcomplex <<  buff[i].imag() << "\n";
                fprintf(filcomplex2, "%2.2f %2.2f \n", buff[i].imag(), buff[i].real()); //старая версия вывода
                fprintf(filcomplex2, "%2.2f \n", buff[i]); //старая версия вывода
            }
            fil.close();
            filcomplex.close();
            fclose (filcomplex2);
            cout << Mybuff_to[0] << endl;
            cout << "size Mybuff_to = " << (1 << MaxSamplingPower) *
                                           (PreambleLength + MaxNumOfSymbols *
                                                             (CyclicPrefixLength + NumOfSubcarriers)) << endl;

            // Для отправки 1 символа Mybuff_to[0] необходимо 8 байт
            msec = times(time_str, time_str2, time_str3);
            log_file << left << setfill(' ') << setw(89) << " Start send data to Test Object 2 " << " | "
            << time_str << "-" << time_str2 << ":" << time_str3 << ":" << right << setfill('0') << setw(6)
            << msec << endl;

            cout << "MaxPacketSize = " << MaxPacketSize << endl;

            if (sendto(socktest2, Mybuff_to, MaxSizeUDP, 0, (struct sockaddr *) &addrtest2, sizeof(addrtest2)) < 0) {
                perror("sending error3...\n");
                //close(socktest2);
                //exit(-1);
            }
            msec = times(time_str, time_str2, time_str3);
            log_file << left << setfill(' ') << setw(89) << " Finish send data to Test Object 2 " << " | " << time_str << "-" << time_str2 << ":" << time_str3 << ":" << right << setfill('0') << setw(6) << msec << endl;

            sleep(1);

            cliAddrLentest2 = sizeof(addrtest2);
            msec = times(time_str, time_str2, time_str3);
            log_file << left << setfill(' ') << setw(89) << " Start time of listening to the channel from Test Object 2 " << " | " << time_str << "-" << time_str2 << ":" << time_str3 << ":" << right << setfill('0') << setw(6) << msec << endl;

            readStatus2 = recvfrom(socktest2, bufout, 10, 0, (struct sockaddr *) &addrtest2, &cliAddrLentest2); // Му While there is some data in the socket...

            if (readStatus2 > 0) {
                msec = times(time_str, time_str2, time_str3);
                log_file << left << setfill(' ') << setw(89) << " Finish time response from Test Object 2 " << " | " << time_str << "-" << time_str2 << ":" << time_str3 << ":" << right << setfill('0') << setw(6) << msec << endl;
                    std::cout << " Message from Test Object2 -> " << bufout << std::endl;
                }
            //]*/

            delete[] Mybuff_to;
            delete[] Mybuff_from;
        }
        msec = times(time_str, time_str2, time_str3);
        log_file << left << setfill(' ') << setw(89) << " End Transmitter "<<" | " << time_str<<"-"<< time_str2<<":"<< time_str3 << ":" << right << setfill('0') << setw(6) <<  msec << endl;

        //log_file.close();
        //break;
    }
}