#CPPCFLAGS = 
CPPFLAGS = -g -O0

multiBody: multiBody.o celestial.h
	c++ $(CPPFLAGS) -o multiBody multiBody.o  -ltrapfpe -lpgplot -lcpgplot -lX11
multiBody.o: multiBody.cpp celestial.h
	c++ $(CPPFLAGS) -c multiBody.cpp
	
	
#----------MAIN FILE BEING USED-----------
manyBody: manyBody.o celestial.h
	c++ $(CPPFLAGS) -o manyBody manyBody.o  -ltrapfpe -lpgplot -lcpgplot -lX11
manyBody.o: manyBody.cpp celestial.h
	c++ $(CPPFLAGS) -c manyBody.cpp
