syntax
=========

A small sandbox renderer for testing ideas / things / stuff to do with opengl.
Set up with Microsoft's Visual Studio 2013 in mind. Code "should" work on other platforms / IDEs but will need to be correctly linked with relevant files.

Before building run cmake on glfw within the dep folder, then build it in Release and Debug configurations via the GLFW.sln solution.

Uses the Autodesk FBX SDK version 2015.1

Current dependancies:
  - GLM (sub-module)
  - GLFW (sub-module)
  - GLEW
  - STB (sub-module)
  - FBX SDK (not contained within /dep/ folder)
