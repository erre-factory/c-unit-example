# Makefile

PROG= test_example
SRCS= cunit.c test_example.c

CLEANFILES+=*.xml *.gcno *.gcda *.info
CLEANDIRS+=html

CFLAGS+=-I/usr/local/include
CFLAGS+=-g
CFLAGS+=-fprofile-arcs -ftest-coverage
CFLAGS+=-ftrapv
CFLAGS+=-fstack-protector-all

LDFLAGS+=-L/usr/local/lib
LDADD+=-lcunit -lcurses

run-test:
	./test_example -a
	lcov -c -d . -o lcov.info
	lcov -r lcov.info *.h -o lcov-temp.info
	lcov -r lcov-temp.info cunit.c -o lcov-html.info
	genhtml -o html lcov-html.info

.include <bsd.prog.mk>

