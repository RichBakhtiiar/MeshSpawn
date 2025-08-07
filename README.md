# Project Tools Plugin for Unreal Engine 5.2

Plugin *ProjectTools* allows you to automatically and manually create and edit objects in the *World Outliner* menu hierarchy.

Available functions:
- Setup Lighting Content
- Auto Setup Folder
- Auto Setup Pack
- Setup Affect Distance Field Lighting
  
![Screenshot_1](https://github.com/RichBakhtiiar/ProjectTools/assets/141634532/174c5892-d04d-4fc4-a2ca-435699a84441)

## Installation guide

Go to your project folder and create a new folder with the name “Plugins”. Download and insert the plugin “ProjectTools” folder inside your “Plugins” folder. The plugin is automatically turned on in the project now.
![image](https://github.com/RichBakhtiiar/ProjectTools/assets/141634532/c38aef92-c7da-47a1-b881-9094cec7b6e6)

## Settings

Before you start working with *ProjectTools*, you need to set up rules for naming objects and creating folders in the *World Outliner* hierarchy.

To edit the plugin settings, go to the appropriate section **Edit → Editor Preferences**.

![Screenshot_2](https://github.com/RichBakhtiiar/ProjectTools/assets/141634532/deae5c0c-060b-4262-aa90-0f77bc4ded59)

On the left side of the Editor Preferences window, find the category *ProjectTools* and click in the section **ProjectTools**.

![Screenshot_3](https://github.com/RichBakhtiiar/ProjectTools/assets/141634532/64c9dbfb-d9db-4b41-bae0-ebf27e9ea630)

## Setup Lighting Content

The section provides you with the ability to automatically create needed *Lighting* on the scene. Please, use the default list of the items in this section.

## Auto Setup Folder

The section provides you with the ability to sort every object with the prefixes in the *World Outliner*. You can add additional prefixes if needed and the name of the folder, in which objects with those prefixes will be placed by plugin. Example: if you have a point light with the name “SM_Doors_Door_01”, the plugin will set the folder path to the “Meshes/Door/SM_Doors_Door_01” in the *World Outliner*.

![Screenshot_4](https://github.com/RichBakhtiiar/ProjectTools/assets/141634532/198d2734-de89-4dba-85c5-c0063a9c68ec)

## Auto Setup Pack

The section provides you with the ability to name the folder in which all of the objects without the prefixes will be placed.

## Setup Affect Distance Field Lighting

The section provides you with the ability to set to true / false Affect Distance Filed Lighting boolean on all static meshes on the current level. Useful for Virtual Production purposes.
