<p align="center">
  <img src="https://github.com/user-attachments/assets/85affe7b-d9c2-4e5f-af73-dea36243c64f" alt="TailorMade" />
</p>

**TailorMade** is a *high-performance* C++20 **ECS** library with on-the-fly entity creation and **JSON-based** definitions for entities, components, and their relationships.  
It supports **snapshots** for saving and reloading, powerful **entity tagging**, and **custom types** that simplify your workflow.

## Table of Contents
- [Features](#features)
- [Architecture](#architecture)
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
<img width="2807" height="1643" alt="image" src="https://github.com/user-attachments/assets/4f6bd637-b9d7-4003-baa2-bc5250a05ecd" />
**NB**: There are some differences between this diagram and the final implementation.  
During development, I made a few *implementation choices* that aren’t reflected in the diagram.

## Documentation

## Usage Example

## Contributing

