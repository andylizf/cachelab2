CC=gcc
CXX=g++
CFLAGS=-Wall -O0 -g -std=c11 -fsanitize=address -fno-omit-frame-pointer
CXXFLAGS=-Wall -O0 -g -std=c++17 -fsanitize=address -fno-omit-frame-pointer
CSIM-REF-FALGS=-Wall -O3 -std=c11

case_s=4
case_E=1
case_b=5

all: main csim demo

matrix.o: matrix.cpp matrix.h common.h cachelab.h
	$(CXX) $(CXXFLAGS) -c matrix.cpp

gemm.o: gemm.cpp matrix.h common.h cachelab.h
	$(CXX) $(CXXFLAGS) -c gemm.cpp

gemm_baseline.o: gemm_baseline.cpp matrix.h common.h cachelab.h
	$(CXX) $(CXXFLAGS) -c gemm_baseline.cpp

test_case.o: test_case.cpp gemm.h matrix.h common.h cachelab.h
	$(CXX) $(CXXFLAGS) -c test_case.cpp

main.o: main.cpp gemm.h matrix.h common.h test_case.h cachelab.h
	$(CXX) $(CXXFLAGS) -c main.cpp

main: main.o gemm.o matrix.o test_case.o gemm_baseline.o
	$(CXX) $(CXXFLAGS) -o main main.o gemm.o gemm_baseline.o matrix.o test_case.o

demo.o: demo.cpp gemm.h matrix.h common.h cachelab.h
	$(CXX) $(CXXFLAGS) -c demo.cpp

demo: demo.o gemm.o matrix.o
	$(CXX) $(CXXFLAGS) -o demo demo.o gemm.o matrix.o

csim: csim.c
	$(CC) $(CFLAGS) -o csim csim.c

csim-ref: csim-ref.c
	$(CC) ${CSIM-REF-FALGS} -o csim-ref csim-ref.c
	strip csim-ref

case%:
	make
	mkdir -p gemm_traces
	./main $@ > gemm_traces/$@.trace
	@if [ "$(NO_LINUX)" = "true" ]; then \
		./csim -s $(case_s) -E $(case_E) -b $(case_b) -t gemm_traces/$@.trace; \
	else \
		./csim-ref -s $(case_s) -E $(case_E) -b $(case_b) -t gemm_traces/$@.trace; \
	fi

clean:
	rm -rf main demo *.o csim handin.tar gemm_traces .csim_results .overall_results .autograder_result .last_submit_time

handin:
	tar cvf handin.tar csim.c gemm.cpp report/report.pdf
