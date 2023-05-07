

README

# 3DViewer Project in Qt/CPP

The Viewer3D project is a C++ based 3D viewer application that enables users to load 3D models and perform various transformations and rendering operations on them. This README file contains important information about the project, including installation instructions, usage, development, and licensing.

## Build

To build the project, follow these steps:

1. Ensure that CMake version 3.15 or higher is installed on your system.
2. Clone the project repository to your local machine.
3. Open a terminal window and navigate to the project directory.
4. Create a new build directory within the project directory.
5. Navigate to the build directory and run the command `cmake ..` to generate the makefile.
6. Run the command `make` to build the project.

## Dependencies

The Viewer3D project has the following dependencies:

- CMake version 3.15 or higher
- C++ (CXX) and C languages
- CMake_INCLUDE_CURRENT_DIR variable set to ON
- CMake_AUTOUIC, CMake_AUTOMOC, and CMake_AUTORCC variables set to ON
- C standard version 11
- C++ standard version 17
- assimp library
- Qt libraries (version 5 or 6), specifically the Widgets and Gui components
- OpenGL, Qt5Core, Qt5Widgets, Qt5Gui, and Qt5OpenGL libraries (for Windows systems)
- QtOpenGL and OpenGLWidgets components (for non-Windows systems)

## Usage

The Viewer3D application provides the following functionalities:

1. Model Loading: The application allows loading 3D models from different file formats such as obj, stl, fbx, dae, etc. These models can include vertices, surfaces, normals, textures, and other attributes.
 
2. Transformations: Translation, rotation, and scaling of models are essential features in the application. Users can move, rotate or scale models to create animations or position them in the desired location in the scene.

3. User Interface: The application provides a graphical user interface (GUI) for easy interaction with the software. The GUI allows users to select different tools and options, and to modify various properties of the models, such as lighting, materials, textures, and rendering options.

To use the Viewer3D application, follow these steps:

1. Launch the application.
2. Load a 3D model by selecting the "File" menu and choosing "Open Model."
3. Use the transformation tools to move, rotate or scale the loaded model.
4. Modify the rendering options as desired using the options available in the GUI.
5. Save the modified model if needed by selecting the "File" menu and choosing "Save Model."

Development:

If you would like to contribute to the development of the 3DViewer project, please follow these steps:

1. Fork the project on GitHub.
2. Clone your forked repository to your local machine.
3. Make changes to the code and test them locally.
4. Commit your changes and push them to your forked repository.
5. Create a pull request on the main repository.

Please note that all contributions must be made under the project's license, which is described below.

License:

The Viewer3D project is licensed under the [MIT License](LICENSE). This means that the project can be used for both commercial and non-commercial purposes, and that anyone is free to modify and distribute the source code. However, the project is provided "as is", without warranty of any kind, and the authors of the project will not be held liable for any damages arising from the use of the project.

<h1 align="center"> 3DViewer </h1>

<h2 align="center"> Main window view </h2>

<img src="data/image/mainwindow.png">

1. Object display area
2. Basic application settings
3. Object/scene control area
4. Light control area
5. Texture control area

<p>With the three areas described above, it is possible to perform various manipulations (display over the object display area, close/open, resize and reposition), for example:</p>

<img src="data/image/randomplace.png">

<hr>

<h2 align="center"> Basic Settings </h2>
<h3><b> File tab: </b></h3>

1. Load the model from a file (obj format)
2. Take a snapshot of the current state of the display area, which opens an additional window (screenshot below) for choosing the image format and resolution that the user can set manually or select from corresponding presets. 

<img src="data/image/savewindow.png">

<hr>

<h3><b> Model tab: </b></h3>

1. Edges
    - Display mode (default is none)
        - none - do not display edges on the object
        - line - display edges as lines
    - Size - change the thickness of lines
    - Color - change the color of lines
2. Vertices
    - Display mode (default is none)
        - none - do not display vertices on the object
        - Circle - display vertices as circles
        - Square - display vertices as squares
    - Size - change the radius of points
    - Color - change the color of points
3. Textures
    - Display mode (default is surface)
        - none - do not display textures
        - Wireframe - display a wireframe model of the object
        - Surface - display the object as surfaces

<p> When changing the line thickness, an auxiliary window is displayed with a visual representation of the thickness and the ability to change it: </p>

<img src="data/image/changevertex.png">
<hr>
<h2 align="center"> Examples: </h2>

| Edge display method |  | Fin thickness |  | Rib color |  |
| :---: | :---: | :---: | :---: | :---: | :---: |
| `none` | <img src="data/image/defaultmodel.png"> | `defaut` | <img src="data/image/withlinesmodel.png"> | `defaut` | <img src="data/image/withbiglinesmodel.png"> |
| `Линия` | <img src="data/image/withlinesmodel.png"> | `default++` | <img src="data/image/withbiglinesmodel.png"> | `pink` | <img src="data/image/withpinklinesmodel.png"> |



| Way of displaying vertices |  | Vertex Radius |  | Vertex color |  |
| :---: | :---: | :---: | :---: | :---: | :---: |
| `none` | <img src="data/image/defaultmodel.png"> | `defaut` | <img src="data/image/withpointsmodel.png"> | `defaut` | <img src="data/image/withpointsmodel.png"> |
| `Круг` | <img src="data/image/withbigpointsmodel.png"> | `default++` | <img src="data/image/withbigpointsmodel.png"> | `pink` | <img src="data/image/withcoloredpointsmodel.png"> |
| `Квадрат` | <img src="data/image/withrectmodel.png"> |

