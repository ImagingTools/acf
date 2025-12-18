# Configuration for a target for running all project automated tests
include(GeneralConfig.pri)

TEMPLATE = lib

CONFIG += staticlib

DESTDIR = $$OUT_PWD/../../../Lib/$$COMPILER_DIR
TESTDIR = $$OUT_PWD/../../../Bin/$$COMPILER_DIR/Tests

# pre-build: executes all tests
if(!defined(RUN_TESTS, var)|contains(RUN_TESTS, true)):win32{
	execute_tests.name = Running automated tests...
	execute_tests.CONFIG += no_link no_clean target_predeps
	execute_tests.output = $$OUT_PWD/dummy
	PHONY_DEPS = .
	execute_tests.input = PHONY_DEPS
	execute_tests.commands = cd $$TESTDIR && $$(ACFCONFIGDIR)\\Test\\run_tests.bat
	QMAKE_EXTRA_COMPILERS = execute_tests
}

