# Project Design


### My Approach
Rasterizing a 3D model onto a 2D screen is essentially just projecting triangles in a 3D space onto a 2D plane. We use triangles because any three points are always coplanar. Points in 3D space that are not coplanar (obviously) do not form a 2D shape, and therefore can't be represented on a screen. To perform this mapping, we draw from the principle behind a pinhole camera- the size of the 2D object on the screen is inversely proportional to its 3D compliment's distance from the camera. We use a technique called perspective projection and define this projection with matrix transformations for computational simplicity.


### LinAlg.h
I started by creating a framework for the linear algebra that is the core of this entire project. I designed structs representing 2D vectors, 3D vectors, 4D vectors, 2x2 matrices, 3x3 matrices, and 4x4 matrices. I defined operators for vector addition, matrix addition, scalar multiplication for both matrices and vectors, matrix composition, and matrix-vector multiplication. I defined the dot product, cross product, and determinant.


### LinAlg.cpp
My getPerspectiveProjectionMatrix() function does as its name implies: returns a Matrix4x4 object that represents a perspective projection matrix. I used this YouTube video as a guide for the math behind this function: https://www.youtube.com/watch?v=U0_ONQQ5ZNM.

Next, I wrote the getProjectedVector() function. It begins by creating a combined rotation matrix based on the camera's orientation angles around the X and Y axes, effectively aligning the world space with the camera's view. The function then translates the 3D point relative to the camera's position to position the camera at the origin. After applying the rotation, the Y-axis is inverted to match the screen's coordinate system (pixels lower on the screen are indexed higher). The 3D vector is then extended to a 4D homogeneous coordinate and multiplied by the perspective projection matrix, which scales the coordinates based on depth. To obtain the final 2D coordinates, a perspective divide is performed by dividing the x and y components by the w component, followed by mapping these normalized device coordinates to the actual pixel positions on the screen.


### Rasterizer.h
I created the triangle struct to conveniently store information about triangles in 3D space that I will later render. Each triangle struct contains three vertices represented as Vector3D values, a color, and a normal vector. This header file also contains a mesh struct which acts as a container for a set of triangles, forming a 3D image.


### Rasterizer.cpp
The fillTriangle() function is responsible for coloring in a single triangle onto the 2D screen. It begins by determining the smallest axis-aligned bounding rectangle that completely contains the triangle by calculating the minimum and maximum X and Y coordinates from the triangle's vertices. To ensure that only pixels within the image boundaries are attempted to be colored, the function clamps these coordinates to the image's dimensions. It then calculates the area of the triangle using the determinant of a 2x2 matrix formed by two of its edges. The function iterates over each pixel within the bounding rectangle and uses barycentric coordinates to determine whether the pixel lies inside of it. If a pixel is inside, it sets the pixel's color accordingly.

The rasterizeMesh() function begins by iterating through each triangle in the mesh and calculating the centroid of each triangle to determine its visibility relative to the camera's position. By checking the dot product between the triangle's normal and the view vector we are able to determine if triangles are facing away from the camera. These triangles are discarded to optimize rendering. For the visible triangles, the function calculates a lighting factor based on the angle between the triangle's normal and the light source, which is used to shade the triangle appropriately, this creates the appearance of a shadow on the mesh. The triangles are then sorted by their depth to ensure correct rendering order, preventing visual bugs. Each triangle's vertices are projected onto the 2D screen using the getProjectedVector() function, and the fillTriangle() function is called to rasterize the projected triangle with the calculated color.

The computeMeshCenter() function calculates the geometric center of a 3D mesh by computing a weighted average of the centroids of all its triangles.

The ensureNormalsFaceOutward() function works for relatively simple meshes and is dependent on all vectors from mesh's centroid to the triangles' centroids facing outwards (if the mesh centroid is outside the mesh, this will not work). Typically, the mesh triangles are defined in the .txt files in the /inputs directory in such a way that their normals are always facing outwards. This happens because the triangle vertices are defined in counterclockwise order when looking directly at the triangle from outside the mesh, but some input files do not follow this pattern, which is the point of this function.

It first computes the mesh's center using the computeMeshCenter() function. Then, for each triangle in the mesh, it calculates the vector from the mesh center to the triangle's centroid. By taking the dot product of this vector with the triangle's normal, the function determines whether the normal is pointing inward or outward. If the dot product is negative, indicating that the normal is facing inward, the normal vector is inverted. This makes sure that all normals consistently point outward, which is essential for accurate lighting and rendering (as described in the previous two function descriptions). The function deals with triangles with zero area (colinear vertices) by setting their normals to zero, preventing potential rendering issues.


### InputHandler.cpp
The getMoveSpeed(), getCamSpeed(), and lightingPrompt() functions prompt the user for data which is received via command line.

The getFileInput() function lists all .txt files in the inputs directory that represent meshes that the user has the option to load. It returns a string representing the path to the selected .txt file. The user is re-prompted if an invalid choice is inputted.

The loadMeshFromFile() function is designed to import a 3D mesh from a .txt file. It begins by attempting to open the given filename and initializes an empty list of Triangle3D objects. The function reads the file line by line. It skips empty lines and those starting with the # character. If a line starts with an exclamation mark, it sets a flag (apply) indicating that the mesh's normals should be adjusted to face outward with the ensureNormalsFaceOutward() function. For each valid line, the function expects nine numerical values representing the coordinates of the triangle's three vertices. These values are used to define three Vec3D objects, which are then used to construct a Triangle3D that is added to the mesh's triangle list. After processing all lines, the function creates a Mesh object from the collected triangles.  Additionally, for the remy.txt file, it applies a translation to position the mesh. I chose to do this because otherwise, remy would be rendered directly above the camera. The function then returns the mesh object.

In all of the .txt files that contain mesh data, I used data from .obj files I found online and rewrote them in a .txt file in such a way that could be easily read by my program.


### main.cpp

The main function prompts for camera movement speed and turn speed. It runs lightingPrompt() to ask the user if they would like to adjust the point light's position to be equal to the camera position; if yes, the point light position is set to the camera position each refresh.

loadMeshFromFile() is executed using the output from getFileInput() to load in the user's choice of mesh. The display loop then begins, re-rasterizing the mesh each cycle with updated data on camera position and orientation and the light source position. Keyboard input is taken and used to support camera movement. At the end of each loop cycle, each pixel in the entire window is refreshed.






