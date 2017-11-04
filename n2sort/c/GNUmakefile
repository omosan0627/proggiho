.PHONY: clean

n2sort: n2sort.c
	gcc n2sort.c -o n2sort -O2 -g -Wall -Wno-deprecated-declarations -lssl -lcrypto

clean:
	rm -f n2sort *.o
	rm -rf *.dSYM
