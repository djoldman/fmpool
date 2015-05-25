.PHONY: all functest perftest example

all: functest perftest example

functest:
	cd test/functest; make

perftest:
	cd test/perftest; make

example:
	cd test/example; make

clean:
	cd test/functest; make clean
	cd test/perftest; make clean
	cd test/example; make clean
