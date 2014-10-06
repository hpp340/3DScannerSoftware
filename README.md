3D Scanner Software
=================

This is the software created for 3D scanner used for 3D scan lab in SBU. This software will be used for 3D scanners developed by our lab.

CopyRight Claim
----
* This Software is developed by 3D Scanning group in Dept. of Computer Science in Stony Brook University. 
* No one is allowed to edit or redistribute both the source code and binary file for any kind of use unless authorized by the authors.

Software Overview
---
* GUI will be written in Qt for cross platform use
* We will have Registration algorithm, Poisson Surface Reconstruction, Hole Filling and Remeshing to be added to our software in the future, which makes it usable for our 3D scanning cameras.
* OpenGL 2.1 and above is supported. Also, you will need to have OpenGL installed on your computer to run the software. For installation guide, check back later.

Installation Guide
---
* We need Qt 5.3.1 or above and Microsoft Visual C++ redistributable packages for Visual Studio 2013, which will be included in the published installation package. Or you can [click here](http://www.microsoft.com/en-us/download/details.aspx?id=40784) to download it.
* For Qt 5.3.1, you can go [here](http://qt-project.org/downloads) to download the package. Make sure to download the 64 bit version with OpenGL.
* After the installation of Qt, you need to change the environmental variables to make our software run successfully. To change the environment variables, check [this page](https://www.java.com/en/download/help/path.xml)(English) or [this page](https://www.java.com/zh_CN/download/help/path.xml)(Simplified Chinese).
* Then you can run our installed software.

Development
---
* Now we have added m2plyConverter to the project. See README file in that project.
* We have ManualRegistration Project which complete registration, as indicated by the name. Registration is finished by selecting some points manually since ICP algorithm has been proved not proper for human face applications. Now this project uses GLUT as GUI to interact with mouse and keyboard. Now we are trying to implement OpenGL in Qt to support complex interaction.
* mainWindow is a project written in Qt by Shawn, which is the basic UI framework for our software. This project will be enriched with all other codes and features to make a complete and useable software.
* This is a brand new branch in which the new UI Framwork is written. The new UI is totally code-based (instead of using Qt Designer) so that we can understand the working mechanism in UI designed.
* Qt GUI has been built and Qt API for OpenGL has been finished.
* All my future commits will be added to this branch until we merge the two branches(master and newUIFramework)
* Registration window/UI has been finished and two point clouds could be shown in the window. 
* X-Y-Z axises have been drawn when rendering a point cloud
* Feature of openning a point cloud fild(ply) in both the mainwindow and registration window has been finished
* Feature of selecting vertices in registratioin window has been finished
* Feature of computing the transformation matrix has been finished, and verified to be correct on small point cloud
* Favicon for the app has been added
* The bug of resizing seems to have been fixed.
* Finished computing the transformed mesh
* Finished the window used to show the align result. And the window can show multiple meshes at the same time with different vertex color
* Finished the light control in all windows
* The registration part has been verified to be correct using the large point cloud and human faces.
* Now you can view the mesh in different modes:Points, Wireframes, Flat, Flatlines and Smooth.
* We have added the feature of selecting points & faces in the main window.
* Deleting points and faces in the main window is finished.
* Save edited mesh to file.
* Hole Filling has been added to master branch.
* Obj file is now supported
* There are some bugs in Hole Filling, so it's moved in this branch.
* Now connected to PrimeSense with OpenNI2 SDK!
* ICP point-point has been finished and tested without bugs!

Authors
---
* Advisor: David Gu
* Coder: Jerome Jiang
* Coder: Jason Zhang
* Coder: Shawn Cui

