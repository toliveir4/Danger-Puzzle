PYFILE = EA_TP1.py
TESTCASE = input_extra.txt
TESTOUTPUT = output_extra.txt
OUTPUT = output.txt
DIFF = diff.txt
TIMEFILE = timefile

exe:
	/usr/bin/time -v -o ${TIMEFILE} pypy3 ${PYFILE} < ${TESTCASE} > ${OUTPUT}
	diff -c ${OUTPUT} ${TESTOUTPUT} > ${DIFF}

clear:
	rm -rf ${DIFF} ${OUTPUT} ${TIMEFILE}