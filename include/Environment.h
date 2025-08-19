/**
 * @file Environment.h
 * Project TailorMade
 * @author Thomas K/BIDI
 * @version 2.0
 */


#ifndef _ENVIRONMENT_H
#define _ENVIRONMENT_H

#include <ComponentManager.h>
#include <EntityManager.h>
#include <Subscription.h>

/**
 * Structure of the Snapshot, store the data of the desired components on a subset of entities.
 * You can then use it to restore its state in the current environment.
 */
typedef struct Snapshot {
    /// Contain every data of the desired components for the registered entities.
    std::unordered_map<std::string, std::vector<std::pair<std::string, dataVector>>> entities; 
} Snapshot;

/**
* @file Environment.h
* @brief Environment implementation
*
* @details This Environment class is a huge interface for the EntityManager and ComponentManagers.
* @details It can be used to update every aspect of this ECS environment, with the possibility to share the updates accross the environment for the systems.
* @details The Environment also give you the possibility to make and load snapshots.
*/


class Environment {
public: 
    /**
     * @brief Constructor of an Environment without files, the only need is an EntityManager.
     * @param entityManager The EntityManager which will be used by this environment.
     */
    Environment(std::shared_ptr<EntityManager> entityManager);
    
    /**
     * @brief Constructor which take the paths of the JSON's description of the entities, components and subscriptions of this ECS environment. 
     * @details The entire environment settings are loaded from these files.
     * @param entitiesPath The entities' root directory.
     * @param componentsPath The components' root directory.
     * @param subscriptionsPath The subscriptions' root directory.
     */
    Environment(const std::string& entitiesPath, const std::string& componentsPath, const std::string& subscriptionsPath);
    
    /**
     * @brief Let you add a ComponentManager from the environment interface.
     * @param manager A shared_ptr towards the ComponentManagers which will be added to the environment.
     */
    void addManager(std::shared_ptr<ComponentManager> manager);
    
    /**
     * @brief Return all the ComponentManagers.
     */
    std::vector<std::shared_ptr<ComponentManager>> getManagers();
    
    /**
     * @brief Return a specific ComponentManager.
     * @param name The name of the desired ComponentManager.
     */
    std::shared_ptr<ComponentManager> getManager(const std::string& name);
    
    /**
     * @brief Return the EntityManager.
     */
    std::shared_ptr<EntityManager> getEntityManager();
    
    /**
     * @brief Let you set an entity's state with its name.
     * @details Apply this state to every components of this entity.
     * @param name Entity's name.
     * @param state The new state to apply.
     * @param share Tells the method if you want the update to be shared to the systems. (default : true)
     */
    void setEntityState(const std::string& name, bool state, bool share = true);
    
    /**
     * @brief Let you set an entity's state with its ID.
     * @details Apply this state to every components of this entity.
     * @param entity Entity's ID.
     * @param state The new state to apply.
     * @param share Tells the method if you want the update to be shared to the systems. (default : true)
     */
    void setEntityState(int entity, bool state, bool share = true);
    
    /**
     * @brief Let you update the state of all entities with a certain prefix in their name or with a certain tag.
     * @details Apply this state to every components of these entities.
     * @param prefixOrTag Prefix or tag for the research of entities.
     * @param state The new state to apply.
     * @param share Tells the method if you want the update to be shared to the systems. (default : true)
     * @param isPrefix Tells if the first parameter should be considered as a prefix or a tag.
     */
    void setEntitiesState(const std::string& prefixOrTag, bool state, bool share = true, bool isPrefix = true);
    
    /**
     * @brief Set the state of a specific component for an entity, with its ID.
     * @param entity Entity's ID.
     * @param compName The name of the component.
     * @param state The new state to apply.
     * @param share Tells the method if you want the update to be shared to the systems. (default : true)
     */
    void setState(int entity, const std::string& compName, bool state, bool share = true);
    
    /**
     * @brief Set the state of a specific component for an entity, with its name.
     * @param name Entity's name.
     * @param compName The name of the component.
     * @param state The new state to apply.
     * @param share Tells the method if you want the update to be shared to the systems. (default : true)
     */
    void setState(const std::string& name, const std::string& compName, bool state, bool share = true);

