/**
 * @file ComponentManager.h
 * Project TailorMade
 * @author Thomas K/BIDI
 * @version 2.0
 */

#ifndef _COMPONENTMANAGER_H
#define _COMPONENTMANAGER_H
#include <Component.h>
#include <memory>

 /**
 * @file ComponentManager.h
 * @brief ComponentManager implementation
 *
 * @details This ComponentManager class manage every components for each type of components.
 * @details It will be construct on a component's file.
 * @details You can un/subscribe entity to it. Subscribed entities will have their component with their own data accessible through this ComponentManager.
 */


class ComponentManager {
public:
    /**
     * @brief The main constructor of the ComponentManager, created the components based of the file's description.
     * @param filename Full path towards the component's file.
     */
    ComponentManager(const std::string& filename);
    
    /**
     * @brief Create a ComponentManager with a reference component.
     * @details The resulting file will be named after the component's name.
     * @details Useful when you making on-the-fly components creation.
     * @param component The reference component of this manager, a copy will be created.
     */
    ComponentManager(std::shared_ptr<Component> component);
    
    /**
     * @brief Return the name of the component managed by this instance.
     */
    const std::string& getName();
    
    /**
     * @brief Return a string which give the type of the data.
     * @details Wrapper of the component's method for the save action.
     * @param data Data's name.
     */
    const std::string& getType(const std::string& data);
    
    /**
     * @brief Subscribe an entity to this component.
     * @details Will give this entity's component the default values of its data's types.
     * @param entity The ID of the entity to subscribe.
     */
    void subscribe(int entity);
    
    /**
     * @brief Subscribe an entity to this component with specific values.
     * @param entity The ID of the entity to subscribe.
     * @param data A vector with the data's names and values.
     * @see dataVector
     */
    void subscribe(int entity, dataVector data);
    
    /**
     * @brief Remove the link between an entity and its component.
     * @param entity The ID of the entity to unsubscribe.
     */
    void unsubscribe(int entity);
    
    /**
     * @brief Return the list of entities linked in this component's manager.
     * @details checkState will tell this method if you want the state of the entities' components to be verify, entities with a state at "false" will not be returned.
     * @param int checkState
     */
    std::vector<int> getEntities(bool checkState);
    
    /**
     * @brief Return the component of a specific entity.
     * @param entity The ID of the entity.
     */
    std::shared_ptr<Component> getComponent(int entity);
    
    /**
     * @brief Return true if the given entity possess this component, false otherwise.
     * @param entity The ID of the entity.
     */
    bool hasEntity(int entity);
    
    /**
     * @brief Return the actual state of the given entity's component, true or false.
     * @param entity The ID of the entity.
     */
    bool getState(int entity);
    
    /**
     * @brief Set the state of the given entity's component.
     * @param entity The ID of the entity.
     * @param newState The entity's component new state.
     */
    void setState(int entity, bool newState);
    
    /**
     * @brief Give the ownership, or make a copy, of an entity's component to another component. 
     * @param giver The ID of the entity which give its component.
     * @param receiver The ID of the entity which take the component.
     * @param copy If true the component is just copied, otherwise the giver doesn't have the component anymore.
     */
    void give(int giver, int receiver, bool copy);
    
    /**
     * @brief Add to the given buffer, the serialized version of this ComponentManager, which will consist of the list of entities' ID + their components data.
     * @param stream The stream on which the serialized version of the ComponentManager is append to.
     */
    void toString(std::ostream& stream);

private: 
    /**
     * A map which with the entities' IDs linked to the entity's component and state.
     */
    std::unordered_map<int, std::pair<std::shared_ptr<Component>, bool>> mapEC;

    /**
     * A reference component used to get information about the components of this ComponentManager.
     */
    std::shared_ptr<Component> referenceComp;

    /**
     * Mutex to protect the modification on the ComponentManager, make it usable in thread.
     */
    std::mutex mtx;
};

#endif //_COMPONENTMANAGER_H