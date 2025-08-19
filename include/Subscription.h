/**
 * @file Subscription.h
 * Project TailorMade
 * @author Thomas K/BIDI
 * @version 2.0
 */

#ifndef _SUBSCRIPTION_H
#define _SUBSCRIPTION_H

#include <EntityManager.h>
#include <ComponentManager.h>

 /**
 * @file Subscription.h
 * @brief Subscription implementation
 *
 * @details This Subscription class will automatically filled linked entities to their components based of JSON's file definition of suscription and apply the value of these files to the components.
 * @details You can define subscription's files on an entity, a set of generated entity or a specific tag.
 */

using unorMapCM = std::unordered_map<std::string, std::shared_ptr<ComponentManager>>;

class Subscription {
public: 
    /**
     * @brief Constructor of the Subscription class.
     * @warning The given subscriptions of the directory folders (and subfolders) should used Entities of the EntityManager and components of the given ComponentManagers, otherwise they will be ignored.
     * @param directory The root directory of the subscription's files.
     * @param entityManager The environment EntityManager.
     * @param compManagers The environment ComponentManagers.
     */
    Subscription(const std::string& directory, std::shared_ptr<EntityManager> entityManager, std::shared_ptr<unorMapCM> compManagers);
    
    /**
     * @brief Let you save the subscription of an entity in a file, it will preserve the current values of the components, therefore it can be used as a saving system.
     * @details If the file is already present in the directory folder or its subfolders, it will be replaced, otherwise it will be created in the directory folder.
     * @param entity The ID of the entity to save.
     */
    void save(int entity);

private: 
    /**
     * Name of the root directory of the subscription's files.
     */
    std::string directory;

    /**
     * Unordered_map between the name of an entity and the paths to its subscription's file.
     */
    std::unordered_map<std::string, std::string> entitiesFP;

    /**
     * Shared_ptr toward the EntityManager used by this Subscription instance.
     */
    std::shared_ptr<EntityManager> entityManager;

    /**
     * Shared_ptr toward the ComponentManagers of the environment.
     */
    std::shared_ptr<unorMapCM> managers;
};

#endif //_SUBSCRIPTION_H