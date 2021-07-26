# Landscape visualization

The following is a task given to third year students of the Computational Mathematics and Cybernetics department of the Moscow State University for their Machine Graphics course.

This task was the last of the four tasks from the course. It was not strictly required to complete the course, but it did add quite a number of points to the overall course score - if a student managed to get those additional points, that is.

## Annotation

The purpose of this task is familiarization with the basics of OpenGL 3/4. The following topics should be covered:

* geometrical objects basics: polygonal meshes, height maps (base);
* graphical pipeline, geometric transformations (base);
* shader programs (base);
* VBO/VAO, OpenGL memory objects (base);
* normals calculation, local illumination models (base);
* microrelief imitation (additional);
* shadow mapping (additional);
* rendering to texture (additional).

(A screenshot of a program that has a score of 13 is then presented; the points are awarded to the base stuff, as well as the sky and translucent water)

## 1. Base topics (10 points)

The students are expected to implement:

1. a height map generation algorithm, as well as a landscape coloring algorithm, and
2. a renderer for the generated landscape that uses OpenGL 3+.

The following features are required:

* textured landscape visualization (active by default, should be activated by pressing the "1" key);
* normal visualization (should be activated by pressing the "2" key);
* the landscape should look convincing. Lightning can be computed in a shader or baked into the texture. Lack of lightning will lead to substraction of 5 points.

## 2. Additional topics

The following features are not required, but may add more points:

* sky visualization:
  * using cube or sphere maps - 1 point;
  * time of day simulation and lightning recalculation using spherical harmonics - up to 6 points depending on how realistic this looks.
* shadows (with depth map visualization from the light source - otherwise no points will be awarded):
  * landscape and other object shadowing (simple shadow map) - 3 points;
  * shadow map + PCF - 4 points;
  * more sophisticated and realistic methods - up to 6 points.
* water surface visualization:
  * simple translucent water - 1 point;
  * sky reflections - 1 point;
  * objects reflection - 4 points;
  * water waves - 1 point for simple texture-based imitation, up to 4 points for realistic water simulation;
  * refraction - 2 points.
* additional object visualization:
  * trees - 2-4 points depending on how realistic they look;
  * foilage - 2-4 points depending on how realistic it looks;
  * stones - 1 point;
  * houses, cars - 1-4 points depending on how realistic they look;
  * people and animals - 1-4 points depending on how realistic they look.
* microrelief imitation:
  * simple normal mapping - 1 point;
  * simple normal mapping + mip filtering - 2 points;
  * parallax occlusion mapping - 4 points;
  * GPU tessellation - 2-6 points.
* large-scale visualisation:
  * endless or cyclic landscape - up to 4 points;
  * fog - 1 point;
  * LODs for parts of the landscape and wireframe visualization - 2 points.
* using professional packages for terrain generation and importing heightmaps from them - 1-2 points (note thet this still requires implementing a generator);
* normal visualization with arrows - 2 points.

## 3. Heightmap generation algorithm

The students are expected to implement a [diamond-square algorithm](#references). The height map is generated on a `2**n + 1` grid. One of the most popular height map generation algorithm<sup>[citation needed]</sup> that produces one of the more realistic results<sup>[citation needed]</sup> is the *diamond-square* algorithm, which is a generalization of a single-dimensional *midpoint displacement* algorithm to two dimensions.

The *midpoint displacement* algorithm is described as follows: a line's height is defined randomly at its ends. It is then split at midpoint. The midpoint is then shifted by a random value. The split and shift is then applied to each of the line segments. The algorithm continues until the segment has a size of 1 pixel. The shifts should be proportional to segment lengths. Thus, the new height for a segment midpoint can be calculated as:

```
h = (hL + hR) / 2 + random(-R * l, R * l)
```

Here, `hL` and `hR` are the heights of the left and right endpoints of a segment, and R is the "roughness" parameter for the algorithm.

This algorithm may be generalized to a two-dimensional heightmap as follows: first, the edges of the (square) map are initialized to random values. Next, the map is split into four equal squares. Each of those squares have a single height value set by the first step. The midpoint of the larger square can be calculated by averaging out the edge points, and the midpoints on the sides of the square can be calculated by averaging the corresponding endpoints. The center point is then shifted by a randomized amount (multiplied by the square side length). Then the steps are repeated recursively for inner squares.

The *diamond-square* algorithm introduces another step in the 2-dimensional *midpoint displacement*. The *square* in the name refers to the midpoint displacement step, and is the same as in the *midpoint displacement* algorithm. The *diamond* is performed after that, and is used to determine the midpoint height on the sides of the squares. Four points are used in the *diamond* step: two of them are the endpoints for the corresponding side (for a vertical side they would be "top" and "bottom"), and two others are adjacent square centers (they would be "left" and "right"). Note that the last two points are generated in the *square* step, and should be calculated before the *diamond* step; therefore, the *square* step must be done before the *diamond* step for the same square size.

In order to make the terrain more "realistic", the heigthmap values (normalzed to the `[0.0..1.0]` range) may be squared - this will make lowlands flatter, and the mountains more steep.

## References

1. *Diamond-square.* // Wikipedia. url = [https://en.wikipedia.org/wiki/Diamond-square_algorithm](https://en.wikipedia.org/wiki/Diamond-square_algorithm)

2. Steve Losh. *Terrain generation with diamond square.* // posted at june 27, 2016. url = [https://stevelosh.com/blog/2016/06/diamond-square/](https://stevelosh.com/blog/2016/06/diamond-square/)
