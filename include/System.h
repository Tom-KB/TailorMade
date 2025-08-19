/**
 * @file System.h
 * Project TailorMade
 * @author Thomas K/BIDI
 * @version 2.0
 */


#ifndef _SYSTEM_H
#define _SYSTEM_H

#include <Environment.h>
#include <unordered_set>

class System {
public:  
    /**
     * The constructor of the System, which take the Environment as a parameter.
     * @param environment
     */
    System(std::shared_ptr<Environment> environment, bool autoUpdate = true);
    
    /**
     * UPDATE THAT
     * @param entities
     */
    void newEntity(int entity);
    
    /**
     * Let you change the Environment.
     * @param environment
     */
    void changeEnvironment(std::shared_ptr<Environment> environment);
    
    /**
     * Return the System's ID.
     */
    const size_t& getID();

    /**
     * Return the value of change and, if change is true flipped it back to false.
     */
    bool getChange();

    /**
     * This is an abstract method which will contains the logic of the System and can be call by the user to process on this System's entities and components.
     */
    virtual void run() = 0;

protected: 
    /**
     * The entities of this system.
     */
    std::unordered_set<int> entities;
    /**
     * A shared_ptr towards the Environment.
     */
    std::shared_ptr<Environment> environment;
    /**
     * Represent the components this System want.
     */
    std::vector<std::string> desiredComponents;
    /**
     * Represent the components this System absolutely doesn't want.
     */
    std::vector<std::string> rejectedComponents;
    /**
     * Represent the tags this System want.
     */
    std::vector<std::string> desiredTags;
    /**
     * Mutex for the entities' vector.
     */
    std::mutex mtx;

    /**
     * Let you add, inside a System, which component should be gathered from the Environment.
     * @param name
     */
    void addComponent(const std::string& name);
    /**
     * Version with a list of desired components.
     * @param name
     */
    void addComponents(std::vector<std::string> names);

    /**
     * Let you add a components to the rejected components, if the entity possess at list one of this components it will not be added.
     * @param name
     */
    void addRejected(const std::string& name);
    /**
     * Version with a list of rejected components.
     * @param name
     */
    void addRejects(std::vector<std::string> names);

    /**
     * Let you add, inside a System, which tag should be gathered from the Environment.
     * @param name
     */
    void addTag(const std::string& tagName);
    /**
    * Version with a list of tags.
    * @param name
    */
    void addTags(std::vector<std::string> names);

private:
    /**
     * Boolean variable which tell if a change of the entities occured, accessible via the getChange() method, which will automatically flipped it back if true. 
     * It lets the opportunity to the system's developper to make a verification on the actual list of entities to reupdate it if he wants to, he could also keep its own version of it and decide himself to copy the new element.
     */
    bool change;

    /**
     * Identification of a system for the callbacks in the environment, let the possibility to create your own callbacks or used the default one.
     */
    size_t ID;

    /**
     * Value incremented to get the next ID.
     */
    static size_t nextID;
};

inline size_t System::nextID = 0; // Initialize the nextID at 0.

#endif //_SYSTEM_H