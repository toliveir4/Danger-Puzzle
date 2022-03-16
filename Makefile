PYFILE = EA_TP1.py
CPPFILE = EA_TP1.cpp
EXE = excluded/exe.out
TESTCASE = excluded/p933.txt
TESTOUTPUT = excluded/output_p933.txt
OUTPUT = excluded/output.txt
DIFF = excluded/diff.txt
TIMEFILE = excluded/timefile

execpp: ${EXE} ${CPPFILE} ${TIMEFILE} ${TESTCASE} ${OUTPUT} ${TESTOUTPUT} ${DIFF}
	g++ -std=c++17 -Wall -Wextra -O2 -o ${EXE} ${CPPFILE} -lm
	/usr/bin/time -v -o ${TIMEFILE} ./${EXE} < ${TESTCASE} > ${OUTPUT}
	diff -c ${OUTPUT} ${TESTOUTPUT} > ${DIFF}

test: ${EXE} ${SHELL}
	./excluded/test.sh

exepypy: ${EXE} ${PYFILE} ${TIMEFILE} ${TESTCASE} ${OUTPUT} ${TESTOUTPUT} ${DIFF}
	/usr/bin/time -v -o ${TIMEFILE} pypy3 ${PYFILE} < ${TESTCASE} > ${OUTPUT}
	diff -c ${OUTPUT} ${TESTOUTPUT} > ${DIFF}

clear: ${TIMEFILE} ${OUTPUT} ${DIFF}
	rm -rf ${DIFF} ${OUTPUT} ${TIMEFILE}