#include <iostream>
#include <mraa.hpp>
using namespace std ;
int main (int argc , char** argv)
{
    mraa::Gpio* gpio = new mraa::Gpio(14, true, true) ; // or Gpio ( 3 6 ) ;
    if (gpio == NULL)
    {
        return mraa::ERROR_UNSPECIFIED;
    }
    mraa::Result response = gpio−>dir(mraa::DIR_OUT) ;
    if (response != mraa::SUCCESS)
    {
        mraa::printError(response) ;
        return 1;
    }
    //Do some s t u f f . . .
    delete gpio;
    return 0;
}
