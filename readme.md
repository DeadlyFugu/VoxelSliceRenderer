Voxel Splice Renderer
=====================

Hi there! This is a volumetric renderer that using raycasting. I was thinking like "Oi I should make like a volumetric renderer sometime" so I did that. Here it is.

Does it run a good?
-------------------

Probably not. If I lower the settings a little bit (720p, sample distance thing = 0.006, MAX_DEPTH = 3) I can get a constant 60fps on my 980ti. Don't expect much from middle to low end hardware. Requires OpenGL 3.3 or above... probably.

How to build?
-------------

Requires a directory, 'Dependencies' to be added in with the rest of the source code. Within it should be the glfw3, glew, and glm libraries. It should be structured something like this:

	Dependencies\
		glm\
			... (few dozen glm headers)
		glew.h
		glew32.lib
		glfw3.h
		glfw3.lib
		... (some more glfw and glew crap)

I don't know if I can just push that stuff to the repo also or not, so I'm playing it safe for now.

License
-------

Only took a small amount of time to make, I don't really see the point of putting some fancy license on here. Do whatever you want, I can't see this being overly useful to anyone though. If it is somehow useful, some form of credit or whatnot would be nice I guess.
