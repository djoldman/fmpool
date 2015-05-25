.PHONY: all perftest

all: perftest example

perftest:
	cd perftest; make

clean:
	rm -fr example example.o
	cd perftest; make clean