| Texture rendering method | `none` | `Frame` | `Surface` |
| :---: | :---: | :---: | :---: | 
| | <img src="data/image/notexturemodel.png"> | <img src="data/image/surfacemodel.png"> | <img src="data/image/model.png"> |

<hr>

<h3><b> Scene tab: </b></h3>

1. Color - Change the color of the scene (displayed correctly only when environment is disabled)
2. Lighting
    - Flat shading - even shading across the entire polygon
    - Smooth shading - uses a gradient to change the color of the polygon
3. Projection type
    - Central
    - Parallel
4. Grid (coordinate axes and squares in space)
    - none
    - Display
5. Smoothing
    - Hard edges
    - Soft edges
6. Environment (landscape around the object)
    - none
    - Display

<hr>
<h2 align="center"> Examples: </h2>

| `Stage yellow` | `Stage green` |
| :---: | :---: | 
| <img src="data/image/noskybox.png"> | <img src="data/image/greenscene.png"> |

| `Flat shading` | `Soft shading` |
| :---: | :---: | 
| <img src="data/image/hardlight.png"> | <img src="data/image/softlight.png"> |

| `Central projection` | `Parallel Projection` |
| :---: | :---: | 
| <img src="data/image/centralprojection.png"> | <img src="data/image/parallelprojection.png"> |

| `Show grid` | `Grid none` |
| :---: | :---: | 
| <img src="data/image/centralprojection.png"> | <img src="data/image/nogrid.png"> |

| `Display environment` | `Environment none` |
| :---: | :---: | 
| <img src="data/image/centralprojection.png"> | <img src="data/image/noskybox.png"> |

<hr>

<h2 align="center"> Object Control Area </h2>

<table align="center">
<tr> 
    <td> <img src="data/image/menutransformation.png"> </td>
    <td><p>
        1-2. Offset along X-axis <br>
        3-4. Offset along Y-axis <br>
        5-6. Offset along Z-axis <br>
        1. Offset step <br>
        2. Current coordinate on X-axis <br>
        3. Current coordinate on Y-axis <br>
        4.  Current coordinate on Z-axis <br>
        5.  Rotation around X-axis <br>
        6.  Rotation around Y-axis <br>
        7.  Rotation around Z-axis <br>
        8.  Proportional model scaling along X-axis <br>
        9.  Proportional model scaling along Y-axis <br>
        10. Proportional model scaling along Z-axis <br>
        11. Proportional model scaling along all axes simultaneously <br>
        12. Scales object dimensions to unity size <br>
        13. Focuses camera on selected object <br>
        14. When selecting the scene, user will modify parameters of all objects <br>
        15. When selecting a specific object, user will modify parameters of that object <br>
        16. When selecting a specific part of an object, user can only modify materials for that part <br>
        17. Clicking the remove field next to an object removes it from the scene
    <p></td>
</tr>
</table>

<hr>
<h2 align="center"> Examples: </h2>

| `Оригинал` | `Смещение по X` | `Смещение по Y` | `Смещение по Z` |
| :---: | :---: | :---: | :---: | 
| <img src="data/image/centralprojection.png"> | <img src="data/image/movex.png"> | <img src="data/image/movey.png"> | <img src="data/image/movez.png"> |
| `Поворот по X` | `Поворот по Y (по Z 45)` | `Поворот по Z` | `Total scale` |
| <img src="data/image/rotatex.png"> | <img src="data/image/rotatey.png"> | <img src="data/image/rotatez.png"> | <img src="data/image/scaletotal.png"> |
| `Scale X` | `Scale Y` | `Scale Z` | `Normalize` |
| <img src="data/image/scalex.png"> | <img src="data/image/scaley.png"> | <img src="data/image/scalez.png"> | <img src="data/image/normalize.png"> |

<hr><hr>

<h2 align="center"> Light Control Area </h2>

<table align="center">
<tr> 
    <td> <img src="data/image/menulight.png"> </td>
    <td><p>
        1. Object emission color <br>
        2. Diffuse color of scattered light <br>
        3. Color of reflected light <br>
        4. Direction of the light source <br>
        5. Position of the light source in space <br>
        6. Ray distance <br>
        7. Inner circle of illuminated space <br>
        8. Outer circle of light scattering zone <br>
        9. Global light source status <br>
        10. Positional light source status and their list <br>
        11. Narrow directional positional light source status and their list
    <p></td>
</tr>
</table>

<hr>

<h2 align="center"> Texture Control Area </h2>

<table align="center">
<tr> 
    <td> <img src="data/image/menutexture.png"> </td>
    <td><p>
        1. Background lighting <br>
        2. Diffuse lighting <br>
        3. Lighting of specular highlights <br>
        4. Glossiness coefficient <br>
        5. Transparency coefficient <br>
        6. Refraction coefficient <br>
        7. Reflection coefficient <br>
        8. Roughness coefficient <br>
        9. Set default values for all coefficients<br>
        10. Enable/disable display of the uploaded texture<br>
        11. Flip the texture<br>
        12. Add a texture<br>
        13. Remove a texture
    <p></td>
</tr>
</table>

<hr>
<h2 align="center"> Examples: </h2>

| `Specular Highlight` | `Reflection` | `Transparency` |
| :---: | :---: | :---: |
| <img src="data/image/highlight.png"> | <img src="data/image/reflect.png"> | <img src="data/image/refract.png"> |

<hr>
