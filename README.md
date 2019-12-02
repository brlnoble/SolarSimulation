# SolarSimulation

Final project for a scientific computing course. Work in progress.

The end goal is a galaxy simulation with N=10^6 bodies. Algorithms for estimating forces between objects will have to be implemented, approximating by center of mass and distance, grouping bodies far away together to ease calculations.

Work to do:
- [x] Simple two body simulation
- [x] Plotting using PGPLOT
- [x] Basic class to handle objects
- [x] Multiple object interaction with N > 2 solar system
- [x] Random generator for initial positions and mass
- [X] Proper generator for initial conditions of velocity - *(May need some tweaking)*
- [x] More advanced class with star colour depedent on mass, ect.
- [X] Collision handling between bodies - *(Very rare event, very simple collisions handled)*
- [x] Simple approximation of dark halo
- [X] Implementation of dark halo - *(Working, may need tweaking in future)*
- [ ] Advanced plotting using Python and matplotlib - *(May be scraped)*
- [ ] Implementation of Barnes-Hut approximation to simply calculations
- [X] Galaxy of N > 100
- [X] Galaxy of N > 1000
- [ ] Galaxy of N = 1 000 000

Current progress is a small galaxy with 1000 objects in orbit. Red is small mass stars, yellow is medium sized ones, blue are high mass stars. The graph draws the orbits with individual points, currently with a low level approximtion for the dark halo. Below is a picture of the galaxy after a long duration of the simulation.
![Final Progress](https://github.com/brlnoble/SolarSimulation/blob/master/Long_Life_Elliptical.PNG?raw=true)
