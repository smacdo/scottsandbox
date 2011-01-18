project(Markov)

add_definitions("-std=c++0x")
add_program_with_test(markov elementnode.cpp logging.cpp
    markovdata.cpp tests.cpp markovfactory.cpp)
