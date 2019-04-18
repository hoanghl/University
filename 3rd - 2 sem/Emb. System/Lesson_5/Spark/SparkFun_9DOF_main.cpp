/******************************************************************************
SparkFun_9DOF_Edison_Block_Example.cpp
Example code for the 9DOF Edison Block
14 Jul 2015 by Mike Hord
https://github.com/sparkfun/SparkFun_9DOF_Block_for_Edison_CPP_Library

Demonstrates the major functionality of the SparkFun 9DOF block for Edison.

** Supports only I2C connection! **

Development environment specifics:
    Code developed in Intel's Eclipse IOT-DK
    This code requires the Intel mraa library to function; for more
    information see https://github.com/intel-iot-devkit/mraa

This code is beerware; if you see me (or any other SparkFun employee) at the
local, and you've found our code helpful, please buy us a round!

Distributed as-is; no warranty is given.
******************************************************************************/

#include "mraa.hpp"

#include <iostream>
#include <unistd.h>
#include <cmath>

#include "SFE_LSM9DS0.h"

#define     EPSILON     0.05
#define     DELTA       10
#define     GAMMA       5

#define     PI          3.14159265
#define     TIME        0.01

int usleep_time = (int)(TIME * 1000000);

using namespace std;

enum MOTION {STILL, WALK, SIT, STAND} motion;

enum MOTION getMotion(float X, float Y, float Z, float Xp, float Yp, float Zp);

float gyroX = 0;
float gyroY = 0;
float gyroZ = 0;


