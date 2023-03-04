# GASMechanics
### Unreal Engine Gameplay Ability System Basics.
![GASMechanicsScreen](https://user-images.githubusercontent.com/17081096/222907930-b81118fb-e202-462b-b102-f90d95ea0fd8.png)

This little project shows very basics of the ability system i.e. how the ASC entities can work together in essence not pretending on an ideal scalable approach, currently it is not optimized for multiplayer. 

To work with source files you may also require https://git-lfs.com 

The project was created by using **4.27**, to migrate to **5.1** do the next:

* After downloading go to the root folder and right click on 'Mechanics.uproject' open it with notepad and change the version to 5.1 like this "EngineAssociation": "5.1" save and close.
* Double click on 'Mechanics.uproject' and wait a little while the compiler does the work, after compiling UE 5.1 should start automatically.
* To generate VS project files right click on the 'Mechanics.uproject' and click on 'Generate Visual Studio project files'.
* **Troubleshooting**. After migration to 5.1 you may have a problem when the enemies do not walk. If so, in the 'Content Browser' go to: All->Content->Levels and open the 'GameLevel' up. In 'Outliner' select 'NavMeshBoundsVolume' and pull up and down the Z-ax of the gizmo and tap P you should see the green area that specifies the walking area for the enemies. Now it should work, but if not, delete 'NavMeshBoundsVolume' and place the new one with the same 'Transform' parameters as before.

Free assets was taken from:
* StarterContent
* ActionRPG Game
* InterfaceAndItemSounds
* InfinityBladeEffects
* HumanVocalizations
* FXVarietyPack
* CityOfBrassEnemies

### Essence.
At the very start you could struggle with understanding why we need GAS and what is the essence of it. You can think of it as described below:

![GAS1](https://user-images.githubusercontent.com/17081096/222914646-118840a2-92da-4d9e-9176-21248312137f.png)

Every type of game can be considered as an environment and actors placed inside. The actors have a set of **Attributes** (characteristics, properties) and can interact with each other, that means just exchanging the numbers. The numbers by itself can be positive or negative but this information is not enough. To describe the meaning of the number that you send to the actor you need additional metadata. This metadata can include duration, attribute to apply, value etc and is packed inside **GameplayEffect**. To be able to send **GameplayEffect** you need the ability to do it, how exactly you will send the **GameplayEffect** is described in **GameplayAbility**.

![GAS2](https://user-images.githubusercontent.com/17081096/222915434-3d180c28-64d8-4c4f-b499-83c2dfceee8f.png)

To get the comprehensive and detailed explanation of GAS components go to https://github.com/tranek/GASdocumentation
