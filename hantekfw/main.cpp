#include <QCoreApplication>
#include <QFile>
#include <QDebug>
#include <QCommandLineParser>
#include "ht6022bx.h"
int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("hantefw");
    QCoreApplication::setApplicationVersion("1.0");
    unsigned int index = 1;//6022BL
    unsigned char* CH1 = 0;
    unsigned char* CH2 = 0;
    HT6022bx    *hantek     = new HT6022bx();
    printf("Return value of hantek->DeviceInit: %d\n", hantek->DeviceInit());
    //printf("Return value of hantek->deviceOpen: %d\n", hantek->deviceOpen(&device, index));
    printf("Return value of hantek->deviceOpen: %d\n", hantek->deviceOpen(index));

    unsigned char *CalValues;
    /*
    typedef enum
    {
        HT6022BX_32B   = 0x00000010, // 32 Bytes
        HT6022BX_64B   = 0x00000020, // 64 Bytes
        HT6022BX_128B  = 0x00000080 // 128 Bytes
    }HT6022BX_CVSizeTypeDef;
    */
    CalValues = (unsigned char*) malloc (sizeof (unsigned char)*HT6022BX_128B);
    /*
    unsigned char *getCalValues;
    getCalValues = (unsigned char*) malloc (sizeof (unsigned char)*HT6022BX_128B);
    printf("Return value of hantek->setCalValues: %d\n", hantek->getCalValues(&device, index, getCalValues,HT6022BX_128B));   //Channel 1 input range. Read HT6022.h for more information
    */
    printf("Return value of hantek->setXXX: %d\n", hantek->setXXX(index));
    //setXXX

    printf("Return value of hantek->setCalValues: %d\n", hantek->setCalValues(index, CalValues,HT6022BX_128B));   //Channel 1 input range. Read HT6022.h for more information
    /*
    typedef enum
    {
        HT6022BX_10V   = 0x01, // -5V    to 5V
        HT6022BX_5V    = 0x02, // -2.5V  to 2.5V
        HT6022BX_2V    = 0x05, // -1V    to 1V
        HT6022BX_1V    = 0x0A  // -500mv to 500mv
    }HT6022BX_IRTypeDef;
    */
    printf("Return value of hantek->setCH1IR: %d\n", hantek->setCH1IR(index, HT6022BX_1V));   //Channel 1 input range. Read HT6022.h for more information

    printf("Return value of hantek->setCH2IR: %d\n", hantek->setCH2IR(index, HT6022BX_1V));   //Channel 2 input range.
    /*
    typedef enum
    {
        HT6022BX_48MSa  = 0x30, // 48MSa per channel
        HT6022BX_16MSa  = 0x10, // 16MSa per channel
        HT6022BX_8MSa   = 0x08, // 8MSa per channel
        HT6022BX_4MSa   = 0x04, // 4MSa per channel
        HT6022BX_1MSa   = 0x01, // 1MSa per channel
        HT6022BX_500KSa = 0x32, // 500KSa per channel
        HT6022BX_200KSa = 0x14, // 200KSa per channel
        HT6022BX_100KSa = 0x0A  // 100KSa per channel
    }HT6022BX_SRTypeDef;
    */
    printf("Return value of hantek->setSR: %d\n", hantek->setSR(index, HT6022BX_1MSa));
    /*
    HT6022BX_1KB   = 0x00000400, //< 1024 Bytes
    HT6022BX_2KB   = 0x00000800, //< 2048 Bytes
    HT6022BX_4KB   = 0x00001000, //< 4096 Bytes
    HT6022BX_8KB   = 0x00002000, //< 8192 Bytes
    HT6022BX_16KB  = 0x00004000, //< 16384 Bytes
    HT6022BX_32KB  = 0x00008000, //< 32768 Bytes
    HT6022BX_64KB  = 0x00010000, //< 65536 Bytes
    HT6022BX_128KB = 0x00020000, //< 131072 Bytes
    HT6022BX_256KB = 0x00040000, //< 262144 Bytes
    HT6022BX_512KB = 0x00080000, //< 524288 Bytes
    HT6022BX_1MB   = 0x00100000  //< 1048576 Bytes
    */
    QFile fileCH1("ch1.dat");
    QFile fileCH2("ch2.dat");
    if ((!fileCH1.open(QIODevice::WriteOnly | QIODevice::Text)) || (!fileCH2.open(QIODevice::WriteOnly | QIODevice::Text)) )
    {
        return app.exec();
    }
    QTextStream outCH1(&fileCH1);
    QTextStream outCH2(&fileCH2);
    for (int j=0; j<1;j++)
    {
        qDebug("J=%u",j);
        CH1 = (unsigned char*) malloc (sizeof (unsigned char)*HT6022BX_8KB);
        CH2 = (unsigned char*) malloc (sizeof (unsigned char)*HT6022BX_8KB);
        size_t iterator = HT6022BX_8KB;
        printf("Return value of hantek->readData: %d\n", hantek->readData(index, CH1, CH2, HT6022BX_8KB, 0)); //read 1kb of data per channel
        for (size_t i = 0; i < iterator; i++)
        {
            outCH1  << CH1[i] << "\n";
            outCH2  << CH2[i] << "\n";
        }
        free(CH1);
        free(CH2);
    }
    //hantek->deviceClose(&device,index);
    hantek->deviceClose(0);
    hantek->DeviceExit();
    return app.exec();
}
