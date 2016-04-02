# EECE 478 Computer Graphics: Full City

### Part 3 of 3

This project uses OpenGL and GLUT to model and render various things, from single models to entire scenes.

Part 3 contains multiple building models arranged in a scene to form a sort of city. The goal of part 3 was to create a flyby video.

The final program imports multiple model files and a skybox and arranges them based on the specifications of a separate city file. Only EarthOceanSciences was created by me. As I no longer have access to many of the other models, running the program results in a mostly empty city.

Lighting is a basic representation of an overcast, stormy night. Ambient lighting moves on a gradient to simulate clouds, and lightning flashes are present, but not physically represented.

The viewpoint camera moves along a 4 curve B-Spline for a smooth flyby of the buildings.

The image capture portion of the program is commented out in the included executable. During the flyby, screenshots are taken frame by frame to be later stitched into a final video, which is also included.
