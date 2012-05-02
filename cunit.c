
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include <CUnit/CUnit.h>

#include <CUnit/Automated.h>
#include <CUnit/Basic.h>
#include <CUnit/Console.h>
#include <CUnit/CUCurses.h>

#include "test_suites.h"

enum {AUTOMATED, BASIC, CONSOLE, CURSES};

struct test_options {
	CU_BasicRunMode basic_runmode;
	const char *filename;
}; 

struct test_interface {
	void (*setup)(const struct test_options *);
	void (*run_tests)(void);
	void (*finish)(void);
};

static void automated_setup(const struct test_options *);
static void basic_setup(const struct test_options *);
static void dummy_setup(const struct test_options *);

static void basic_run_tests(void);

static void automated_finish(void);
static void basic_finish(void);
static void dummy_finish(void);

static void usage(void);

static void
automated_setup(const struct test_options *test_opts)
{

	if (test_opts->filename != NULL) {
		CU_set_output_filename(test_opts->filename);
	}
	CU_automated_enable_junit_xml(CU_TRUE);
}

static void
basic_setup(const struct test_options *test_opts) 
{

	CU_basic_set_mode(test_opts->basic_runmode);
}

static void
dummy_setup(const struct test_options *test_opts)
{

	;
}

static void 
basic_run_tests(void)
{

	CU_basic_run_tests();
}

static void
automated_finish(void)
{

	CU_list_tests_to_file();
}

static void
basic_finish(void)
{

	CU_basic_show_failures(CU_get_failure_list());
}

static void
dummy_finish(void)
{

	;
}

static void
usage(void)
{

	fprintf(stderr, "%s [-abcChnsv] [-p filename-prefix]\n", getprogname());
	fprintf(stderr, "-a : set Automated interface\n");
	fprintf(stderr, "-b : set Basic interface\n");
	fprintf(stderr, "-c : set Console interface\n");
	fprintf(stderr, "-C : set Curses interface\n");
	fprintf(stderr, "-h : show this help\n");
	fprintf(stderr, "-n : set NORMAL  mode (Basic interface only)\n");
	fprintf(stderr, "-s : set SILENT  mode (Basic interface only)\n");
	fprintf(stderr, "-v : set VERBOSE mode (Basic interface only)\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "-p filename-prefix : "
			"set filename-prefix (Automated interface only)\n");

	exit(1);
}

int 
main(int argc, char *argv[])
{
	struct test_interface test_interfaces[] = {
		{automated_setup,	CU_automated_run_tests,	automated_finish},
		{basic_setup,		basic_run_tests,	basic_finish},
		{dummy_setup,		CU_console_run_tests,	dummy_finish},
		{dummy_setup,		CU_curses_run_tests,	dummy_finish}
	};
	const struct test_interface *test_if;
	struct test_options test_opts;
	int ch;

	test_if = &test_interfaces[AUTOMATED];
	test_opts.basic_runmode = CU_BRM_NORMAL;
	test_opts.filename = NULL;
	while ((ch = getopt(argc, argv, "abcChnp:sv")) != -1) {
		switch (ch) {
		case 'a':
			test_if = &test_interfaces[AUTOMATED];
			break;
		case 'b':
			test_if = &test_interfaces[BASIC];
			break;
		case 'c':
			test_if = &test_interfaces[CONSOLE];
			break;
		case 'C':
			test_if = &test_interfaces[CURSES];
			break;
		case 'n':
			test_opts.basic_runmode = CU_BRM_NORMAL;
			break;
		case 'p':
			test_opts.filename = optarg;
			break;
		case 's':
			test_opts.basic_runmode = CU_BRM_SILENT;
			break;
		case 'v':
			test_opts.basic_runmode = CU_BRM_VERBOSE;
			break;
		case 'h':
		case '?':
		default:
			usage();
			/* NOTREACHED */
			break;
		}
	}
	argc -= optind;
	argv += optind;

	if (CU_initialize_registry() != CUE_SUCCESS) {
		goto end;
	}
	if (CU_register_suites(test_suites) != CUE_SUCCESS) {
		goto cleanup;
	}

	(*test_if->setup)(&test_opts);

	(*test_if->run_tests)();

	(*test_if->finish)();

cleanup:
	CU_cleanup_registry();
end:
	if (CU_get_error() != CUE_SUCCESS) {
		fprintf(stderr, "%s\n", CU_get_error_msg());
	}

	return 0;
}

