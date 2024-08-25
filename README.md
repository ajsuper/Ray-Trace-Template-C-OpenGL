# Ray-Trace-Template-C-OpenGL
|||IMPORTANT||| I couldn't figure out how to upload the .vscode file so it is uploaded as l.vscode, just remove the l from the file lol.
A simple template for the basis of a ray tracer written in C++ using OpenGL. Feel free to take any of this code and use in your own projects. This was made on Linux however I'm sure you can get it to work on windows with some modifications to the file paths of the libraries. 

make sure to use the g++ compiler to automatically link the libraries, make sure to change the file path as necessary. I am fairly sure that glew and freeglut will need to be downloaded.

The main ray tracing code is handled in fragmentShader.glsl
It automatically handles the following:
  Window resizing/scaling
  Whatever FOV you want
  Camera movement/rotation (Uses WASD QE. A bit choppy due to the way glut handles inputs, too lazy to fix it)
  
