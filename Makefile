#PPCFLAGS = 
CPPFLAGS = -g -O0

linetest: linetest.o
	c++ -o linetest linetest.o -ltrapfpe -lpgplot -lcpgplot -lX11 -lm 

linetest.o: linetest.cpp
	c++ -c linetest.cpp


circletest: circletest.o celestial.h
	c++ -o circletest circletest.o -ltrapfpe -lpgplot -lcpgplot -lX11 -lm 

circletest.o: circletest.cpp celestial.h
	c++ -c circletest.cpp


twobody: twobody.o celestial.h
	c++ $(CPPFLAGS) -o twobody twobody.o  -ltrapfpe -lpgplot -lcpgplot -lX11
twobody.o: twobody.cpp celestial.h
	c++ $(CPPFLAGS) -c twobody.cpp