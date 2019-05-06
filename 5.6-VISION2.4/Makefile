all : 
	
	g++ -c COM_SEND.cpp -o COM_SEND.o -I ./ -w
	g++ -c REC_PART1.cpp -o REC_PART1.o -I ./ -w
	g++ -c REC_PART2.cpp -o REC_PART2.o -I ./ -w
	g++ -c main.cpp -o main.o -I ./ -w
	sudo g++ -o main main.o COM_SEND.o  REC_PART1.o REC_PART2.o `pkg-config --cflags --libs opencv` -w -I ./
	./main
#getarmor_hsv.cpp:287:42: error: ‘to_string’ was not declared in this scope
#         string Numbers1 = to_string(index);


