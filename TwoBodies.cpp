#include<cpgplot.h>
#include<iostream>
#include<cmath>
#include<unistd.h>
#include"celestial.h"


int main() {
	const float G=1.0, M=1.0, AU=1.0, SecYear=1.0;
	//const float G=6.67e-11, M=2.0e30, AU=1.5e11, SecYear=3.154e7; // real constants
	int nIter = 10000,i,numOrb; //number of iterations per circle, loop iterator, number of orbits
	float orbit = 5.2; //predefined orbital radius of planet and sun
	float CM[] = {0.0,0.0};
	float theta=0.0, tempY=0.0;
	
	float angMom;
	
	//#####Set up plot window#####
	if(!cpgopen("/XWINDOW")) return 1; //open window
	cpgenv(-10,10,-10,10,1,0); //sets up axes
	cpglab("x","y","Simple Orbit Around Sun");
	
	
	celestial s,p;
	p.mass=0.001*M; s.mass=M;
	
	CM[0]=orbit-s.mass/(p.mass+s.mass)*orbit; //centre of mass X coordinant
	p.x=orbit; p.y=0.0; p.R=orbit; //initial position of planet
	p.a=orbit-CM[0]; p.e=CM[0]/p.a; p.b=sqrt(p.a*p.a-CM[0]*CM[0]); //ellipse properties
	
	p.vT=sqrt(G*M*(2.0/(p.R*AU)-1.0/(p.a*AU))); //orbital velocity in m/s, elliptical
	p.vT*=SecYear/AU; //convert to AU/year
	p.w=p.vT/p.R; //angular velocity in AU/year rad??
	p.T=(2.0*M_PI/p.w); //period in years
	p.dt=p.T/nIter;
	
	s.x=0.0,s.y=0.0;
	cpgsci(7); //yellow
	cpgpt(1,&s.x,&s.y,25); //draw star orbit position
	
	
	std::cout <<"\n###########Calculated Properties###########\nRP = " <<p.R << "AU \t\tRS = " <<s.R <<"AU\n";
	std::cout <<"TP = " <<p.T << "years\n";
	std::cout <<"vT P = " <<p.vT << "AU/year\n";
	std::cout <<"Planet eccentricity = " <<p.e <<"\n";
	std::cout <<"CM: " << CM[0] << "\n";
	
	
	p.dt=p.T/nIter; //find time step
	angMom = p.mass*p.vT*p.R; //determine initial angular momentem, do not update as this remains constant
	std::cout <<"Mom: "<<angMom<<"\n";
	
	for(int i=0; i<10000000; i++) {
		if(p.y>=0.0&&tempY<0.0) numOrb++;
		tempY=p.y;
		theta+=p.w*p.dt;
		if(theta>2*M_PI) theta-=2*M_PI;
		p.x+=-p.vT*p.dt*sin(theta);
		p.y+=p.vT*p.dt*cos(theta);
		p.R=sqrt(p.x*p.x+p.y*p.y);
		p.vT=angMom/(p.mass*p.R); //use angular momentum to find velocity
		//p.vT=sqrt(G*M*(2.0/(p.R*AU)-1.0/(p.a*AU))); //orbital velocity in m/s, elliptical
		//p.vT*=SecYear/AU; //convert to AU/year
		p.w=p.vT/p.R; //angular velocity in AU/year rad??
		
		cpgsci(6); //purple 
		cpgpt(1,&p.x,&p.y,1); //draw planet orbit position
		angMom = p.mass*p.vT*p.R;
		std::cout <<" Orbits: "<<numOrb<<"\n";
		//usleep(500); //delay in microseconds
	}
	cpgclos();//pause
}