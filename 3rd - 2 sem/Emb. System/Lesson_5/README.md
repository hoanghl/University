In this lab, I have to implement a small program that detects basic human activities: **STANDING**, **WALKING** and **SITTING**

I use Compliment filter to combine values obtained by gyroscope and accelerometer to come up **angle**

* STANDING and SITTING *
    I detect by determine fixed value of **angle** as in **STANDING** and **SITTING** state
* WALKING *
    To this ends, the solution I proposed is detecting fixed value of gyroscope without using **angle**
    This problem needs more deep research to figure out better solution
