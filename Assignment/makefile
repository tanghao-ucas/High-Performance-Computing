ch1_1:ch1_1.c myhead.c
	gcc $^ -o $@

ch1_2:ch1_2.c myhead.c
	gcc $^ -o $@

ch4_2:ch4_2.c myhead.c
	mpicc $^ -o $@

ch6_1:ch6_1.c myhead.c
	gcc $^ -lm -o $@

ch6_2:ch6_2.c
	gcc $^ -o $@

ch8:ch8.c myhead.c
	mpicc $^ -o $@ 

ch9_1:ch9_1.c myhead.c
	mpicc $^ -o $@

ch9_2:ch9_2.c myhead.c
	mpicc $^ -o $@

ch9_3:ch9_3.c myhead.c
	mpicc $^ -o $@

ch10_1:ch10_1.c myhead.c
	mpicc $^ -o $@

ch10_2:ch10_2.c myhead.c
	mpicc $^ -o $@

comm2d:comm2d.c myhead.c
	mpicc $^ -lm -o $@

cannon:cannon.c myhead.c
	mpicc $^ -lm -o $@

LU_serial:LU_serial.c myhead.c
	gcc $^ -lm -o $@

gauss_seidel:gauss_seidel.c myhead.c
	mpicc $^ -lm -o $@

.PHONY:clean
clean:
	rm ch1_1 ch1_2 ch4_2 ch6_1 ch6_2 ch8 ch9_1 ch9_2 ch9_3 ch10_1 ch10_2 comm2d cannon LU_serial gauss_seidel 

