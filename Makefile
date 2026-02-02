all: trim generate

trim: trim.cc
	g++ trim.cc -o trim -Wall -Werror -std=c++20 -O2
generate: generate.cc
	g++ generate.cc -o generate -Wall -Werror -std=c++20 -O2
clean: 
	rm trim
	rm generate
	rm list.csv
	rm listTrimmed.csv
	rm -rf configs
