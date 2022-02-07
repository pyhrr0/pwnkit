all:
	gcc -o pwnkit.so -fPIC -shared pwnkit.c -Wl,-e,main

clean:
	rm -rf GCONV_PATH=. pwnkit pwnkit.so

test: all
	./pwnkit.so
