#include<cpgplot.h>
#include<iostream>
#include<cmath>
#include<unistd.h>
#include"celestial.h"

int main() {

	const float G=1.0, M=1.0;
	//const float G=6.67e-11, M=2.0e30, AU=1.5e11, SecYear=3.154e7; // real constants
	//const float GM = 4*M_PI*M_PI, M=2.0e30; //derived from Kepler's Laws
	int nIter = 1000,i,j,k,numOrb=0,N=2; //number of iterations per circle, loop iterator, number of orbits
	float tempY; //for number of orbits
	float dt = 0.01; //time step
	float dx,dy,a;

	//#####Set up plot window#####
	if(!cpgopen("/XWINDOW")) return 1; //open window
	cpgenv(-10,10,-10,10,1,0); //sets up axes
	cpglab("x (AU)","y (AU)","Jupiter Orbiting the Sun");
	
	//#####Initial Conditions#####
	
	celestial obj[N]; //make planet and sun objects

	obj[0].X = 5.0;
	obj[0].Y = 0.0;
	obj[0].m = 0.1*M;
	obj[1].X = 0.0;
	obj[1].Y = 0.0;
	obj[1].m = 1.0*M;
	
	dx = obj[0].X-obj[1].X; //find radius between objects
	dy = obj[0].Y-obj[1].Y;
	obj[0].R = sqrt( dx*dx + dy*dy );
	
	obj[0].aY = 0.0;
	obj[0].aX = -G*M/(obj[0].R*obj[0].R);
	obj[0].Vy = sqrt(G*M/(obj[0].R)); //finds tangential in m/s
	obj[0].Vx = 0.0;
	
	dx = obj[1].X-obj[0].X; //find radius between objects
	dy = obj[1].Y-obj[0].Y;
	obj[1].R = sqrt( dx*dx + dy*dy );
	
	obj[1].aY = 0.0;
	obj[1].aX = G*obj[0].m/(obj[1].R*obj[1].R);//-G*obj[0].m/(obj[1].R*obj[1].R*obj[1].R)*dx;
	obj[1].Vx = 0.0;
	obj[1].Vy = -(obj[0].m/obj[1].m)*obj[0].Vy; //base speed of sun off of planet speed, proportional to mass diference
	
	cpgsci(7); //yellow
	cpgpt(1,&obj[1].X,&obj[1].Y,2); //draw star orbit position
	cpgsci(1); //white
	cpgpt(1,&obj[0].X,&obj[0].Y,2); //draw starting position marker
	
	//std::cout <<"\n###########Calculated Properties###########\nR = " <<obj[0].R << "AU\t\tR = "<<obj[1].R<<"\n";
	
	tempY=obj[0].Y;
	//#####Loop Calculations#####
	for(i=0; i<10000000; i++) { //using leapfrog method to calculate orbit
		for(j=0;j<N;j++) { //pick body affected
			for(k=0;k<N;k++) { //pick body acting on selected one
				if(j==k) continue; //skip itself
				
				//Leapfrog with position first
				
				obj[j].X += (dt/2)*obj[j].Vx;
				obj[j].Y += (dt/2)*obj[j].Vy;
				
				dx = obj[j].X-obj[k].X;
				dy = obj[j].Y-obj[k].Y;
				
				
				obj[j].R = sqrt( dx*dx + dy*dy );
				
				a = -G*obj[k].m/(obj[j].R*obj[j].R);
				
				obj[j].aX = a * dx / obj[j].R;
				obj[j].aY = a * dy / obj[j].R;
				
				obj[j].Vx += obj[j].aX*dt;
				obj[j].Vy += obj[j].aY*dt;
				
				obj[j].X += (dt/2)*obj[j].Vx;
				obj[j].Y += (dt/2)*obj[j].Vy;
			}
		}
		//std::cout<<"Ax: "<<obj[1].aX<<" Ay: "<<obj[1].aY<<"\n";
		cpgsci(7); //yellow
		cpgpt(1,&obj[1].X,&obj[1].Y,1); //draw star orbit position
		cpgsci(6); //white
		cpgpt(1,&obj[0].X,&obj[0].Y,1); //draw starting position marker
		usleep(500); //slows program to show progression of orbit
		if(obj[0].Y>=0.0&&tempY<0.0) { //counts and prints the number of orbits
			numOrb++;
			std::cout<<"Orbits: "<<numOrb<<"\n";
		}
		tempY=obj[0].Y;
	}
	cpgclos(); //pause, require user to return to close plot
}
