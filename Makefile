PYFILE = EA_TP1.py
TESTCASE = input_extra
TESTOUTPUT = output_extra
OUTPUT = output
DIFF = diff.txt

exe:
	/usr/bin/time -v -o timefile pypy3 ${PYFILE} < ${TESTCASE} > ${OUTPUT}
	diff -c ${OUTPUT} ${TESTOUTPUT} > ${DIFF}

clear:
	rm -rf ${DIFF} ${OUTPUT}