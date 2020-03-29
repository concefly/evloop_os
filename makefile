test: test/main
	./test/main

test/main: os/*.c test/*.c test/lib/*.c
	$(CC) $^ -o test/main 

clean:
	rm test/main

.PHONY: test clean