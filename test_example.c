
#include <CUnit/CUnit.h>

#include <stdlib.h>

#include "test_suites.h"

static void foo_test_xxx(void);
static void bar_test_xxx(void);
static int foo_setup(void);
static int bar_setup(void);
static int foo_teardown(void);
static int bar_teardown(void);

static int foo(int);
static int bar(int);

static CU_TestInfo foo_test[] = {
	{"foo", foo_test_xxx},
	CU_TEST_INFO_NULL
};

static CU_TestInfo bar_test[] = {
	{"bar", bar_test_xxx},
	CU_TEST_INFO_NULL
};

CU_SuiteInfo test_suites[] = {
	{"foo", foo_setup, foo_teardown, foo_test},
	{"bar", bar_setup, bar_teardown, bar_test},
	CU_SUITE_INFO_NULL
};

static void
foo_test_xxx(void)
{

	CU_ASSERT(0 == foo(0));
	CU_ASSERT(2 == foo(1));
}

static void
bar_test_xxx(void)
{

	CU_ASSERT(0 == bar(0));
	CU_ASSERT(1 == bar(1));
	CU_ASSERT(3 == bar(2));
}

static int 
foo_setup(void)
{

	return 0;
}

static int
bar_setup(void)
{

	return 0;
}

static int
foo_teardown(void)
{

	return 0;
}

static int
bar_teardown(void)
{

	return 0;
}

static int
foo(int a)
{

	return a * 2;
}

static int bar(int a)
{
	static int b = 0;

	b += a;
	return b;
}

