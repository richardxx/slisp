
CC=gcc
CFLAGS=-O2

slisp:aux.o buf.o internal.o interpreter.o lexical.o parser.o linker.o lk_list.o sym_mgr.o opt.o main.o err.o
	${CC} aux.o buf.o internal.o interpreter.o lexical.o parser.o linker.o lk_list.o sym_mgr.o opt.o main.o err.o ${CFLAGS} -o slisp

aux.o:aux.c aux.h types.h err.h trace.h buf.h sys_ds.h
	${CC} -c ${CFLAGS} aux.c -o aux.o

buf.o:buf.c buf.h types.h trace.h
	${CC} -c ${CFLAGS} buf.c -o buf.o

internal.o:internal.h types.h parser.h buf.h err.h trace.h lk_list.h sys_ds.h
	${CC} -c ${CFLAGS} internal.c -o internal.o

interpreter.o:interpreter.h types.h buf.h aux.h trace.h err.h lk_list.h sys_ds.h
	${CC} -c ${CFLAGS} interpreter.c -o interpreter.o

lexical.o:lexical.h buf.h err.h trace.h types.h
	${CC} -c ${CFLAGS} lexical.c -o lexical.o

parser.o:parser.h buf.h lexical.h err.h trace.h types.h lk_list.h sys_ds.h
	${CC} -c ${CFLAGS} parser.c -o parser.o

linker.o:linker.h types.h err.h sys_ds.h lk_list.h trace.h
	${CC} -c ${CFLAGS} linker.c -o linker.o

lk_list.o:lk_list.h types.h err.h
	${CC} -c ${CFLAGS} lk_list.c -o lk_list.o

sym_mgr.o:sys_ds.h types.h err.h
	${CC} -c ${CFLAGS} sym_mgr.c -o sym_mgr.o

opt.o:opt.h err.h types.h
	${CC} -c ${CFLAGS} opt.c -o opt.o

main.o:opt.h interpreter.h 
	${CC} -c ${CFLAGS} main.c -o main.o

err.o:err.h 
	${CC} -c ${CFLAGS} err.c -o err.o
	
clean:
	- rm -f aux.o buf.o internal.o interpreter.o lk_list.o sym_mgr.o opt.o err.o main.o parser.o lexical.o linker.o slisp

