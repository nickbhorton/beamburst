### Goals
The main goal of this project is to continue my education in C++, CMake, graphics programming, 
and medium-sized project organization. 

## I <3 trees
There are several very interesting tree-like problem structures.
in ray tracing. 

#### Bounding Volume Hierarchy
The most performance-critical tree is the bounding volume hierarchy. Which can be represented by a tree.
Instead of checking if each ray intersects with each object in the scene, it is much faster to check if each ray 
intersects with a box that contains all objects in your scene. Then, if it does intersect with n boxes containing 
approximately 1/n objects from the scene. This gives the number of intersection checks a logarithmic nature, which is
much faster than a linear nature. 

#### Light Tree
Most object materials can be approximated mathematically. Generally, one uses some equation to calculate the approximate
diffuse lighting and specular highlights. This looks good, but where the ray tracing really shines is in its mirror and glass simulations.
The first thing a naive ray tracer will do is represent a reflection sequence with a vector of intersections. This makes sense until 
implementation of light, permiable materials. At each intersection with a light-permeable object, part of the ray is reflected, and part
of the ray is refracted. Then two new intersections with two new (weaker) rays need to be calculated. So simulating light in a scene
With these objects, light creates a tree of light. To effectively calculate the pixel value from this tree, a depth first
walk is performed.

### Dependencies
`libpng`

### Examples
![example 4](https://github.com/nickbhorton/beamburst/blob/main/resources/example4.png?raw=true)
![example 3](https://github.com/nickbhorton/beamburst/blob/main/resources/example3.png?raw=true)
