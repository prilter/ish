build:
	g++ src/*.cc -c -lreadline
	mkdir bin bin/debug bin/release bin/objs -p
	rm -f bin/objs/*
	mv *.o bin/objs
	g++ bin/objs/*.o -o bin/debug/ish -lreadline

debug:
	g++ -Werror -Wall src/*.cc -c -lreadline -g
	mkdir bin bin/debug bin/release bin/objs -p
	rm -f bin/objs/*
	mv *.o bin/objs
	g++ -Werror -Wall bin/objs/*.o -o bin/debug/ish -lreadline -g

release:
	g++ -O3 -flto src/*.cc -c -lreadline
	mkdir bin bin/debug bin/release bin/objs -p
	rm -f bin/objs/*
	mv *.o bin/objs
	g++ bin/objs/*.o -o bin/release/ish -lreadline

run: build
	./bin/debug/ish


