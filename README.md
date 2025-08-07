# Mesh Spawn Plugin for Unreal Engine 5.3

The *MeshSpawn* plugin enables physically accurate placement of static meshes in your scene.

<img width="730" height="263" alt="Screenshot_01" src="https://github.com/user-attachments/assets/b46ad7d5-a595-4ce5-8fac-a16038aa41f6" />

## Installation Guide

Go to your project folder and create a new folder with the name “Plugins”. Download and insert the plugin “MeshSpawn” folder inside your “Plugins” folder. The plugin is automatically turned on in the project now.
<img width="534" height="293" alt="Screenshot_02" src="https://github.com/user-attachments/assets/82814690-c997-47e6-84a4-3eb7998512ff" />

## Settings

Before you begin placing meshes using the *MeshSpawn* tool, make sure to configure the following settings properly:

Available functions:

- Brush size - determines the area within which meshes can be randomly spawned when painting on surfaces.
- Place with gravity - when enabled, meshes are spawned with gravity applied, causing them to fall and settle naturally according to the physics simulation.
- Enable offsets - enables position randomization for each spawned mesh. When this option is active, the Min/Max Position, Rotation and Scale Offsets fields become editable, allowing variation in placement.
- Static mesh list - select a mesh from this list to define which asset will be placed during spawning.
- Bake all spawned meshes to instance mesh - converts all currently spawned Static Mesh Actors into Instanced Static Mesh for better performance and optimized rendering.

<img width="392" height="404" alt="Screenshot_03" src="https://github.com/user-attachments/assets/7da65a20-a366-4adc-88db-6db4715a1344" />

## Use Case Scenario: Quickly Populate Wooden Barrels in a Market Scene

You're building a medieval market or dock environment and want to populate the area with wooden barrels scattered around crates, walls, and pathways to add realism and storytelling detail. 

<img width="1498" height="642" alt="Screenshot_04" src="https://github.com/user-attachments/assets/9d7e99c7-248a-496d-b8b4-bd42606d92b1" />

With MeshSpawn Toolkit, you can: 

- Select the Wooden Barrel Static Mesh from the asset list.
- Set a Small-to-Medium Brush Size to focus placement around stalls, corners, or alleys.
- Enable Gravity Placement so barrels land naturally on uneven cobblestone, wooden docks, or stacked crates.
- Turn on Offsets and Random Rotation to make each barrel appear uniquely placed — tilted slightly, pushed aside, or off-center.
- Paint (spawn) intuitively in the viewport to add details quickly without manual dragging.
- Bake to Instanced Meshes once you're happy with the result, ensuring optimal performance and clean scene organization.

This scenario is ideal for filling background spaces without manual placement effort.

<img width="1913" height="644" alt="Screenshot_05" src="https://github.com/user-attachments/assets/4c9b6dac-1821-451b-b430-e942841be8ed" />
