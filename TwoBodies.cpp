#include<cpgplot.h>
#include<iostream>
#include<cmath>
#include<unistd.h>
#include"celestial.h"

int main() {

	//const float G=1.0, M=1.0, AU=1.0, SecYear=1.0;
	//const float G=6.67e-11, M=2.0e30, AU=1.5e11, SecYear=3.154e7; // real constants
	const float GM = 4*M_PI*M_PI; //derived from Kepler's Laws
	int nIter = 1000,i,numOrb=0; //number of iterations per circle, loop iterator, number of orbits
	float tempY; //for number of orbits
	float dt = 0.01; //time step

	//#####Set up plot window#####
	if(!cpgopen("/XWINDOW")) return 1; //open window
	cpgenv(-15,10,-10,10,1,0); //sets up axes
	cpglab("x","y","Simple Orbit Around Sun");
	
	//#####Set up planet variables####
	celestial p,s; //make planet and sun objects
	
	
	//#####Initial Calculations###### (modelled after Sun/Jupiter system)
	p.R = 5.2; //orbital radius at 5.2AU
	p.m = 0.001*M; //mass set to a thousandth of solar mass
	p.X = p.R; p.Y = 0.0;
	p.Vt = sqrt(GM/(p.R)); //finds tangential in m/s
	
	p.aX=-GM*(p.X)/(p.R*p.R*p.R); //update acceleration derived from Newton's gravity equations
	p.aY=-GM*(p.Y)/(p.R*p.R*p.R);
	
	p.Vx = 0.0; //initial X velocity
	p.Vy = 1.2*p.Vt; //inital Y velocity, testing at 1.2*Jupiter's to see elliptical orbits
	
	
	//cpgsci(7); //yellow
	//cpgpt(1,&p.Y,&p.Y,25); //draw star orbit position
	cpgsci(1); //white
	cpgpt(1,&p.X,&p.Y,2); //draw starting position marker
	
	std::cout <<"\n###########Calculated Properties###########\nR = " <<p.R << "AU\n";
	std::cout <<"vT = " <<p.Vt << "AU/year\n";
	std::cout <<"Ax = " <<p.aX <<"\t\tAy = "<<p.aY<<"\n";
	std::cout <<"dt: " << dt << "\n";
	
	tempY=p.Y;
	//#####Loop Calculations#####
	for(i=0; i<10000000; i++) { //using leapfrog method to calculate orbit
		p.X += (dt/2)*p.Vx;
		p.Y += (dt/2)*p.Vy;
		p.R = sqrt(p.X*p.X+p.Y*p.Y);
		
		p.aX = -GM*(p.X)/(p.R*p.R*p.R);
		p.aY = -GM*(p.Y)/(p.R*p.R*p.R);
		
		p.Vx += p.aX*dt;
		p.Vy += p.aY*dt;
		
		p.X += (dt/2)*p.Vx;
		p.Y += (dt/2)*p.Vy;
		
		cpgsci(6); //purple 
		cpgpt(1,&p.X,&p.Y,1); //draw planet orbit position
		//usleep(500); //slows program to show progression of orbit
		if(p.Y>=0.0&&tempY<0.0) { //counts and prints the number of orbits
			numOrb++;
			std::cout<<"Orbits: "<<numOrb<<"\n";
		}
		tempY=p.Y;
	}
	cpgclos(); //pause before closing, require user to return to close plot
}
