
cc = g++
link = g++
copts = -c -I/usr/include/modbus -fPIC -O3
lopts = -L/usr/lib/ -lmodbus -lwiringPi

bin = pibus
objects = pibus.o

$(bin): $(objects)
	$(link) $(lopts) -o $(bin) $(objects)

pibus.o: pibus.cpp
	$(cc) $(copts) pibus.cpp

clean:
	rm $(objects)
	rm $(bin)
