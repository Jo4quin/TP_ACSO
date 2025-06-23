CXX      := g++
CXXFLAGS := -std=c++11 -pthread -Wall -g
SRCS     := thread-pool.cc Semaphore.cc

.PHONY: all test clean

all: threadpool tptest customtest run_tests

threadpool: $(SRCS) main.cc
<TAB>$(CXX) $(CXXFLAGS) -o $@ $(SRCS) main.cc

tptest:    $(SRCS) tptest.cc
<TAB>$(CXX) $(CXXFLAGS) -o $@ $(SRCS) tptest.cc

customtest: $(SRCS) tpcustomtest.cc
<TAB>$(CXX) $(CXXFLAGS) -o $@ $(SRCS) tpcustomtest.cc

run_tests: thread-pool.cc test_custom1.cc
<TAB>$(CXX) -std=c++17 -pthread -g -O0 thread-pool.cc test_custom1.cc -o run_tests

test: run_tests
<TAB>./run_tests

clean:
<TAB>rm -f threadpool tptest customtest run_tests