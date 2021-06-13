build:
	gcc process_generator.c -o pg
	gcc clk.c -o clk.out
	gcc scheduler.c -o scheduler.out
	gcc process.c -o process.out
	gcc test_generator.c -o tg

clean:
	rm -f *.out  processes.txt

all: clean build

run:
	./process_generator.out
