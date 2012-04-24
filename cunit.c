
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include <CUnit/CUnit.h>

#include <CUnit/Automated.h>
#include <CUnit/Basic.h>
#include <CUnit/Console.h>
#include <CUnit/CUCurses.h>

#include "test_suites.h"

static void basic_run_tests(void);
static void usage(void);

static void 
basic_run_tests(void)
{

	CU_basic_run_tests();
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
	void (*run_tests)(void);
	CU_BasicRunMode basic_runmode;
	int ch;
	const char *filename;

	run_tests = basic_run_tests;
	basic_runmode = CU_BRM_NORMAL;
	filename = NULL;
	while ((ch = getopt(argc, argv, "abcChnp:sv")) != -1) {
		switch (ch) {
		case 'a':
			run_tests = CU_automated_run_tests;
			break;
		case 'b':
			run_tests = basic_run_tests;
			break;
		case 'c':
			run_tests = CU_console_run_tests;
			break;
		case 'C':
			run_tests = CU_curses_run_tests;
			break;
		case 'n':
			basic_runmode = CU_BRM_NORMAL;
			break;
		case 'p':
			filename = optarg;
			break;
		case 's':
			basic_runmode = CU_BRM_SILENT;
			break;
		case 'v':
			basic_runmode = CU_BRM_VERBOSE;
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

	if (run_tests == CU_automated_run_tests) {
		if (filename != NULL) {
			CU_set_output_filename(filename);
		}
		CU_automated_enable_junit_xml(CU_TRUE);
	} else if (run_tests == basic_run_tests) {
		CU_basic_set_mode(basic_runmode);
	}

	(*run_tests)();

	if (run_tests == CU_automated_run_tests) {
		CU_list_tests_to_file();
	} else if (run_tests == basic_run_tests) {
		CU_basic_show_failures(CU_get_failure_list());
	}

cleanup:
	CU_cleanup_registry();
end:
	if (CU_get_error() != CUE_SUCCESS) {
		fprintf(stderr, "%s\n", CU_get_error_msg());
	}

	return 0;
}

