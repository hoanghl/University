#include <iostream>
#include <mraa.hpp>

using namespace std ;

int main (int argc, char** argv)
{
    mraa::Uart* uart ;
    try
    {
        uart = new mraa::Uart("/dev/ttyMFD1") ;
    } catch (std::exception& e) {
        cout << " Error while setting up raw UART\n" ;
        std::terminate() ;
    }
    
    if (uart−>setBaudRate(115200) != mraa::SUCCESS)
    {
        cout << " Error while setting baudrate \n" ;
    }
    if (uart−>setMode (8, mraa::UART_PARITY_NONE, 1) != mraa::SUCCESS)
    {
        cout << " Error while settin g mode\n" ;
    }
    if (uart−>setFlowcontrol(false, false ) != mraa::SUCCESS)
    {
        cout << " Error while setting flow control \n" ;
    }
    
    uart−>flush() ;
    uart−>writeStr("Hello") ;
    delete uart ;
}
