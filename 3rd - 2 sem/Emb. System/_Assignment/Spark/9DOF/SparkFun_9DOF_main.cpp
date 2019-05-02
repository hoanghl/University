#include "mraa.hpp"

#include <iostream>
#include <unistd.h>
#include <cmath>

#include "SFE_LSM9DS0.h"

int usleep_time = (int)(TIME * 1000000);

using namespace std;

int main()
{
    LSM9DS0 *imu;
    imu = new LSM9DS0(0x6B, 0x1D);

    uint16_t imuResult = imu->begin();
    cout<<hex<<"Chip ID: 0x"<<imuResult<<dec<<" (should be 0x49d4)"<<endl;

    bool newAccelData = false;
    bool newMagData = false;
    bool newGyroData = false;
    bool overflow = false;

    while (1)
    {            
        while ((newGyroData & newAccelData & newMagData) != true)
        {
            if (newAccelData != true)
            {
                newAccelData = imu->newXData();
            }
            if (newGyroData != true)
            {
                newGyroData = imu->newGData();
            }
            if (newMagData != true)
            {
                newMagData = imu->newMData();
            } 
        }

        newAccelData = false;
        newMagData = false;
        newGyroData = false;

        // Of course, we may care if an overflow occurred; we can check that
        //    easily enough from an internal register on the part. There are functions
        //    to check for overflow per device.
        overflow = imu->xDataOverflow() | 
                            imu->gDataOverflow() | 
                            imu->mDataOverflow();

        if (overflow)
        {
            cout<<"WARNING: DATA OVERFLOW!!!"<<endl;
        }

        // Calling these functions causes the data to be read from the IMU into
        //    10 16-bit signed integer public variables, as seen below. There is no
        //    automated check on whether the data is new; you need to do that
        //    manually as above. Also, there's no check on overflow, so you may miss
        //    a sample and not know it.
        imu->readAccel();
        imu->readMag();
        imu->readGyro();
        imu->readTemp();

        // Print the "real" values in more human comprehensible units.

        float acceX = imu->calcAccel(imu->ax);
        float acceY = imu->calcAccel(imu->ay);
        float acceZ = imu->calcAccel(imu->az);
        float gyroX = imu->calcGyro(imu->gx);
        float gyroY = imu->calcGyro(imu->gy);
        float gyroZ = imu->calcGyro(imu->gz);

        //cout<<"-------------------------------------"<<endl;
        // cout<<"Gyro x: "  << gyroX << endl;
        // cout<<"Gyro y: "  << gyroY << endl;
        // cout<<"Gyro z: "  << gyroZ << endl;

        // cout<<"Accel x: " << acceX <<" g" << endl;
        // cout<<"Accel y: " << acceY <<" g" << endl;
        // cout<<"Accel z: " << acceZ <<" g" << endl;

        // cout<<"Mag x: "<<imu->calcMag(imu->mx)<<" Gauss"<<endl;
        // cout<<"Mag y: "<<imu->calcMag(imu->my)<<" Gauss"<<endl;
        // cout<<"Mag z: "<<imu->calcMag(imu->mz)<<" Gauss"<<endl;
        
        float a = sqrt(acceX * acceX + acceY * acceY + acceZ * acceZ);

        
        

        usleep(usleep_time);
    }

	return MRAA_SUCCESS;
}



void run() {
    while (1) {
        switch (state)
        {
        case /* constant-expression */:
            /* code */
            break;
        
        default:
            break;
        }
    }
}