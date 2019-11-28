#include<cpgplot.h>
#include<iostream>
#include<cmath>
#include<cstdlib>
#include"celestial.h"


using namespace std;
int rand();
float Random(float a, float b); //random function created to help simplify code
int posOrNeg(); //randomly generates +1 or -1

//------- -------
int main() {
	//const float G=6.67e-11, M=2.0e30, AU=1.5e11, SecYear=3.154e7; // real constants, not used
	const float G=1.0, M=1.0;
	int i,j,k,N=1000; //loop iterators and number of bodies
	float dt = 1; //time step
	float dx,dy,a,e=0.001; //e is to prevent 0 division
	float scale = 2000.0*N/7.0; //scale of graph, follows number of bodies present
	float adjust=0.007; //adjusts the velocity generator to prevent slingshot orbits
	//float angMom=0.0,angMomI=0.0; //used for testing angular momentum


	//------- Set up plot window -------
	if(!cpgopen("/XWINDOW")) return 1; //open window
	cpgenv(-scale,scale,-scale,scale,1,0); //sets up axes
	cpglab("X","Y","Galaxy System Test (N=1000)");
	cout<<"---------Simple Galaxy Simulation---------\n";
	
	
	//------- Set up Initial Conditions Here -------
	celestial obj[N]; //create array of bodies using self made Celestial class	
	for(i=0; i<N; i++) { //create rest of objects
		obj[i].state = true;
		obj[i].m = Random(0.25, 3.0)*M;
		obj[i].updateCol();
		obj[i].X = posOrNeg()*Random( 50.0, 500.0)*(N/5.0); //scales with number of bodies to help space them out
		obj[i].Y = posOrNeg()*sqrt(500.0*500.0*(N*N/25.0)-obj[i].X*obj[i].X)*Random(0.1, 1.0);//*Random( 10.0, 500.0)*(N/5.0);
		obj[i].Vx = adjust; //variation in velocitiese, also corrects for exceptionally high initial conditions
		obj[i].Vy = adjust;
		dx = obj[i].X; 
		dy = obj[i].Y;
		obj[i].Vy *= sqrt(G*(obj[i].haloMass()*obj[i].m)/sqrt(dx*dx+e*e))*(dx/sqrt(dx*dx+e*e));
		obj[i].Vx *= -sqrt(G*(obj[i].haloMass()*obj[i].m)/sqrt(dy*dy+e*e))*(dy/sqrt(dy*dy+e*e));
		//cout<<"Body "<<i<<"\n\tm: "<<obj[i].m<<"\n\tX,Y: "<<obj[i].X<<","<<obj[i].Y<<"\n\tVx,Vy: "<<obj[i].Vx<<","<<obj[i].Vy<<"\n\tState: "<<obj[i].state<<"\n";
		cpgsci(obj[i].colour); //follows colour of the given body
		cpgpt(1,&obj[i].X,&obj[i].Y,17); //draw the current position as a point
		//angMomI += obj[i].m*sqrt(obj[i].X*obj[i].X+obj[i].Y*obj[i].Y)*sqrt(obj[i].Vx*obj[i].Vx+obj[i].Vy*obj[i].Vy);
	}
	//cout<<"Initial Angular Momentum: "<<angMomI<<"\n";
	
	//------- Loop calculations -------
	for(i=0; i<10000000; i++) { //runs for an excessively long time
		//angMom = 0.0;
		for(j=0;j<N;j++) { //pick body affected, start at 1 as black hole is not moving
			if(obj[j].state == false) continue; //skip dead objects, happens with collisions
			cpgsci(0); //erase curent position before drawing next one
			cpgpt(1,&obj[j].X,&obj[j].Y,17);
			
			//Dark halo approximation
				obj[j].X += (dt/2)*obj[j].Vx;
				obj[j].Y += (dt/2)*obj[j].Vy;
				
				dx = obj[j].X;
				dy = obj[j].Y;

				obj[j].R = sqrt( dx*dx + dy*dy + e*e); //add e^e to prevent R from being zero, prevents zero division			
				a = -G*(obj[j].haloMass()*obj[j].m)/(obj[j].R*obj[j].R);
				
				obj[j].aX = a * dx / obj[j].R;
				obj[j].aY = a * dy / obj[j].R;
				
				obj[j].Vx += obj[j].aX*dt;
				obj[j].Vy += obj[j].aY*dt;
				
				obj[j].X += (dt/2)*obj[j].Vx;
				obj[j].Y += (dt/2)*obj[j].Vy;
			
			//Loop for other bodies
			for(k=0;k<N;k++) { //pick body acting on selected one
				if(j==k) continue; //skip itself
				if(obj[k].state == false) continue; //skip dead stars that have been absorbed in collisions
				
				//Leapfrog with position first
				
				obj[j].X += (dt/2)*obj[j].Vx;
				obj[j].Y += (dt/2)*obj[j].Vy;
				
				dx = obj[j].X-obj[k].X;
				dy = obj[j].Y-obj[k].Y;
				
				obj[j].R = sqrt( dx*dx + dy*dy + e*e); //add e^e to prevent R from being zero, prevents zero division
				
				if(obj[j].R <= 1.0) { //handles the very rare event of a collision
							obj[j].collision(obj[k]);
							cout<<"Collision between "<<j<<" and "<<k<<"\n";
							continue;
				}				
				
				a = -G*(obj[k].m*obj[j].m)/(obj[j].R*obj[j].R);
				
				obj[j].aX = a * dx / obj[j].R;
				obj[j].aY = a * dy / obj[j].R;
				
				obj[j].Vx += obj[j].aX*dt;
				obj[j].Vy += obj[j].aY*dt;
				
				obj[j].X += (dt/2)*obj[j].Vx;
				obj[j].Y += (dt/2)*obj[j].Vy;
			} //END CALCULATION GROUP
			//angMom += obj[j].m*sqrt(obj[j].X*obj[j].X+obj[j].Y*obj[j].Y)*sqrt(obj[j].Vx*obj[j].Vx+obj[j].Vy*obj[j].Vy);
			
			if(obj[j].state == true) { //only draw living bodies, not those absorbed by collisions
				cpgsci(obj[j].colour); //follows colour of the given body
				cpgpt(1,&obj[j].X,&obj[j].Y,17); //draw the current position as a point
			}
		} //END BODY CHOOSING LOOP
		//cout<<"AngMom: "<<angMom<<"\t\tI: "<<angMomI<<"\n"; //used to track angular momentum throughout simulation
		//usleep(500); //slows down plotting so orbits can be traced at a relatively easy to see speed
	} //END MAIN LOOP
}

//------- Random Generator -------

float Random(float a, float b) {
	float r = ( (float) rand() ) / ( (float) RAND_MAX );
	float range = b - a;
	return r*range + a;
}

int posOrNeg() {
	int x = rand()%2;
	return -2*x+1;
}
