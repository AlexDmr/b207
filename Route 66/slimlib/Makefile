# Project: slimlib
# Makefile created by L.Balme

CPP  = g++

L_CORE_OBJS = ./bin/linux/slimcore.o ./bin/linux/slimthread.o ./bin/linux/slimsocket.o ./bin/linux/slimfile.o ./bin/linux/slimregexp.o
D_CORE_OBJS = ./bin/darwin/slimcore.o ./bin/darwin/slimthread.o ./bin/darwin/slimsocket.o ./bin/darwin/slimfile.o ./bin/darwin/slimregexp.o
W_CORE_OBJS = ./bin/win32/slimcore.o ./bin/win32/slimthread.o ./bin/win32/slimsocket.o ./bin/win32/slimfile.o ./bin/win32/slimregexp.o

L_XML_OBJS = ./bin/linux/slimxmlparser.o ./bin/linux/slimxmlpattern.o ./bin/linux/slimxmlconfigurationfile.o
D_XML_OBJS = ./bin/darwin/slimxmlparser.o ./bin/darwin/slimxmlpattern.o ./bin/darwin/slimxmlconfigurationfile.o
W_XML_OBJS = ./bin/win32/slimxmlparser.o ./bin/win32/slimxmlpattern.o ./bin/win32/slimxmlconfigurationfile.o

INCS =  -I"./slimcore/includes" -I"./slimxml/includes"

usage:
	more README 
clean: 
	rm -f $(LINUX_OBJS) ./bin/linux/slimlib.o
	rm -f $(DARWIN_OBJS) ./bin/darwin/slimlib.o
	rm -f $(WIN32_OBJS) ./bin/win32/slimlib.o


linux: $(L_CORE_OBJS) $(L_XML_OBJS)
	ar -r ./bin/linux/slimlib.o $(L_CORE_OBJS)  $(L_XML_OBJS)
	ranlib ./bin/linux/slimlib.o
	rm $(L_CORE_OBJS)  $(L_XML_OBJS)

darwin: $(D_CORE_OBJS) $(D_XML_OBJS)
	ar -r ./bin/darwin/slimlib.o $(D_CORE_OBJS)  $(D_XML_OBJS)
	ranlib ./bin/darwin/slimlib.o
	rm $(D_CORE_OBJS)  $(D_XML_OBJS)

win32: $(W_CORE_OBJS) $(W_XML_OBJS)
	ar -r ./bin/win32/slimlib.o $(W_CORE_OBJS)  $(W_XML_OBJS)
	ranlib ./bin/win32/slimlib.o
	rm $(W_CORE_OBJS)  $(W_XML_OBJS)







./bin/linux/slimthread.o: slimcore/sources/slimthread.cpp
	$(CPP) -c slimcore/sources/slimthread.cpp -o ./bin/linux/slimthread.o $(INCS)

./bin/darwin/slimthread.o: slimcore/sources/slimthread.cpp
	$(CPP) -c slimcore/sources/slimthread.cpp -o ./bin/darwin/slimthread.o $(INCS)

./bin/win32/slimthread.o: slimcore/sources/slimthread.cpp
	$(CPP) -c slimcore/sources/slimthread.cpp -o ./bin/win32/slimthread.o $(INCS)



./bin/linux/slimsocket.o: slimcore/sources/slimsocket.cpp
	$(CPP) -c slimcore/sources/slimsocket.cpp -o ./bin/linux/slimsocket.o $(INCS)

./bin/darwin/slimsocket.o: slimcore/sources/slimsocket.cpp
	$(CPP) -c slimcore/sources/slimsocket.cpp -o ./bin/darwin/slimsocket.o $(INCS)

./bin/win32/slimsocket.o: slimcore/sources/slimsocket.cpp
	$(CPP) -c slimcore/sources/slimsocket.cpp -o ./bin/win32/slimsocket.o $(INCS)




./bin/linux/slimfile.o: slimcore/sources/slimfile.cpp
	$(CPP) -c slimcore/sources/slimfile.cpp -o ./bin/linux/slimfile.o $(INCS)

