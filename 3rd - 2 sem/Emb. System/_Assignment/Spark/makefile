CC=g++
CFLAGS=-g -c -Wall

all: SFE_LSM9DS0.o SparkFun_9DOF_main.o
	$(CC) -o EdisonV3 SFE_LSM9DS0.o SparkFun_9DOF_main.o -lmraa

SFE_LSM9DS0.o: SFE_LSM9DS0.cpp SFE_LSM9DS0.h
	$(CC) $(CFLAGS) SFE_LSM9DS0.cpp -lmraa

SparkFun_9DOF_main.o: SparkFun_9DOF_main.cpp SFE_LSM9DS0.h
	$(CC) $(CFLAGS) SparkFun_9DOF_main.cpp -lmraa

clean:
	rm -rf *.o EdisonV3

CC=g++
CFLAGS=-g -c -Wall


all: snake
snake: main.o resources.o gpio_edison.o Edison_OLED.o spi_port_edison.o spi_device_edison.o
	$(CC) -g -o snake main.o resources.o gpio_edison.o Edison_OLED.o spi_port_edison.o spi_device_edison.o



gpio_edison.o: gpio/gpio_edison.cpp gpio/gpio_edison.h
	$(CC) $(CFLAGS) gpio/gpio_edison.cpp
Edison_OLED.o: oled/Edison_OLED.cpp oled/Edison_OLED.h spi/spi.h
	$(CC) $(CFLAGS) oled/Edison_OLED.cpp
spi_port_edison.o: spi/spi_port_edison.cpp
	$(CC) $(CFLAGS) spi/spi_port_edison.cpp
spi_device_edison.o: spi/spi_device_edison.cpp
	$(CC) $(CFLAGS) spi/spi_device_edison.cpp

clean:
	rm -rf *.o snake
