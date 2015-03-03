/*! \mainpage Manual for 3D MeshViewer and PrimeSense Scanning Software

Though this software doesn't have a formal name right now, it's in the folder of MainSoftware and 3DScannerSoftware. The initial purpose is to build a system for 3D Scanners to finish scanning, registration and surface reconstruction. Though these three parts have been finished separately, a good integration is still needed. This document is mainly focused on the following functionality: Mesh Viewer, Scanning, Registration and Surface Reconstruction.

##Devices##

We use [OPENNI2](http://structure.io/openni) for the driver and APIs, which is an open source project. **OPENNI2** supports PrimeSense and Kinect. So in theory, both should be supported though not tested for Kinect.

A turning table is used for the whole body scan. While the scanner is working, the human body is rotating on the turning table. After a cycle, all the point clouds are recorded and written on the hard disk. At the same time, a time stamp for every frame is also recorded.

## Dependencies and Requirements##

 - UI : [Qt 5](http://qt-project.org/)
	 - Qt5 is a cross platform UI library. To use the software, you must download and install Qt5 and set the corresponded environment variables such as PATH.
 - Linear Algebra Library: Eigen
 - 3D Rendering: OpenGL
 - [Poisson Surface Reconstruction](http://www.cs.jhu.edu/~misha/Code/PoissonRecon/Version6.13/)
 - This software has been compiled and tested under **64-bits** environment. 

## Functionalities ##

Generally, this software provides four major functions: mesh viewer, 3D scanning, registration and surface reconstruction. 

### Mesh Viewer ###

The mesh viewer is simple, though, fully functional. Once the software is started, you'll see the mesh viewer. Supported file formats include ***.obj** and ***.ply**. Point clouds and triangle meshes are both supported. Since the code of Poisson Surface Reconstruction can only process ply files, support of ply is essential. 

The design philosophy is mainly followed as MeshLab, which is a famous mesh viewer and editing software. Different rendering modes, light control and simple editing such as vertices and faces deletion are all supported.

### 3D Scanning ###

If the button of scanning (right end of the toolbar) is clicked, the 3D scanning process is started. First you need to type in the range X you want to scan. Then the point cloud of objects in the range 0-X will be shown in the window. In our experiment, X=800 because we don't want the background also scanned and recorded.

When you are ready, you can start to record the point clouds by clicking the "start scanning" button. While the point cloud is still shown live in the window, it is being recorded and written in the hard disk. The recorded file names would be **scanned_point_cloudY.ply** where Y starting from 0. When you decide to stop the scan, click the button of "stop scanning". Then a file of timerecord.txt will be written, which contains the time stamp of each frame.

Since the conversion from depth image to point cloud is done while the scanning is working, the time interval is limited to about 200ms, which is mentioned before. When the number of vertices in the scanned point cloud increases or decreases suddenly (such as the left or right side of human body is being scanned), the time interval is variable between about 150ms to 300ms. However, it can still guarantee that ICP could align most of the frames automatically. For better results, we can just record the depth image and the conversion could be done offline.

### Registration ###

We support both manual registration and Iterative Closest Point.

#### Manual Registration ####

For Manual Registration, click on the button of "**Align Two Mesh**" whose icon is like two triangles partly overlapped. By the way, the icon design is also an important task but in our project, programmer is the UI designer. So any better icons while their licenses allow would be appreciated.

Then a window containing two viewers is shown. Two "Open" buttons with number 1 and 2 are used to load different point clouds. To decide a rigid transformation, **four pairs of corresponded vertices** need to be selected. Click on the button of "Select" to enter **selection mode**. Then click on the vertex to select it. The selected vertices are rendered in orange. The order selection on the two point clouds must be consistent. For example, the four pairs of vertices are corresponded:
 
- Vertex 10 on Mesh 1, Vertex 12 on Mesh 2
- Vertex 7 on Mesh 1, Vertex 5 on Mesh 2
- Vertex 23 on Mesh 1, Vertex 9 on Mesh 2
- Vertex 97 on Mesh 1, Vertex 62 on Mesh 2
  
Then if you select the vertices on Mesh 1 in the order of (23, 7, 97, 10), you must select the vertices on Mesh 2 in the order of (9, 5, 62, 12). Anytime you don't want to select vertices, e.g. you want to rotate the mesh or zoom, click the button of "Select" again to quit the **Selection Mode**.

After selection, you can click the button "Align" and then, a new window showing two aligned point cloud would appear. You can merge the two point clouds and save it there.

####ICP####

The ICP code included in this package is not the latest version. For reference and latest version, refer to the program "ScanICPDeform".

### Surface Reconstruction ###

We use Poisson Surface Reconstruction mentioned before to complete the task. First open a point cloud in the viewer and then click on the button of "Poisson Surface Reconstruction" whose icon is a little girl. The reconstructed triangle mesh will be shown in the viewer automatically. And the reconstructed surface mesh is saved in the root folder. You may observe that there are some extra surface patches surrounding the desired surface if the surface is not water tight. So we need to use Surface Trimmer.

Just after the surface reconstruction, you may click the button "Trim Mesh". The result is shown directly in the viewer once it finishes.


##Technical Parameters##

For scanning purpose, we need to set some parameters such as scanning interval time and the rotation speed.

 - The scanning interval time is about **200ms**. Since the translation from depth image to point cloud is finished at the same time, the scanning interval time may be variable up to **300ms**.
 - Rotation Speed of the turning table is **25 seconds per cycle**.

*/

#include "mainwindow.h"
#include "alignWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication scannerApp(argc, argv);
    MainWindow mainWin;
    mainWin.setGeometry(100, 100, mainWin.sizeHint().width(), mainWin.sizeHint().height());
    mainWin.resize(mainWin.sizeHint());
    mainWin.showMaximized();
	

    return scannerApp.exec();
}
