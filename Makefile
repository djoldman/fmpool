.PHONY: all perftest functest

all: functest perftest example

perftest:
	cd perftest; make

functest:
	cd functest; make

clean:
	rm -fr example example.o
	cd perftest; make clean
	cd functest; make clean
