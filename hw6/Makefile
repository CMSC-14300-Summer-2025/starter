CC      = clang
CFLAGS  += -gdwarf-4 -Wall -Wextra -pedantic -std=gnu99 -O2 -pthread
LDFLAGS += -gdwarf-4 -O2 -std=gnu99 -pthread

all: simulate simulate_threaded

simulate: simulate.o
	$(CC) $(LDFLAGS) -o $@ $^

simulate_threaded: simulate_threaded.o
	$(CC) $(LDFLAGS) -o $@ $^

table-test: table.o array-list.o linked-list.o table-test.o tests.o hash.o
	$(CC) $(LDFLAGS) -o $@ $^

export LSAN_OPTIONS := suppressions=memcheck.supp,print_suppressions=0
export ASAN_OPTIONS := detect_leaks=1
export MallocNanoZone := 0

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

.PHONY: clean
clean:
	rm -rf *.o simulate simulate_threaded