int main()
{
    float angleX = 0;
    float angleY = 0;
    float angleZ = 0;

    float prev_angleX = 0;
    float prev_angleY = 85;
    float prev_angleZ = 0;

    int dt = 0;

    LSM9DS0 *imu;
    imu = new LSM9DS0(0x6B, 0x1D);
    // The begin() function sets up some basic parameters and turns the device
    // on; you may not need to do more than call it. It also returns the "whoami"
    // registers from the chip. If all is good, the return value here should be
    //    0x49d4. Here are the initial settings from this function:
    //    Gyro scale:                245 deg/sec max
    //    Xl scale:                    4g max
    //    Mag scale:                 2 Gauss max
    //    Gyro sample rate:    95Hz
    //    Xl sample rate:        100Hz
    //    Mag sample rate:     100Hz
    // These can be changed either by calling appropriate functions or by
    //    pasing parameters to the begin() function. There are named constants in
    //    the .h file for all scales and data rates; I won't reproduce them here.
    //    Here's the list of fuctions to set the rates/scale:
    //    setMagScale(mag_scale mScl)            setMagODR(mag_odr mRate)
    //    setGyroScale(gyro_scale gScl)        setGyroODR(gyro_odr gRate)
    //    setAccelScale(accel_scale aScl)    setGyroODR(accel_odr aRate)
    // If you want to make these changes at the point of calling begin, here's
    //    the prototype for that function showing the order to pass things:
    //    begin(gyro_scale gScl, accel_scale aScl, mag_scale mScl, 
	//				gyro_odr gODR, accel_odr aODR, mag_odr mODR)
    uint16_t imuResult = imu->begin();
    cout<<hex<<"Chip ID: 0x"<<imuResult<<dec<<" (should be 0x49d4)"<<endl;

    bool newAccelData = false;
    bool newMagData = false;
    bool newGyroData = false;
    bool overflow = false;

    // Loop and report data
    while (1)
    {            
        // First, let's make sure we're collecting up-to-date information. The
        //    sensors are sampling at 100Hz (for the accelerometer, magnetometer, and
        //    temp) and 95Hz (for the gyro), and we could easily do a bunch of
        //    crap within that ~10ms sampling period.
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
                newMagData = imu->newMData(); // Temp data is collected at the same
                                                                            //    rate as magnetometer data.
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

        // Print the unscaled 16-bit signed values.
        // cout<<"-------------------------------------"<<endl;
        // cout<<"Gyro x: "<<imu->gx<<endl;
        // cout<<"Gyro y: "<<imu->gy<<endl;
        // cout<<"Gyro z: "<<imu->gz<<endl;
        // cout<<"Accel x: "<<imu->ax<<endl;
        // cout<<"Accel y: "<<imu->ay<<endl;
        // cout<<"Accel z: "<<imu->az<<endl;
        // cout<<"Mag x: "<<imu->mx<<endl;
        // cout<<"Mag y: "<<imu->my<<endl;
        // cout<<"Mag z: "<<imu->mz<<endl;
        // cout<<"Temp: "<<imu->temperature<<endl;
        // cout<<"-------------------------------------"<<endl;

        // Print the "real" values in more human comprehensible units.

        float acceX = imu->calcAccel(imu->ax);
        float acceY = imu->calcAccel(imu->ay);
        float acceZ = imu->calcAccel(imu->az);
        gyroX = imu->calcGyro(imu->gx);
        gyroY = imu->calcGyro(imu->gy);
        gyroZ = imu->calcGyro(imu->gz);

        //cout<<"-------------------------------------"<<endl;
        // cout<<"Gyro x: "  << gyroX << endl;
        // cout<<"Gyro y: "  << gyroY << endl;
        // cout<<"Gyro z: "  << gyroZ << endl;

        // cout<<"Accel x: " << acceX <<" g" << endl;
        // cout<<"Accel y: " << acceY <<" g" << endl;
        // cout<<"Accel z: " << acceZ <<" g" << endl;
        
        float thetaX = atan2 (acceY, sqrt(acceX*acceX + acceZ*acceZ)) * 180 / PI;
        float thetaY = atan2 (acceZ, sqrt(acceY*acceY + acceX*acceX)) * 180 / PI;
        float thetaZ = atan2 (acceX, sqrt(acceZ*acceZ + acceY*acceY)) * 180 / PI;
        angleX = 0.98 * (angleX + gyroX * TIME * 5) + 0.02 * (thetaX);
        angleY = 0.98 * (angleY + gyroY * TIME * 5) + 0.02 * (thetaY);
        angleZ = 0.98 * (angleX + gyroZ * TIME * 5) + 0.02 * (thetaZ);

        if (dt++ == 5){
            switch(getMotion(angleX, angleY, angleZ, prev_angleX, prev_angleY, prev_angleZ)) {
                case SIT: {
                    cout << "                                SIT" << endl;
                    break;
                }
                case STAND: {
                    cout << "                                STAND" << endl;
                    break;
                }
                case WALK: {
                    cout << "                                WALk" << endl;
                    break;
                }
                default: {}
            }
            cout<<"-------------------------------------"<<endl;
            cout << "angle X = " << angleX << endl;
            cout << "angle Y = " << angleY << endl;
            cout << "angle Z = " << angleZ << endl;

            dt = 0;
        }
        
        // cout<<"Mag x: "<<imu->calcMag(imu->mx)<<" Gauss"<<endl;
        // cout<<"Mag y: "<<imu->calcMag(imu->my)<<" Gauss"<<endl;
        // cout<<"Mag z: "<<imu->calcMag(imu->mz)<<" Gauss"<<endl;
        // Temp conversion is left as an example to the reader, as it requires a
        //    good deal of device- and system-specific calibration. The on-board
        //    temp sensor is probably best not used if local temp data is required!
        // cout<<"-------------------------------------"<<endl;

        usleep(usleep_time);
    }

	return MRAA_SUCCESS;
}


bool        diff(float number, float range) {
    if (  number - (range - DELTA) - EPSILON >= 0
       && (range + DELTA) - number - EPSILON >= 0)
       return true;
    else
        return false;
}

float delta(float cur, float prev) {
    return cur - prev;
}

// **************************************************************
enum MOTION getMotion(float X, float Y, float Z, float Xp, float Yp, float Zp) {
    // SIT
    if (diff(Y, 83) && diff(X, 0) && diff(Z, 0)
        && delta(Y, Yp) <= GAMMA && delta(X, Xp) <= GAMMA && delta(X, Xp) <= GAMMA)
        return SIT;

    // SIT
    if (diff(Y, 0) && diff(X, 0) && diff(Z, 0)
        && delta(Y, Yp) <= GAMMA && delta(X, Xp) <= GAMMA && delta(X, Xp) <= GAMMA)
        return STAND;

    // case 1 of WALK
    // if ((diff(Y, 10) || diff(Y, -10)) && diff(X, 0) && diff(Z, 0)
    //     && delta(Y, Yp) >= GAMMA*2 && delta(X, Xp) >= GAMMA*2 && delta(X, Xp) >= GAMMA*2)
    //     return WALK;

    // case 2 of WALK
    if (gyroY >= 200 || gyroY <= -200)
        return WALK;

    
    return STILL;

}
