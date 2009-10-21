# Project: slimlib_tests
# Makefile created by L.Balme

CPP  = g++

L_SLIMLIB_OBJ = ../bin/linux/slimlib.o 
D_SLIMLIB_OBJ = ../bin/darwin/slimlib.o 
W_SLIMLIB_OBJ = ../bin/win32/slimlib.o 

L_TEST_OBJS = ../bin/linux/test_slimxmlpattern.o 
D_TEST_OBJS = ../bin/darwin/test_slimxmlpattern.o 
W_TEST_OBJS = ../bin/win32/test_slimxmlpattern.o 

INCS =  -I"../slimcore/includes" -I"../slimxml/includes"



usage:
	more README 

clean: 
	rm -f $(L_TEST_OBJS)
	rm -f $(D_TEST_OBJS)
	rm -f $(W_TEST_OBJS)


linux: $(L_SLIMLIB_OBJ) $(L_TEST_OBJS)
	$(CPP) ../bin/linux/test_slimxmlpattern.o $(L_SLIMLIB_OBJ) -o test_slimxmlpattern $(INCS)

darwin: $(D_SLIMLIB_OBJ) $(D_TEST_OBJS)
	$(CPP) ../bin/darwin/test_slimxmlpattern.o $(D_SLIMLIB_OBJ) -o test_slimxmlpattern $(INCS)

win32: $(W_SLIMLIB_OBJ) $(W_TEST_OBJS)
	$(CPP) ../bin/win32/test_slimxmlpattern.o $(W_SLIMLIB_OBJ) -o test_slimxmlpattern.exe $(INCS)







../bin/linux/test_slimxmlpattern.o: sources/test_slimxmlpattern.cpp
	$(CPP) -c sources/test_slimxmlpattern.cpp -o ../bin/linux/test_slimxmlpattern.o $(INCS)

../bin/darwin/test_slimxmlpattern.o: sources/test_slimxmlpattern.cpp
	$(CPP) -c sources/test_slimxmlpattern.cpp -o ../bin/darwin/test_slimxmlpattern.o $(INCS)

../bin/win32/test_slimxmlpattern.o: sources/test_slimxmlpattern.cpp
	$(CPP) -c sources/test_slimxmlpattern.cpp -o ../bin/win32/test_slimxmlpattern.o $(INCS)
