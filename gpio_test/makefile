
cc = g++
link = g++
copts = -c -I/usr/local/include -fPIC -O3
lopts = -L/usr/local/lib -lwiringPi

bin = gpio_test
objects = main.o

$(bin): $(objects)
	$(link) $(lopts) -o $(bin) $(objects)

main.o: main.cpp
	$(cc) $(copts) main.cpp

clean:
	rm $(objects)
	rm $(bin)
