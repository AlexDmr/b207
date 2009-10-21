# Project: route66lib_tests
# Makefile created by L.Balme

CPP  = g++

L_LIB_OBJ = ../bin/linux/route66lib.o ../../slimlib/bin/linux/slimlib.o
D_LIB_OBJ = ../bin/darwin/route66lib.o ../../slimlib/bin/darwin/slimlib.o
W_LIB_OBJ = ../bin/win32/route66lib.o ../../slimlib/bin/win32/slimlib.o

L_TEST_OBJS = ../bin/linux/test_route66.o ../bin/linux/test_route66tcpgateway.o ../bin/linux/test_steadypacketservice.o ../bin/linux/test_overseacallbackservice.o
D_TEST_OBJS = ../bin/darwin/test_route66.o ../bin/darwin/test_route66tcpgateway.o ../bin/darwin/test_steadypacketservice.o ../bin/darwin/test_overseacallbackservice.o
W_TEST_OBJS = ../bin/win32/test_route66.o ../bin/win32/test_route66tcpgateway.o ../bin/win32/test_steadypacketservice.o ../bin/win32/test_overseacallbackservice.o

INCS =  -I"../../slimlib/slimcore/includes" -I"../../slimlib/slimxml/includes" -I"../includes"



usage:
	more README 

clean: 
	rm -f $(L_TEST_OBJS)
	rm -f $(D_TEST_OBJS)
	rm -f $(W_TEST_OBJS)


linux: $(L_LIB_OBJ) $(L_TEST_OBJS)
	$(CPP) ../bin/linux/test_route66.o $(L_LIB_OBJ) -o test_route66 $(INCS) -lpthread
	$(CPP) ../bin/linux/test_route66tcpgateway.o $(L_LIB_OBJ) -o test_route66tcpgateway $(INCS) -lpthread
	$(CPP) ../bin/linux/test_steadypacketservice.o $(L_LIB_OBJ) -o test_steadypacketservice $(INCS) -lpthread
	$(CPP) ../bin/linux/test_overseacallbackservice.o $(L_LIB_OBJ) -o test_overseacallbackservice $(INCS) -lpthread

darwin: $(D_LIB_OBJ) $(D_TEST_OBJS)
	$(CPP) ../bin/darwin/test_route66.o $(D_LIB_OBJ) -o test_route66 $(INCS)
	$(CPP) ../bin/darwin/test_route66tcpgateway.o $(D_LIB_OBJ) -o test_route66tcpgateway $(INCS)
	$(CPP) ../bin/darwin/test_steadypacketservice.o $(D_LIB_OBJ) -o test_steadypacketservice $(INCS)
	$(CPP) ../bin/darwin/test_overseacallbackservice.o $(D_LIB_OBJ) -o test_overseacallbackservice $(INCS)

win32: $(W_LIB_OBJ) $(W_TEST_OBJS)
	$(CPP) ../bin/win32/test_route66.o $(W_LIB_OBJ) -o test_route66.exe $(INCS) -lwsock32
	$(CPP) ../bin/win32/test_route66tcpgateway.o $(W_LIB_OBJ) -o test_route66tcpgateway.exe $(INCS) -lwsock32
	$(CPP) ../bin/win32/test_steadypacketservice.o $(W_LIB_OBJ) -o test_steadypacketservice.exe $(INCS) -lwsock32
	$(CPP) ../bin/win32/test_overseacallbackservice.o $(W_LIB_OBJ) -o test_overseacallbackservice.exe $(INCS) -lwsock32







../bin/linux/test_route66.o: sources/test_route66.cpp
	$(CPP) -c sources/test_route66.cpp -o ../bin/linux/test_route66.o $(INCS)

../bin/darwin/test_route66.o: sources/test_route66.cpp
	$(CPP) -c sources/test_route66.cpp -o ../bin/darwin/test_route66.o $(INCS)

../bin/win32/test_route66.o: sources/test_route66.cpp
	$(CPP) -c sources/test_route66.cpp -o ../bin/win32/test_route66.o $(INCS)




../bin/linux/test_route66tcpgateway.o: sources/test_route66tcpgateway.cpp
	$(CPP) -c sources/test_route66tcpgateway.cpp -o ../bin/linux/test_route66tcpgateway.o $(INCS)

../bin/darwin/test_route66tcpgateway.o: sources/test_route66tcpgateway.cpp
	$(CPP) -c sources/test_route66tcpgateway.cpp -o ../bin/darwin/test_route66tcpgateway.o $(INCS)

../bin/win32/test_route66tcpgateway.o: sources/test_route66tcpgateway.cpp
	$(CPP) -c sources/test_route66tcpgateway.cpp -o ../bin/win32/test_route66tcpgateway.o $(INCS)




../bin/linux/test_steadypacketservice.o: sources/test_steadypacketservice.cpp
	$(CPP) -c sources/test_steadypacketservice.cpp -o ../bin/linux/test_steadypacketservice.o $(INCS)

../bin/darwin/test_steadypacketservice.o: sources/test_steadypacketservice.cpp
	$(CPP) -c sources/test_steadypacketservice.cpp -o ../bin/darwin/test_steadypacketservice.o $(INCS)

../bin/win32/test_steadypacketservice.o: sources/test_steadypacketservice.cpp
	$(CPP) -c sources/test_steadypacketservice.cpp -o ../bin/win32/test_steadypacketservice.o $(INCS)




../bin/linux/test_overseacallbackservice.o: sources/test_overseacallbackservice.cpp
	$(CPP) -c sources/test_overseacallbackservice.cpp -o ../bin/linux/test_overseacallbackservice.o $(INCS)

../bin/darwin/test_overseacallbackservice.o: sources/test_overseacallbackservice.cpp
	$(CPP) -c sources/test_overseacallbackservice.cpp -o ../bin/darwin/test_overseacallbackservice.o $(INCS)

../bin/win32/test_overseacallbackservice.o: sources/test_overseacallbackservice.cpp
	$(CPP) -c sources/test_overseacallbackservice.cpp -o ../bin/win32/test_overseacallbackservice.o $(INCS)
