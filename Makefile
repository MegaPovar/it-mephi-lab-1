matrix_lab: main.c matrix.c field_info.c complex.c tests.c
	gcc -o matrix_lab main.c matrix.c field_info.c complex.c tests.c -lm

clean:
	rm -f matrix_lab