    /**
     * @brief Set the state of a set of entities for a specific component.
     * @details The entities can be gathered from a prefix or from a tag.
     * @param prefixOrTag Prefix or tag for the research of entities.
     * @param compName The name of the component.
     * @param state The new state to apply.
     * @param share Tells the method if you want the update to be shared to the systems. (default : true)
     * @param isPrefix Tells if the first parameter should be considered as a prefix or a tag.
     */
    void setStates(const std::string& prefixOrTag, const std::string& compName, bool state, bool share = true, bool isPrefix = true);
    
    /**
     * @brief Return the state of an entity's component, with its ID.
     * @param entity Entity's ID.
     * @param compName The name of the component.
     */
    bool getState(int entity, const std::string& compName);
    
    /**
     * @brief Return the state of an entity's component, with its name.
     * @param entityName Entity's name.
     * @param compName The name of the component.
     */
    bool getState(const std::string& name, const std::string& compName);
    
    /**
     * @brief Return an entity's ID from its name.
     * @param name Entity's name.
     */
    int getEntity(const std::string& name);

    /**
     * @brief Return an entity's name from its ID.
     * @param entity Entity's ID.
     */
    const std::string& getName(int entity);

    /**
     * @brief Create a new entity and return its ID.
     * @details If createFile is true, the entity's file will be create in the root directory.
     * @param name Entity's name.
     * @param createFile If true, an entity's file is created in the root directory.
     * @param share Tells the method if you want the update to be shared to the systems. (default : true)
     */
    int createEntity(const std::string& name, bool createFile = false, bool share = true);

    /**
     * @brief Remove an entity from the EntityManager.
     * @warning The removed entity's ID will be reused.
     * @param name Entity's name.
     * @param share Tells the method if you want the update to be shared to the systems. (default : true)
     */
    void removeEntity(const std::string& name, bool share = true);
    
    /**
     * @brief Return all the components from an entity, with its ID.
     * @param entity Entity's ID.
     */
    std::vector<std::shared_ptr<Component>> getComponents(int entity);
    
    /**
     * @brief Return all the components of an entity, with its name.
     * @param name Entity's name.
     */
    std::vector<std::shared_ptr<Component>> getComponents(const std::string& name);
    
    /**
     * @brief Return the component of a specific entity, with its ID.
     * @param entity Entity's ID.
     * @param name The name of the component.
     */
    std::shared_ptr<Component> getComponent(int entity, const std::string& name);
    
    /**
      * @brief Return the component of a specific entity, with its name.
      * @param name Entity's name.
      * @param name The name of the component.
      */
    std::shared_ptr<Component> getComponent(const std::string& entityName, const std::string& name);
    
    /**
     * @brief Return true if the given entity possess the given component, false otherwise.
     * @param name Entity's name.
     * @param compName Component's name.
     */
    bool hasComponent(const std::string& name, const std::string& compName);
    
    /**
     * @brief Return true if the given entity possess the given component, false otherwise.
     * @param entity Entity's ID.
     * @param compName Component's name.
     */
    bool hasComponent(int entity, const std::string& compName);

    /**
     * @brief Return true if the entity have the given tag, false otherwise.
     * @param entity Entity's ID.
     * @param tag The tag to search for.
     */
    bool hasTag(int entity, const std::string& tagName);

    /**
     * @brief Return true if the entity have the given tag, false otherwise.
     * @param name Entity's name.
     * @param tag The tag to search for.
     */
    bool hasTag(const std::string& name, const std::string& tagName);

    /**
     * @brief Add a tag to an entity.
     * @param entity  Entity's ID.
     * @param tag The tag to add.
     * @param share Tells the method if you want the update to be shared to the systems. (default : true)
     */
    void addTag(int entity, const std::string& tag, bool share = true);

    /**
     * @brief Add a tag to an entity.
     * @param name  Entity's name.
     * @param tag The tag to add.
     * @param share Tells the method if you want the update to be shared to the systems. (default : true)
     */
    void addTag(const std::string& entity, const std::string& tag, bool share = true);

