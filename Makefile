all: trim generate

trim: trim.cpp
	g++ trim.cpp -o trim -Wall -Werror -std=c++20 -O2
generate: generate.cpp
	g++ generate.cpp -o generate -Wall -Werror -std=c++20 -O2
clean: 
	rm trim
	rm generate
	rm list.csv
	rm listTrimmed.csv
	rm -rf configs
