# Flocking-Behavior-Simulation
Boids Algorithm Flocking simulation on Unreal Engine 4
# Setup
 - Right click on 'boidsAlgorithm.uproject' and generate VS project files
 - Open project in UE4 and accept build requests
 - Launch project
# Simulation
Settings to simulate flocking behavior:
- AreaExtent (4000 * 4000 * 400): Size of simulation volume
- Boids (600) - Number of boid instances to use 
- Max Velocity (2000) - Maximum velocity for boids
- Cohesion Distance (1300) - Min separation for cohesion. Smaller value will result in smaller flocks.
- Separation Distance (200) - Min separation for boids to avoid others.
- Alignment Distance (300) - Min separation for boids to align  with others
- Cohesion Factor (500) - Weight factor for cohesion
- Separation Factor (110) - Weight factor for separation. 
- Alignment Factor (80) - Weight factor for alignment
- Avoidance Factor (1000)
- Avoidance Distance (400)

Settings can be modified in BP_Director.
'2' to speed up sim

![boidsimu](https://user-images.githubusercontent.com/54764474/194859753-98356133-9e77-4966-aabc-c56dc4c5491c.gif)
