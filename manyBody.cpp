#include<cpgplot.h>
#include<iostream>
#include<cmath>
#include<unistd.h>
#include<cstdlib>
#include"celestial.h"

using namespace std;
int rand();
float Random(float a, float b); //random function created to help simplify code

int main() {

	const float G=1.0, M=1.0;
	//const float G=6.67e-11, M=2.0e30, AU=1.5e11, SecYear=3.154e7; // real constants
	//const float GM = 4*M_PI*M_PI, M=2.0e30; //derived from Kepler's Laws
	int nIter = 1000,i,j,k,numOrb=0,N=10; //number of iterations per circle, loop iterator, number of orbits
	float tempY; //for number of orbits
	float dt = 0.01; //time step
	float dx,dy,a;
	float e = 0.01; //offset to try and avoid slingshotting
	float scale = 15.0; //scale of graph

	//##### Set up plot window #####
	if(!cpgopen("/XWINDOW")) return 1; //open window
	cpgenv(-scale,scale,-scale,scale,1,0); //sets up axes
	cpglab("x (AU)","y (AU)","Galaxy System Test");
	
	cpgsci(1);
	
	//###### Set up Initial Conditions Here #####
	
	celestial obj[N]; //create array of bodies using self made Celestial class
	
	obj[0].m = 1000.0; //supermassive blackhole at centre of galaxy, do not move it
	obj[0].X = 0.0;
	obj[0].Y = 0.0;
	obj[0].state = true;
	
	for(i=1; i<N; i++) {
		obj[i].state = true;
		obj[i].m = Random(0.5, 3.0);
		obj[i].updateCol();
		obj[i].X = Random(-8.0, 8.0);
		obj[i].Y = Random(-0.0, 0.0);
		//obj[i].Vx = Random(-0.01, 0.02);
		//obj[i].Vy = Random(-4.0, -1.0);
		dx = obj[i].X-obj[0].X; //first body
		dy = obj[i].Y-obj[0].Y;
		obj[i].Vy = 0.25*sqrt(G*obj[0].m/sqrt(dx*dx+e*e))*dx/sqrt(dx*dx+e*e);
		obj[i].Vx = -0.25*sqrt(G*obj[0].m/sqrt(dy*dy+e*e))*dy/sqrt(dy*dy+e*e);
		cout<<"Body "<<i<<"\n\tm: "<<obj[i].m<<"\n\tX,Y: "<<obj[i].X<<","<<obj[i].Y<<"\n\tVx,Vy: "<<obj[i].Vx<<","<<obj[i].Vy<<"\n\tState: "<<obj[i].state<<"\n";
		cpgpt(1,&obj[i].X,&obj[i].Y,2);
	}
	
	//##### Loop calculations ######
	for(i=0; i<10000000; i++) { //using leapfrog method to calculate orbit
		for(j=1;j<N;j++) { //pick body affected
		if(obj[j].state == false) continue;
			for(k=0;k<N;k++) { //pick body acting on selected one
				if(j==k) continue; //skip itself
				if(obj[k].state == false) continue;
				//Leapfrog with position first
				
				obj[j].X += (dt/2)*obj[j].Vx;
				obj[j].Y += (dt/2)*obj[j].Vy;
				
				dx = obj[j].X-obj[k].X;
				dy = obj[j].Y-obj[k].Y;
				
				
				obj[j].R = sqrt( e*e + dx*dx + dy*dy ); //add e^e to prevent R from being zero, prevents slingshotting
				if(k == 0) {
					if(obj[j].R <= 0.15) {
						obj[j].collision(obj[k]);
						cout<<"Collision between "<<j<<" and "<<k<<"\n";
						continue;
					}
					else {
						if(obj[j].R <= 0.05) {
							obj[j].collision(obj[k]);
							cout<<"Collision between "<<j<<" and "<<k<<"\n";
							continue;
						}
					}
				}
				
				a = -G*(obj[k].m/obj[j].m)/(obj[j].R*obj[j].R);
				
				obj[j].aX = a * dx / obj[j].R;
				obj[j].aY = a * dy / obj[j].R;
				
				obj[j].Vx += obj[j].aX*dt;
				obj[j].Vy += obj[j].aY*dt;
				
				obj[j].X += (dt/2)*obj[j].Vx;
				obj[j].Y += (dt/2)*obj[j].Vy;
			} //END CALCULATION GROUP
			//cpgeras();
			if(obj[j].state == true) {
				cpgsci(obj[j].colour); //follows colour of the given body
				cpgpt(1,&obj[j].X,&obj[j].Y,1); //draw the current position as a point
			}
		} //END BODY CHOOSING LOOP
		usleep(50000);
	} //END MAIN LOOP
}

//###### Random Generator ######

float Random(float a, float b) {
	float r = ( (float) rand() ) / ( (float) RAND_MAX );
	float range = b - a;
	return r*range + a;
}
