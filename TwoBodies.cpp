#include<cpgplot.h>
#include<iostream>
#include<cmath>
#include<unistd.h>
#include"celestial.h"

using namespace std;

int main() {

	const float G=1.0, M=1.0;
	//const float G=6.67e-11, M=2.0e30, AU=1.5e11, SecYear=3.154e7; // real constants
	int nIter = 1000,i,j,k,N=2; //number of iterations per circle, loop iterator, number of orbits
	float tempY; //for number of orbits
	float dt = 0.01; //time step
	float dx,dy,a;
	float e = 0.01; //offset to try and avoid slingshotting
	float scale = 10.0; //scale of graph

	//#####Set up plot window#####
	if(!cpgopen("/XWINDOW")) return 1; //open window
	cpgenv(-scale,scale,-scale,scale,1,0); //sets up axes
	cpglab("x (AU)","y (AU)","Binary System, 1*M and 0.5*M");
	
	//#####Initial Conditions#####
	
	celestial obj[N]; //make planet and sun objects

	cout<<"Please enter the (X,Y) position and mass of the first body\n";
	cin >>obj[0].X >>obj[0].Y >>obj[0].m;
	do { //validate user input for first body
		cin.clear();
		cin.ignore(200, '\n');
		cout<<"Please enter the (X,Y) position and mass of the first body"<<endl;
		cin >>obj[0].X >>obj[0].Y >>obj[0].m;
	} while(cin.fail());
	cout<<"Please enter the (X,Y) position and mass of the second body\n";
	cin >>obj[1].X >>obj[1].Y >> obj[1].m;
	do { //validate user input for second body
		cin.clear();
		cin.ignore(200, '\n');
		cout<<"Please enter the (X,Y) position and mass of the first body"<<endl;
		cin >>obj[0].X >>obj[0].Y >>obj[0].m;
	} while(cin.fail());
	
	dx = obj[1].X-obj[0].X; //second body
	dy = obj[1].Y-obj[0].Y;
	obj[1].R = e*e + sqrt( dx*dx + dy*dy );
	obj[1].Vy = sqrt(G*M/sqrt(dx*dx+e*e))*dx/sqrt(dx*dx+e*e);
	obj[1].Vx = sqrt(G*M/sqrt(dy*dy+e*e))*dy/sqrt(dy*dy+e*e); 
	
	obj[0].Vx = 0.0; //first body
	obj[0].Vy = -(obj[1].m)*obj[1].Vy;//base speed of first body off of second one
	
	cpgsci(1); //yellow
	cpgpt(1,&obj[0].X,&obj[0].Y,2); //draw first body starting positon marker
	cpgpt(1,&obj[1].X,&obj[1].Y,2); //draw second body starting positon marker
	
	cout <<"\n-----------------Initial State-----------------\n";
	cout <<"(X,Y) : ("<<obj[0].X<<","<<obj[0].Y<<")\t\t(Vx,Vy): ("<<obj[0].Vx<<","<<obj[0].Vy<<")\t\tM: "<<obj[0].m<<"\n";
	cout <<"(X,Y) : ("<<obj[1].X<<","<<obj[1].Y<<")\t\t(Vx,Vy): ("<<obj[1].Vx<<","<<obj[1].Vy<<")\t\tM: "<<obj[1].m<<"\n";
	
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
				
				
				obj[j].R = e*e + sqrt( dx*dx + dy*dy );
				
				a = -G*obj[k].m/(obj[j].R*obj[j].R+e*e);
				
				obj[j].aX = a * dx / obj[j].R;
				obj[j].aY = a * dy / obj[j].R;
				
				obj[j].Vx += obj[j].aX*dt;
				obj[j].Vy += obj[j].aY*dt;
				
				obj[j].X += (dt/2)*obj[j].Vx;
				obj[j].Y += (dt/2)*obj[j].Vy;
			}
		}
		cpgsci(7); //yellow
		cpgpt(1,&obj[0].X,&obj[0].Y,1); //draw star orbit position
		cpgsci(6); //purple
		cpgpt(1,&obj[1].X,&obj[1].Y,1); //draw starting position marker
		//usleep(500); //slows program to show progression of orbit
	}
	cpgclos(); //pause, require user to return to close plot
}
