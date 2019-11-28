#include<cmath>
class celestial {
public:

	float m; //mass in solar units, the Sun will be 1.0
	float X,Y,Vx,Vy; //position in velocity according to axis in units of AU/year
	float R; //radius from center of mass, intermediate variable for calculations
	float aX, aY; //X and Y acceleration components
	bool state; //used to determine if the current body is alive or not, used for collisions
	
	int colour; //colour used to plot the object, dependent on mass to replicate
	
	void updateCol() { //change colour depending on mass, attempting to replicate real star colours
		if(m<1.0) colour = 2; //red
		else if(m>=1.0 && m<5.0) colour = 7; //yellow
		else if(m>=5.0 && m<10.0) colour = 5; //blue
		else colour = 1; //white
	};
	void collision(celestial bod) { //kill one body, moving its mass to the other one
		if(bod.m == 1000.0) state = false;
		else if(m >= bod.m) {
			bod.state = false;
			m += bod.m;
			updateCol();
			//Vx -= bod.Vx*(X/R); //currently not used as this event is extremely rare
			//Vy -= bod.Vy*(Y/R);
		}
		else {
			state = false;
			bod.m += m;
			bod.updateCol();
			//bod.Vx -= Vx*(X/R);
			//bod.Vy -= Vy*(Y/R);
		}
	};
	float haloMass() { //dark halo approximation, mass it assumed to be distributed linearly
		float r = sqrt(X*X + Y*Y);
		return 2000.0*r;
	};
};
