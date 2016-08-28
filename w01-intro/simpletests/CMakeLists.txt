#Compile a simple executable
add_executable(helloCS40 helloCS40.cpp)
add_executable(glmTest glmTest.cpp)

#Compile multiple executables the same way
set(MTESTS mathTest1 mathTest2)

foreach(PROG ${MTESTS})
 add_executable(${PROG} ${PROG}.cpp)
endforeach(PROG)

