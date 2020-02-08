# CMU 15-462 - Scotty3D

Welcome to Scotty3D! This 3D graphics software implements interactive mesh
editing, realistic path tracing, and dynamic animation. Implementing the
functionality of the program constitutes the majority of the coursework for
15-462/662 Computer Graphics at Carnegie Mellon University.

Further information is available in the User Guide and Developer Manual, both
available on the project wiki accessible via tab at the top of the Github.com UI.

[Link to the wiki:](https://github.com/cmu462/Scotty3D/wiki)

[developers guide](https://github.com/cmu462/Scotty3D/wiki/Developer-Manual)

[users guide](https://github.com/cmu462/Scotty3D/wiki/User-Guide)

[build instructions](https://github.com/cmu462/Scotty3D/wiki/Build-Instructions)



# Some implementation details I've had time for so far

## Linear Subdivision:

1. Here is the initial Beetle mesh:

![LinearSubdivision](pics/subdivision/initialBeetle.png)

2. Here it is after 1 round of linear subdivision:

![LinearSubdivision](pics/subdivision/linearSubDivBeetle.png)


## Catmul Clark, naive schemeL

1. Here is the initial CrazyTorus mesh:
![LinearSubdivision](pics/subdivision/CrazyTorusInitial.png)

2. Here is the Catmull refined CrazyTorus:
![LinearSubdivision](pics/subdivision/CrazyTorusCatmullClark3level.png)

Of course we have barely scratched the surface with subdivision.

### Subdivision things to do:

1. Boundary conditions

2. Matrix subdivision operators

3. Stamm's eigenvalue based fast evaluation

4. Other fast evaluators (B-spline patch perhaps...)

### Other things to do:

1. Add discrete differential geometric operators

2. Add automatic differentiation in some way yet to be determined.

3. ?think real hard?

4. variational constrained multiresolution subdivision discrete differential geometry for geometric design?

