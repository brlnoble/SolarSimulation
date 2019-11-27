#include<cpgplot.h>
#include<iostream>
#include<cmath>
//#include<unistd.h>
#include<cstdlib>
#include"celestial.h"

//WORK ON VARIABLE TIME STEP TO AVOID SLINGSHOTTING
//WORK ON DARK HALO APPROXIMATION

using namespace std;
int rand();
float Random(float a, float b); //random function created to help simplify code

int main() {
	//const float G=6.67e-11, M=2.0e30, AU=1.5e11, SecYear=3.154e7; // real constants
	const float G=1.0, M=1.0, AU=1.0, SecYear=1.0;
	int i,j,k,N=500; //number of iterations per circle, loop iterator, number of orbits
	float dt = 0.2; //time step
	float dx,dy,a,e=0.001; //e is to prevent 0 division
	float scale = 2000.0*N/5.0; //scale of graph, follows number of bodies present
	float adjust=0.04; //adjusts the velocity generator, needs tweaking depending on number of bodies present

	//##### Set up plot window #####
	if(!cpgopen("/XWINDOW")) return 1; //open window
	cpgenv(-scale,scale,-scale,scale,1,0); //sets up axes
	cpglab("x (AU)","y (AU)","Galaxy System Test");
	
	//###### Set up Initial Conditions Here #####
	celestial obj[N]; //create array of bodies using self made Celestial class
	
	obj[0].m = 1000.0*M; //supermassive blackhole at centre of galaxy, do not move it
	obj[0].X = 0.0;
	obj[0].Y = 0.0;
	obj[0].state = true;
	
	cpgsci(6); //draw blackhole position
	cpgpt(1, &obj[0].X, &obj[0].Y,23);
	
	for(i=1; i<N; i++) { //create rest of objects
		obj[i].state = true;
		obj[i].m = Random(0.25, 3.0)*M;
		obj[i].updateCol();
		obj[i].X = Random(-400.0, 400.0)*(N/5.0); //scales with number of bodies to help space them out
		obj[i].Y = Random(-400.0, 400.0)*(N/5.0);
		obj[i].Vx = adjust; //variation in velocitiese, also corrects for exceptionally high initial conditions
		obj[i].Vy = adjust;
		dx = obj[i].X; 
		dy = obj[i].Y;
		obj[i].Vy *= sqrt(G*(obj[i].haloMass()*obj[i].m)/sqrt(dx*dx+e*e))*(dx/sqrt(dx*dx+e*e));
		obj[i].Vx *= -sqrt(G*(obj[i].haloMass()*obj[i].m)/sqrt(dy*dy+e*e))*(dy/sqrt(dy*dy+e*e));
		//cout<<"Body "<<i<<"\n\tm: "<<obj[i].m<<"\n\tX,Y: "<<obj[i].X<<","<<obj[i].Y<<"\n\tVx,Vy: "<<obj[i].Vx<<","<<obj[i].Vy<<"\n\tState: "<<obj[i].state<<"\n";
		//cpgpt(1,&obj[i].X,&obj[i].Y,2); //draw initial positions
	}
	
	//##### Loop calculations ######
	for(i=0; i<10000000; i++) { //runs for an excessively long time
		for(j=1;j<N;j++) { //pick body affected, start at 1 as black hole is not moving
			if(obj[j].state == false) continue; //skip dead objects, happens with collisions
			cpgsci(0); //erase curent position before drawing next one
			cpgpt(1,&obj[j].X,&obj[j].Y,17);
		
			for(k=0;k<N;k++) { //pick body acting on selected one
				if(j==k) continue; //skip itself
				if(obj[k].state == false) continue; //skip dead stars that have been absorbed in collisions
				
				//Leapfrog with position first
				
				obj[j].X += (dt/2)*obj[j].Vx;
				obj[j].Y += (dt/2)*obj[j].Vy;
				
				dx = obj[j].X-obj[k].X;
				dy = obj[j].Y-obj[k].Y;
				
				a=0.0;
				obj[j].R = sqrt( dx*dx + dy*dy ); //add e^e to prevent R from being zero, prevents zero division
				
				if(k == 0) { //VERY BASIC COLLISION HANDLING - NEEDS WORK
					if(obj[j].R <= 0.5*AU) { //black hole collision
						obj[j].collision(obj[k]);
						cout<<"Collision between "<<j<<" and "<<k<<"\n";
						continue;
						}
					}
					else { //other body collision
						if(obj[j].R <= 0.1*AU) {
							obj[j].collision(obj[k]);
							cout<<"Collision between "<<j<<" and "<<k<<"\n";
							continue;
						}
					}
				
				
				a += -G*(obj[k].m*obj[j].m)/(obj[j].R*obj[j].R);
				if(k==0) { //dark matter approximation
					a = -G*(obj[j].haloMass()*obj[j].m)/(obj[j].R*obj[j].R);
				}
				
				obj[j].aX = a * dx / obj[j].R;
				obj[j].aY = a * dy / obj[j].R;
				
				obj[j].Vx += obj[j].aX*dt;
				obj[j].Vy += obj[j].aY*dt;
				
				obj[j].X += (dt/2)*obj[j].Vx;
				obj[j].Y += (dt/2)*obj[j].Vy;
			} //END CALCULATION GROUP
			
			if(obj[j].state == true) { //only draw living bodies, not those absorbed by collisions
				cpgsci(obj[j].colour); //follows colour of the given body
				cpgpt(1,&obj[j].X,&obj[j].Y,17); //draw the current position as a point
			}
		} //END BODY CHOOSING LOOP
		//usleep(500); //slows down plotting so orbits can be traced at a relatively easy to see speed
	} //END MAIN LOOP
}

//###### Random Generator ######

float Random(float a, float b) {
	float r = ( (float) rand() ) / ( (float) RAND_MAX );
	float range = b - a;
	return r*range + a;
}
