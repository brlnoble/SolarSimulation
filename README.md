# SolarSimulation

Final project for a scientific computing course. Work in progress.

The end goal is a galaxy simulation with N=10^6 bodies. Algorithms for estimating forces between objects will have to be implemented, approximating by center of mass and distance, grouping bodies far away together to ease calculations.

Work to do:
- [x] Simple two body simulation
- [x] Plotting using PGPLOT
- [x] Basic class to handle objects
- [x] Multiple object interaction with N > 2 solar system
- [x] Random generator for initial positions and mass
- [X] Proper generator for initial conditions of velocity (May need some tweaking)
- [x] More advanced class with star colour depedent on mass, ect.
- [ ] Collision handling between bodies (Very rare event, not a priority)
- [x] Simple approximation of dark halo
- [ ] Implementation of dark halo - *Working on*
- [ ] Advanced plotting using Python and matplotlib (May be scraped)
- [ ] Implementation of Barnes-Hut approximation to simply calculations
- [X] Galaxy of N > 100
- [ ] Galaxy of N > 1000
- [ ] Galaxy of N = 1 000 000

Current progress is a small galaxy with 100 objects in orbit around a supermassive black hole. Red is small mass stars, yellow is medium sized ones. The graph draws the orbits with individual points, currently showing the supermassive black hole at the center as purple.
![November 26th Progress](https://github.com/brlnoble/SolarSimulation/blob/master/November_26_2019.PNG?raw=true)
