3D Scanner Software
=================

This is the software created for 3D scanner used for 3D scan lab in SBU. This software will be used for 3D scanners developed by our lab.

CopyRight Claim
----
* This Software is developed by 3D Scanning group in Dept. of Computer Science in Stony Brook University. 
* No one is allowed to edit or redistribute both the source code and binary file for any kind of use unless authorized by the authors.

Development
---
* GUI will be written in Qt for cross platform use
* Now we have added m2plyConverter to the project. See README file in that project.
* We will have Registration algorithm, Poisson Surface Reconstruction, Hole Filling and Remeshing to be added to our software in the future, which makes it usable for our 3D scanning cameras.
* We have ManualRegistration Project which complete registration, as indicated by the name. Registration is finished by selecting some points manually since ICP algorithm has been proved not proper for human face applications. Now this project uses GLUT as GUI to interact with mouse and keyboard. Now we are trying to implement OpenGL in Qt to support complex interaction.
* mainWindow is a project written in Qt by Shawn, which is the basic UI framework for our software. This project will be enriched with all other codes and features to make a complete and useable software.
* OpenGL 2.1 and above is supported. Also, you will need to have OpenGL installed on your computer to run the software. For installation guide, check back later.
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

Authors
---
* Advisor: David Gu
* Coder: Jerome Jiang
* Coder: Jason Zhang
* Coder: Shawn Cui

