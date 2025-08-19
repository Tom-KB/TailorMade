/**
 * @file EntityManager.h
 * Project TailorMade
 * @author Thomas K/BIDI
 * @version 2.0
 */

#ifndef _ENTITYMANAGER_H
#define _ENTITYMANAGER_H

#include <queue>
#include <unordered_set>
#include <TM_Tools.h>

 /**
 * @file EntityManager.h
 * @brief EntityManager implementation
 *
 * @details This EntityManager class handle every entities of an environment.
 * @details Every entities have a name and an ID, both should be unique.
 * @details The EntityManager also manage the tags on the entities, tags are useful when you need to specify group of entities with non specific components.
 */

class EntityManager {
public:
    /**
     * @brief Default constructor of the EntityManager.
     */
    EntityManager();

    /**
     * @brief Constructor of the EntityManager, take the root directory with the entities as a parameter.
     * @param directory The root directory with all the entities, files in subfolders are include.
     */
    EntityManager(const std::string& directory);
    
    /**
     * @brief Return the ID of an Entity based on its name.
     * @param name Name of the entity.
     */
    int getEntity(const std::string& name);
    
    /**
     * @brief Return every entities with a name which start by the given prefix or all the entities of a tag.
     * @details You can generate multiple entities in the JSON file and manage them from their common prefix.
     * @details If isPrefix is false, prefixOrTag is considered a tag and all entities with the given tag will be returned.
     * @param prefixOrTag Prefix or tag for the research.
     * @param isPrefix Tells if the precedent parameter should be considered as a prefix or a tag.
     */
    std::vector<int> getEntities(const std::string& prefixOrTag, bool isPrefix = true);

    /**
     * @brief Return a vector with the names of all the entities.
     */
    std::vector<std::string> getNames();
    
    /**
     * @brief Return the name of an entity based on its ID.
     * @param entity The ID of the entity.
     */
    const std::string& getName(int entity);
    
    /**
     * @brief Create a new entity and return its ID.
     * @details If createFile is true, the entity's file will be create in the root directory.
     * @param name Entity's name.
     * @param createFile If true, an entity's file is created in the root directory.
     */
    int createEntity(const std::string& name, bool createFile = false);

    /**
     * @brief Remove an entity from the EntityManager.
     * @warning The removed entity's ID will be reused.
     * @param name Entity's name.
     */
    void removeEntity(const std::string& name);
    
    /**
     * @brief Append a serialized version of the EntityManager to the given stream. 
     * @details Consist of the name of the entities and their ID.
     * @param stream The stream on which the serialized version of the EntityManager is append to.
     */
    void toString(std::ostream& stream);
    
    /**
     * @brief Return true if the entity have the given tag, false otherwise.
     * @param entity The ID of the entity.
     * @param tag The tag to search for.
     */
    bool hasTag(int entity, const std::string& tag);

    /**
     * @brief Add a tag to an entity.
     * @param entity The ID of the entity.
     * @param tag The tag to add.
     */
    void addTag(int entity, const std::string& tag);

private: 
    /**
     * Map on the entities' names and IDs.
     */
    std::map<std::string, int> entities;

    /**
     * Fast way to return the name of an entity based on its index.
     * name = names[i] with i the entity's ID.
     */
    std::vector<std::string> names;

    /**
     * A queue with the available IDs, if any.
     * Added when an entity is removed.
     */
    std::queue<int> availableIDs;

    /**
     * The number of entities. 
     * Used for the IDs.
     */
    int count;

    /**
     * The root's folder in which the entities can be found.
     */
    std::string directory;

    /**
     * Placeholder string to be returned as an error.
     * Just an empty string.
     */
    std::string placeholder;

    /**
     * Store the tags of an entity.
     */
    std::unordered_map<std::string, std::unordered_set<int>> tags;
};

#endif //_ENTITYMANAGER_H