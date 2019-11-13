#CPPCFLAGS = 
CPPFLAGS = -g -O0

twobody: twobody.o celestial.h
	c++ $(CPPFLAGS) -o twobody twobody.o  -ltrapfpe -lpgplot -lcpgplot -lX11
twobody.o: twobody.cpp celestial.h
	c++ $(CPPFLAGS) -c twobody.cpp
	
odeTest: odeTest.o celestial.h
	c++ $(CPPFLAGS) -o odeTest odeTest.o  -ltrapfpe -lpgplot -lcpgplot -lX11
odeTest.o: odeTest.cpp celestial.h
	c++ $(CPPFLAGS) -c odeTest.cpp
	
	
#Using these ones
multiBody: multiBody.o celestial.h
	c++ $(CPPFLAGS) -o multiBody multiBody.o  -ltrapfpe -lpgplot -lcpgplot -lX11
multiBody.o: multiBody.cpp celestial.h
	c++ $(CPPFLAGS) -c multiBody.cpp
