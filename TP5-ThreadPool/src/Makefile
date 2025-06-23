CXX      := g++
CXXFLAGS := -std=c++17 -pthread -Wall -Wextra -g

# Archivos fuente comunes
SRCS     := thread-pool.cc
SEMAPHORE := Semaphore.cc

# Archivos principales para cada test
MAIN_TESTS := main.cc tptest.cc tpcustomtest.cc test_custom1.cc

.PHONY: all test clean

all: threadpool tptest customtest run_tests

threadpool: $(SRCS) $(SEMAPHORE) main.cc
	$(CXX) $(CXXFLAGS) -o $@ $(SRCS) $(SEMAPHORE) main.cc

tptest: $(SRCS) $(SEMAPHORE) tptest.cc
	$(CXX) $(CXXFLAGS) -o $@ $(SRCS) $(SEMAPHORE) tptest.cc

customtest: $(SRCS) $(SEMAPHORE) tpcustomtest.cc
	$(CXX) $(CXXFLAGS) -o $@ $(SRCS) $(SEMAPHORE) tpcustomtest.cc

run_tests: $(SRCS) $(SEMAPHORE) test_custom1.cc
	$(CXX) $(CXXFLAGS) -o $@ $(SRCS) $(SEMAPHORE) test_custom1.cc

test: run_tests
	./run_tests

clean:
	rm -f threadpool tptest customtest run_tests
