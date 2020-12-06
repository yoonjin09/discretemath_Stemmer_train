all:
	gcc -o stemmer -I../include stemmer.c ../lib/libstemmer.o 
	gcc -c predictor.c
	gcc predictor.o -o predictor
clean:
	rm stemmer predictor predictor.o ./alltext/*.txt ./result/*.txt
