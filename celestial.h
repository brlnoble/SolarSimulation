#include<cmath>
class celestial {
public:
	float mass; //mass in solar units, our sun will be 1.0
	float x,y,vX,vY; //position in velocity according to axis
	float T,R,w; //period,radius from center of mass,angular velocity
	float vT; //tangential velocity
	float radStep; //radial step between samples
	float thetaC; //theta old and theta current
	float dt;
	
	float a,b,e; //elliptical properties of orbit
	
	void updateV() {
		vX=-vT*sin(thetaC);
		vY=vT*cos(thetaC);
	}
	void updatePos() {
		x+=vX*dt;
		y+=vY*dt;
	}
};