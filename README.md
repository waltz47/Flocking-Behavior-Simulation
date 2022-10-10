# Flocking-Behavior-Simulation
Boids Algorithm Flocking simulation on Unreal Engine 4
# Setup
 - Right click on 'boidsAlgorithm.uproject' and generate VS project files
 - Open project in UE4 and accept build requests
 - Launch project
# Simulation
Settings to simulate flocking behavior:
- AreaExtent (1000 * 1000): Size of simulation volume
- Boids (1000) - Number of boid instances to use 
- Max Velocity (2000) - Maximum velocity for boids
- Cohesion Distance (1300) - Min separation for cohesion. Smaller value will result in smaller flocks.
- Separation Distance (200) - Min separation for boids to avoid others.
- Alignment Distance (300) - Min separation for boids to align  with others
- Cohesion Factor (1000) - Weight factor for cohesion
- Separation Factor (110) - Weight factor for separation. 
- Alignment Factor (80) - Weight factor for alignment

Settings can be modified in the BP_Director Blueprint.

![boidsimu](https://user-images.githubusercontent.com/54764474/194859753-98356133-9e77-4966-aabc-c56dc4c5491c.gif)
