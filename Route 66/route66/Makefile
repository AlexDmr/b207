# Project: route66lib
# Makefile created by L.Balme

CPP  = g++

L_ROUTE66_OBJS = ./bin/linux/route66.o ./bin/linux/route66tcpgateway.o ./bin/linux/steadypacketservice.o ./bin/linux/overseacallbackservice.o
D_ROUTE66_OBJS = ./bin/darwin/route66.o ./bin/darwin/route66tcpgateway.o ./bin/darwin/steadypacketservice.o ./bin/darwin/overseacallbackservice.o
W_ROUTE66_OBJS = ./bin/win32/route66.o ./bin/win32/route66tcpgateway.o ./bin/win32/steadypacketservice.o ./bin/win32/overseacallbackservice.o

INCS =  -I"../slimlib/slimcore/includes" -I"../slimlib/slimxml/includes" -I"./includes"



usage:
	more README 

clean: 
	rm -f $(L_ROUTE66_OBJS) ./bin/linux/route66lib.o
	rm -f $(D_ROUTE66_OBJS) ./bin/darwin/route66lib.o
	rm -f $(W_ROUTE66_OBJS) ./bin/win32/route66lib.o


linux: $(L_ROUTE66_OBJS)
	rm -f ./bin/linux/route66lib.o
	ar -r ./bin/linux/route66lib.o $(L_ROUTE66_OBJS)
	ranlib ./bin/linux/route66lib.o
	rm $(L_ROUTE66_OBJS)

darwin: $(D_ROUTE66_OBJS)
	rm -f ./bin/darwin/route66lib.o
	ar -r ./bin/darwin/route66lib.o $(D_ROUTE66_OBJS)
	ranlib ./bin/darwin/route66lib.o
	rm $(D_ROUTE66_OBJS)

win32: $(W_ROUTE66_OBJS)
	rm -f ./bin/win32/route66lib.o
	ar -r ./bin/win32/route66lib.o $(W_ROUTE66_OBJS)
	ranlib ./bin/win32/route66lib.o
	rm $(W_ROUTE66_OBJS)







./bin/linux/route66.o: sources/route66.cpp
	$(CPP) -c sources/route66.cpp -o ./bin/linux/route66.o $(INCS)

./bin/darwin/route66.o: sources/route66.cpp
	$(CPP) -c sources/route66.cpp -o ./bin/darwin/route66.o $(INCS)

./bin/win32/route66.o: sources/route66.cpp
	$(CPP) -c sources/route66.cpp -o ./bin/win32/route66.o $(INCS)




./bin/linux/route66tcpgateway.o: sources/route66tcpgateway.cpp
	$(CPP) -c sources/route66tcpgateway.cpp -o ./bin/linux/route66tcpgateway.o $(INCS)

./bin/darwin/route66tcpgateway.o: sources/route66tcpgateway.cpp
	$(CPP) -c sources/route66tcpgateway.cpp -o ./bin/darwin/route66tcpgateway.o $(INCS)

./bin/win32/route66tcpgateway.o: sources/route66tcpgateway.cpp
	$(CPP) -c sources/route66tcpgateway.cpp -o ./bin/win32/route66tcpgateway.o $(INCS)





./bin/linux/steadypacketservice.o: sources/steadypacketservice.cpp
	$(CPP) -c sources/steadypacketservice.cpp -o ./bin/linux/steadypacketservice.o $(INCS)

./bin/darwin/steadypacketservice.o: sources/steadypacketservice.cpp
	$(CPP) -c sources/steadypacketservice.cpp -o ./bin/darwin/steadypacketservice.o $(INCS)

./bin/win32/steadypacketservice.o: sources/steadypacketservice.cpp
	$(CPP) -c sources/steadypacketservice.cpp -o ./bin/win32/steadypacketservice.o $(INCS)



./bin/linux/overseacallbackservice.o: sources/overseacallbackservice.cpp
	$(CPP) -c sources/overseacallbackservice.cpp -o ./bin/linux/overseacallbackservice.o $(INCS)

./bin/darwin/overseacallbackservice.o: sources/overseacallbackservice.cpp
	$(CPP) -c sources/overseacallbackservice.cpp -o ./bin/darwin/overseacallbackservice.o $(INCS)

./bin/win32/overseacallbackservice.o: sources/overseacallbackservice.cpp
	$(CPP) -c sources/overseacallbackservice.cpp -o ./bin/win32/overseacallbackservice.o $(INCS)

