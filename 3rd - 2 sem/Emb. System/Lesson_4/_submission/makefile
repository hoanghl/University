CC=g++
CFLAGS=-g -c -Wall


all: snake
snake: main.o resources.o gpio_edison.o Edison_OLED.o spi_port_edison.o spi_device_edison.o
	$(CC) -g -o snake main.o resources.o gpio_edison.o Edison_OLED.o spi_port_edison.o spi_device_edison.o


resources.o: resources.cpp resources.hpp Draw.hpp $(SOURCES)
	$(CC) $(CFLAGS) resources.cpp $(SOURCES)
gpio_edison.o: gpio/gpio_edison.cpp gpio/gpio_edison.h
	$(CC) $(CFLAGS) gpio/gpio_edison.cpp
Edison_OLED.o: oled/Edison_OLED.cpp oled/Edison_OLED.h spi/spi.h
	$(CC) $(CFLAGS) oled/Edison_OLED.cpp
spi_port_edison.o: spi/spi_port_edison.cpp
	$(CC) $(CFLAGS) spi/spi_port_edison.cpp
spi_device_edison.o: spi/spi_device_edison.cpp
	$(CC) $(CFLAGS) spi/spi_device_edison.cpp
main.o: main.cpp resources.hpp
	$(CC) $(CFLAGS) main.cpp

clean:
	rm -rf *.o snake
