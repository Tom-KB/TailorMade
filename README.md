<p align="center">
  <img src="https://github.com/user-attachments/assets/85affe7b-d9c2-4e5f-af73-dea36243c64f" alt="TailorMade" />
</p>

**TailorMade** is a C++20 **ECS** library with on-the-fly entity creation and **JSON-based** definitions for entities, components, and their relationships.  
It supports **snapshots** for saving and reloading, powerful **entity tagging**, and **custom types** that simplify your workflow.

## Table of Contents
- [Features](#features)
- [Architecture](#architecture)
- [Dependencies](#dependencies)
- [Documentation](#documentation)
- [Usage Example](#usage-example)
- [Contributing](#contributing)

## Features
Here’s a list of what TailorMade offers:
 - **JSON-based** definitions for entities, components and subscriptions (*relationships*)
 - An easy way to create **Systems**
 - **Automatic update** of the entities in the Systems, based on your criteria
 - **Custom types** such as : **Vector2** and **Vector3** (more to come) with multiples operators for them
 - **Tagging System** for entities and subscriptions
 - **Snapshots** where you can precise the entities and/or the components to save, then load them back
 - The ability to **organize** JSON definitions across multiple folders
 - **On-the-fly instantiation** of entities and components in code
 - The ability to **save and reload** entire entity data through the library

## Architecture
Here is the class diagram I designed and used to build this project :
<img width="2807" height="1638" alt="image" src="https://github.com/user-attachments/assets/0c0590e7-7097-4a4c-8bef-c820fd1f28a6" />
**NB**: There are some differences between this diagram and the final implementation.  
During development, I made a few *implementation choices* that aren’t reflected in the diagram.

## Dependencies
This library depends on [nlohmann/json](https://github.com/nlohmann/json).  
You need to install it before building this project.

## Documentation
There is also a [documentation](https://tom-kb.github.io/TailorMade/annotated.html), generated with Doxygen.  
Here is a brief description of each class :
 - [Component](https://tom-kb.github.io/TailorMade/class_component.html) : Representation of a component, with its data and their types
 - [ComponentManager](https://tom-kb.github.io/TailorMade/class_component_manager.html) : Manager for each component, used to add/remove a component to an entity
 - [EntityManager](https://tom-kb.github.io/TailorMade/class_entity_manager.html) : Manage every entity in the ECS environment, store the entity's name, ID and tags
 - [Subscription](https://tom-kb.github.io/TailorMade/class_environment.html) : Load the entities' components and data through JSON files
 - [System](https://tom-kb.github.io/TailorMade/class_subscription.html) : Abstract class to create systems, can automatically retrieve components through criteria
 - [Environment](https://tom-kb.github.io/TailorMade/class_system.html) : Huge interface for the entire ECS environment, with a system to propagate the updates on entities to the systems

## Usage Example

### Entities files
Firslty, you can just create a file and give the name of one entity :
```cpp
{
    "name" : "Object"
}
```
Secondly, you could tell the EntityManager that you want to make multiple copies of this entity :
```cpp
{
    "name" : "Object",
    "generate" : 50,
}
```
It will create 50 entities with their name being **ObjectX** with X ranging from 1 to 50 (included).  
Lastly, you could define multiple entities in a single file : 
```cpp
{
    "names" : [
        "Villager",
        "Golem",
        "Chicken1",
        "Chicken2"
    ],
    "tags" : ["NPC"]
}
```
It will create each entity in the "names" list.  
Also, you can see that a field *tags* **can** be added, it is a list in which you can define the tags of the entity/entities.  
These tags can then be used to define the components of a set of entity or to retrieve them in the systems.  

### Components files
For every component you will have to give it a name, which must be unique, and give the names and types of its data.
```json
{
    "name" : "Transform",
    "data" : {
        "position" : "vector3",
        "scale" : "float",
        "rotation" : "float"
    }
}
```

#### Types
Here is a list of the **available types for this version** and the accepted variations for their names :
 - **Integer** : *integer*, *int*
 - **Float** : *float*
 - **String** : *string*, *str*
 - **Boolean** : *boolean*, *bool*
 - **Vector2** : *vector2*
 - **Vector3** : *vector3*

### Subscription files
You can define a subscription's file for a specific entity :
```json
{
    "entity" : "Object",
    "generated" : true,
    "components" : [
        {
            "name" : "Transform",
            "data" : {
                "position" : [10, -12, 23],
                "scale" : 1.0
            }
        },
        {
            "name": "Shape",
            "data": {
                "type": "square",
                "size": [50, 50],
                "filled": true,
                "color": [255, 255, 255]
            }
        }
    ]
}
```
The **generated** field is *optional*, but if it is *true* the components will be given to every entities with the prefix of the field *entity* (here every **ObjectX** will have these components).

You can also create a subscription's file for one or multiple tags :
```json
{
    "tags" : ["NPC"],
    "state" : true,
    "components" : [
        {
            "name" : "GameAI",
            "data" : {
                "level" : "easy"
            }
        }
    ]
}
```
You also have the **state** parameter which will tell if the components of the file should be set active or not by default.  

Here is a sort of "*design pattern*" to ease your subscription creation :
```json
{
    "name" : "",
    "state" : true,
    "generated" : false,
    "tags" : [],
    "components" : [
        {
            "name" : "",
            "data" : {
                "_" : 0,
                "_" : 0
            }
        }
    ]
}
```

### Environment from files
Once you have created all this files and placed them in separated folders, you can create an environment from there :
```cpp
// Creation of an environment.
shared_ptr<Environment> environment = make_shared<Environment>("../Assets/ECS/Entities", "../Assets/ECS/Components", "../Assets/ECS/Subscriptions");
```
Here, all files from **each** given directory and **their sub-directories**, will be loaded.

### On-the-fly instantiation
Here is an example of how to create a *fully working ECS environment* **from the code** :
```cpp
// On-the-fly instantiation from code
shared_ptr<EntityManager> entityManager = make_shared<EntityManager>();
int newEntity = entityManager->createEntity("NewEntity");

// Creation of an environment from an EntityManager
shared_ptr<Environment> environment = make_shared<Environment>(entityManager);

// Definition of a component from scratch
dataUnMap dataMap; // These values will be considered as the default values for the manager
dataMap["data1"] = { "integer", 0 };
dataMap["data2"] = { "Vector2", Vector2({2.0f, 3.0f}) };
dataMap["data3"] = { "float", 1.618033f };
shared_ptr<Component> componentA = make_shared<Component>("A", dataMap);

// Creation of a manager for our newly created component
shared_ptr<ComponentManager> managerA = make_shared<ComponentManager>(componentA);
managerA->subscribe(newEntity); // Link the entity to the component A

environment->addManager(managerA); // Manager added to the environment
environment->createEntity("EntityA");
```

### Systems
A system can be created by deriving the **System** class and implementing its *run()* method :
```cpp
TestSystem::TestSystem(shared_ptr<Environment> environment) : System(environment) {
  // All the possible methods to precise the system's criteria on entities.
	//this->addComponent("Tilemap");
	this->addComponents({ "Drag", "Transform", "Tilemap"});
	//this->addRejected("Camera");
	//this->addRejects({ "GameAI", "Shape" });
	//this->addTag("NPC");
}

void TestSystem::run() {
	for (auto& entity : entities) {
		cout << environment->getName(entity) << endl;
	}
}
```
You can precise the criteria of your system, see [System](https://tom-kb.github.io/TailorMade/class_system.html) for more details on these methods.  
It will **automatically** update your entities list according to these **criteria**, if you let the *autoUpdate* parameter of the System's class to *true*.

## Contributing
Contributions are more than welcome.  
Feel free to submit bug fixes, new features or optimization.  
However, please **don’t propose your systems in this repository**.  
I’ll be creating a separate repository specifically for this.
