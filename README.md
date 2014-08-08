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
* We have another new branch:newUIFramework, in which the Qt GUI has been implemented. Qt API for OpenGL has been finished. It means that we can show mesh or point cloud in Qt now.
* Registration UI/window has been finished and now it could show two point cloud.
* Hole Filling has been added by Shawn.

Authors
---
* Advisor: David Gu
* Coder: Jerome Jiang
* Coder: Jason Zhang
* Coder: Shawn Cui

