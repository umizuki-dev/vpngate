trim: trim.cpp
	g++ trim.cpp -o trim -Wall -Werror -std=c++20 -O2
clean: 
	rm trim