./bin/darwin/slimfile.o: slimcore/sources/slimfile.cpp
	$(CPP) -c slimcore/sources/slimfile.cpp -o ./bin/darwin/slimfile.o $(INCS)

./bin/win32/slimfile.o: slimcore/sources/slimfile.cpp
	$(CPP) -c slimcore/sources/slimfile.cpp -o ./bin/win32/slimfile.o $(INCS)




./bin/linux/slimregexp.o: slimcore/sources/slimregexp.cpp
	$(CPP) -c slimcore/sources/slimregexp.cpp -o ./bin/linux/slimregexp.o $(INCS)

./bin/darwin/slimregexp.o: slimcore/sources/slimregexp.cpp
	$(CPP) -c slimcore/sources/slimregexp.cpp -o ./bin/darwin/slimregexp.o $(INCS)

./bin/win32/slimregexp.o: slimcore/sources/slimregexp.cpp
	$(CPP) -c slimcore/sources/slimregexp.cpp -o ./bin/win32/slimregexp.o $(INCS)



	

./bin/linux/slimcore.o: slimcore/sources/slimcore.cpp
	$(CPP) -c slimcore/sources/slimcore.cpp -o ./bin/linux/slimcore.o $(INCS)

./bin/darwin/slimcore.o: slimcore/sources/slimcore.cpp
	$(CPP) -c slimcore/sources/slimcore.cpp -o ./bin/darwin/slimcore.o $(INCS)

./bin/win32/slimcore.o: slimcore/sources/slimcore.cpp
	$(CPP) -c slimcore/sources/slimcore.cpp -o ./bin/win32/slimcore.o $(INCS)












./bin/linux/slimxmlparser.o: slimxml/sources/slimxmlparser.cpp
	$(CPP) -c slimxml/sources/slimxmlparser.cpp -o ./bin/linux/slimxmlparser.o $(INCS)

./bin/darwin/slimxmlparser.o: slimxml/sources/slimxmlparser.cpp
	$(CPP) -c slimxml/sources/slimxmlparser.cpp -o ./bin/darwin/slimxmlparser.o $(INCS)

./bin/win32/slimxmlparser.o: slimxml/sources/slimxmlparser.cpp
	$(CPP) -c slimxml/sources/slimxmlparser.cpp -o ./bin/win32/slimxmlparser.o $(INCS)




./bin/linux/slimxmlpattern.o: slimxml/sources/slimxmlpattern.cpp
	$(CPP) -c slimxml/sources/slimxmlpattern.cpp -o ./bin/linux/slimxmlpattern.o $(INCS)

./bin/darwin/slimxmlpattern.o: slimxml/sources/slimxmlpattern.cpp
	$(CPP) -c slimxml/sources/slimxmlpattern.cpp -o ./bin/darwin/slimxmlpattern.o $(INCS)

./bin/win32/slimxmlpattern.o: slimxml/sources/slimxmlpattern.cpp
	$(CPP) -c slimxml/sources/slimxmlpattern.cpp -o ./bin/win32/slimxmlpattern.o $(INCS)




./bin/linux/slimxmlconfigurationfile.o: slimxml/sources/slimxmlconfigurationfile.cpp
	$(CPP) -c slimxml/sources/slimxmlconfigurationfile.cpp -o ./bin/linux/slimxmlconfigurationfile.o $(INCS)

./bin/darwin/slimxmlconfigurationfile.o: slimxml/sources/slimxmlconfigurationfile.cpp
	$(CPP) -c slimxml/sources/slimxmlconfigurationfile.cpp -o ./bin/darwin/slimxmlconfigurationfile.o $(INCS)

./bin/win32/slimxmlconfigurationfile.o: slimxml/sources/slimxmlconfigurationfile.cpp
	$(CPP) -c slimxml/sources/slimxmlconfigurationfile.cpp -o ./bin/win32/slimxmlconfigurationfile.o $(INCS)
