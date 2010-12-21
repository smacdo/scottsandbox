ADD_EXECUTABLE(bf bf.cpp cli.cpp)

IF(UNIT_TESTS)
    ADD_EXECUTABLE(testrunner-bf bf.cpp tests.cpp)
ENDIF(UNIT_TESTS)
