cc = clang++
link = clang++
copts = -c -O3 -std=c++11 -stdlib=libc++ -I/usr/local/Cellar/libmodbus/3.0.3/include/modbus/
lopts = -lmodbus -std=c++11 -stdlib=libc++

bin = modtest
objects = main.o 

$(bin): $(objects)
	$(link) $(lopts) -o $(bin) $(objects)

main.o: main.cpp
	$(cc) $(copts) main.cpp

clean:
	rm $(objects)
	rm $(bin)