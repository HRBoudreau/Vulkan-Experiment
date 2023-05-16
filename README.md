# Vulkan-Experiment
Messing around with Vulkan Graphics API for fun. <br />

![image](https://user-images.githubusercontent.com/76916678/236537080-ce0403b3-6170-4fbf-b733-b087e57ca309.png)

There is a entity component system. Each entity can have a mesh, transform, and a script that runs every frame. It's very easy to add and edit components using an addComponent and getComponent function. 

It's built with cmake so just go to the folder you want to build in, type cmake <path-to-root-directory>, then type make, and it will create an .exe called vkfun. You'll have to adjust cmake to use whatever your compiler is. 
