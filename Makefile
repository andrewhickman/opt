OBJECTS := $(patsubst source/*.c, build/*.o, $(wildcard source/*.c))
FLAGS   := -std=c11 -Wall -Wextra -pedantic -g -fmax-errors=3

all: $(OBJECTS)
	gcc $(FLAGS) -I include -o bin $(OBJECTS) 

build/%.o: source/%.c
	gcc $(FLAGS) -I include -c -o $@ $<

.PHONY: clean test
clean:
	rm -rf build/*.o
