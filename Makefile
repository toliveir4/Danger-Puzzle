PYFILE = EA_TP1.py
CPPFILE = EA_TP1.cpp
EXE = excluded/exe.out
TESTCASE = excluded/input_ze.txt
TESTOUTPUT = excluded/output_extra.txt
OUTPUT = excluded/output.txt
DIFF = excluded/diff.txt
TIMEFILE = excluded/timefile

execpp:
	g++ -std=c++17 -Wall -Wextra -O2 -o ${EXE} ${CPPFILE} -lm 
	/usr/bin/time -v -o ${TIMEFILE} ./${EXE} < ${TESTCASE} > ${OUTPUT}
	diff -c ${OUTPUT} ${TESTOUTPUT} > ${DIFF}

exepypy:
	/usr/bin/time -v -o ${TIMEFILE} pypy3 ${PYFILE} < ${TESTCASE} > ${OUTPUT}
	diff -c ${OUTPUT} ${TESTOUTPUT} > ${DIFF}

clear:
	rm -rf ${DIFF} ${OUTPUT} ${TIMEFILE}