    /**
     * @brief Let you save the subscription of an entity in a file, it will preserve the current values of the components, therefore it can be used as a saving system.
     * @details If the file is already present in the directory folder or its subfolders, it will be replaced, otherwise it will be created in the directory folder of the subscriptions.
     * @param entity The ID of the entity to save.
     */
    void save(int entity);
    
    /**
     * @brief Let you save the subscription of an entity in a file, it will preserve the current values of the components, therefore it can be used as a saving system.
     * @details If the file is already present in the directory folder or its subfolders, it will be replaced, otherwise it will be created in the directory folder of the subscriptions.
     * @param name The name of the entity to save.
     */
    void save(const std::string& name);
    
    /**
     * @brief Let you join the Environment's update list.
     * @details This callback will be called every time a notification is send.
     * @param callback Function which take an entity's ID and decide if it should be added to a system or not.
     * @param ID System's ID, to identify its callback.
     */
    void join(std::function<void(int)> callback, size_t ID);
    
    /**
     * @brief Will notify every callbacks of an update in the environment.
     * @brief Only the updated entity is concerned.
     * @param entity The updated entity to share.
     */
    void notify(int entity);
   
    /**
    * @brief Will share with a specific system all the entities currently in the environment.
    * @details If the callback doesn't exist, do nothing.
    * @details Let you update entirely (or resync) a system's entities list.
    * @param ID System's ID, identify its callback.
    */
    void notify(size_t ID);

    /**
     * @brief Let you create a new entity, which is a perfect copy of the original.
     * @details The new entity will have the name "copy" and its ID is returned.
     * @param original The name of the original entity.
     * @param copy The desired name for the copy.
     * @param createFile If true, an entity's file is created in the root directory.
     * @param share Tells the method if you want the update to be shared to the systems. (default : true)
     */
    int copy(const std::string& original, const std::string& copy, bool createFile = false, bool share = true);
    
    /**
     * @brief Give the ownership, or make a copy, of an entity's component to another component.
     * @param component Name of the component to give.
     * @param giver The ID of the entity which give its component.
     * @param receiver The ID of the entity which take the component.
     * @param copy If true the component is just copied, otherwise the giver doesn't have the component anymore.
     * @param share Tells the method if you want the update to be shared to the systems. (default : true)
     */
    void give(const std::string& component, int giver, int receiver, bool copy, bool share = true);
    
    /**
     * @brief Make and store a snapshot of the desired entities and components.
     * @details By default, toSave and components are empty, this means that every entities and every components should be saved.
     * @details The snapshot will be saved with the name "snapshotName", if it already exist the old snapshot is replaced.
     * @param snapshotName Name of the snapshot.
     * @param toSave List of entities to save, empty means all.
     * @param components List of components to save, empty means all those of the entities.
     */
    void makeSnapshot(const std::string& snapshotName, const std::vector<std::string>& toSave = {}, const std::vector<std::string>& components = {});
    
    /**
     * @brief Load the snapshot from the given name if it exist.
     * @brief All the entities saved and the components saved of the snapshot will replace the current ones.
     * @param snapshotName Name of the snapshot.
     */
    void loadSnapshot(const std::string& snapshotName);

    /**
     * @brief Let you clear a snapshot from its name.
     * @param snapshotName Name of the snapshot.
     */
    void clearSnapshot(const std::string& snapshotName);

private: 
    /**
     * Link the ComponentManagers to their names.
     */
    std::unordered_map<std::string, std::shared_ptr<ComponentManager>> mapNC;

    /**
     * The EntityManager used in this environment.
     */
    std::shared_ptr<EntityManager> entityManager;

    /**
     * The Subscription instance of the current Environment.
     */
    std::shared_ptr<Subscription> subscription;

    /**
     * Vector of callback to notify changes inside the environment to the systems.
     */
    std::unordered_map<size_t, std::function<void(int)>> notifiers;

    /**
     * Link the snapshots to their names.
     */
    std::unordered_map<std::string, Snapshot> snapshots;
};

#endif //_ENVIRONMENT_H