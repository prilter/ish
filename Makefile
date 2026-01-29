comp:
	g++ src/*.cc -c -lreadline
	mkdir bin bin/debug bin/release bin/objs -p
	rm bin/objs/*
	mv *.o bin/objs
	g++ bin/objs/*.o -o bin/debug/ish -lreadline

release:
	g++ -O3 -flto src/*.cc -c -lreadline
	mkdir bin bin/debug bin/release bin/objs -p
	rm bin/objs/*
	mv *.o bin/objs
	g++ bin/objs/*.o -o bin/release/ish -lreadline

run: comp
	./bin/debug/ish


