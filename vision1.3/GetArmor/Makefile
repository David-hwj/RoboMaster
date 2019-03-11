all : 
	
	g++ -c com.cpp -o com.o -I ./ -w
	g++ -c getarmor_rgb.cpp -o getarmor_rgb.o -I ./ -w
	g++ -c getarmor_hsv.cpp -o getarmor_hsv.o -I ./ -w
	g++ -c rgb_and_hsv.cpp -o rgb_and_hsv.o -I ./ -w
	g++ -c main.cpp -o main.o -I ./ -w
	sudo g++ -o main main.o com.o getarmor_rgb.o getarmor_hsv.o rgb_and_hsv.o `pkg-config --cflags --libs opencv` -w -I ./
	./main
#getarmor_hsv.cpp:287:42: error: ‘to_string’ was not declared in this scope
#         string Numbers1 = to_string(index);


