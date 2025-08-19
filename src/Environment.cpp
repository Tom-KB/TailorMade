#include "Environment.h"

using namespace std;

Environment::Environment(shared_ptr<EntityManager> entityManager) : entityManager(entityManager), subscription(nullptr) {

}

Environment::Environment(const string& entitiesPath, const string& componentsPath, const string& subscriptionsPath) {
	try {
		entityManager = make_shared<EntityManager>(entitiesPath);

		vector<string> files = getAllFilesFromDirectory(componentsPath); // Return every files in the directory's folder and its sub-folders

		for (const auto& file : files) {
			addManager(make_shared<ComponentManager>(file));
		}

		subscription = make_shared<Subscription>(subscriptionsPath, entityManager, make_shared<unorMapCM>(mapNC));
	}
	catch (exception& e) {
		cerr << "Environment : " << e.what() << endl;
	}
}

void Environment::addManager(shared_ptr<ComponentManager> manager) {
	mapNC.insert({manager->getName(), manager});
}

std::vector<std::shared_ptr<ComponentManager>> Environment::getManagers() {
	std::vector<std::shared_ptr<ComponentManager>> result;

	for (const auto& [_, value] : mapNC) {
		result.push_back(value);
	}

	return result;
}

shared_ptr<ComponentManager> Environment::getManager(const string& name) {
	if (!mapNC.contains(name)) {
		return mapNC[name];
	}
	return nullptr;
}

shared_ptr<EntityManager> Environment::getEntityManager() {
	return entityManager;
}

void Environment::setEntityState(const string& name, bool state, bool share) {
	int ID = entityManager->getEntity(name);
	this->setEntityState(ID, state, share);
}

void Environment::setEntityState(int entity, bool state, bool share) {
	for (const auto& [_, value] : mapNC) {
		if (value->hasEntity(entity)) value->setState(entity, state);
	}
	
	if (share) notify(entity);
}

void Environment::setEntitiesState(const string& prefixOrTag, bool state, bool share, bool isPrefix) {
	vector<int> IDs = entityManager->getEntities(prefixOrTag, isPrefix);

	for (int id : IDs) {
		setEntityState(id, state, share);
	}
}

void Environment::setState(int entity, const string& compName, bool state, bool share) {
	if (!mapNC.contains(compName) || !mapNC[compName]->hasEntity(entity)) return;

	mapNC[compName]->setState(entity, state);

	if (share) notify(entity);
}

void Environment::setState(const string& name, const string& compName, bool state, bool share) {
	int ID = entityManager->getEntity(name);

	this->setState(ID, compName, state, share);
}

void Environment::setStates(const string& prefixOrTag, const string& compName, bool state, bool share, bool isPrefix) {
	vector<int> IDs = entityManager->getEntities(prefixOrTag, isPrefix);

	for (int id : IDs) {
		setState(id, compName, state, share);
	}
}

bool Environment::getState(int entity, const string& compName) {
	if (!mapNC.contains(compName) || !mapNC[compName]->hasEntity(entity)) return false;
	return mapNC[compName]->getState(entity);
}

bool Environment::getState(const string& name, const string& compName) {
	int ID = entityManager->getEntity(name);
	return this->getState(ID, compName);
}

int Environment::getEntity(const string& name) {
	return entityManager->getEntity(name);
}


const string& Environment::getName(int entity) {
	return entityManager->getName(entity);
}

int Environment::createEntity(const string& name, bool createFile, bool share) {
	int ID = entityManager->createEntity(name, createFile);
	if (share) notify(ID);
	return ID;
}

void Environment::removeEntity(const string& name, bool share) {
	// Use EM remove and loop trough every CMs to unsubscribe the entity.
	int ID = entityManager->getEntity(name);
	entityManager->removeEntity(name);

	for (const auto& [key, value] : mapNC) {
		if (value->hasEntity(ID)) {
			value->unsubscribe(ID);
		}
	}

	if (share) notify(ID);
}

vector<shared_ptr<Component>> Environment::getComponents(int entity) {
	vector<shared_ptr<Component>> result;

	for (const auto& [_, value] : mapNC) {
		if (value->hasEntity(entity)) result.push_back(value->getComponent(entity));
	}

	return result;
}

vector<shared_ptr<Component>> Environment::getComponents(const string& name) {
	int ID = entityManager->getEntity(name);
	return this->getComponents(ID);
}

shared_ptr<Component> Environment::getComponent(int entity, const string& name) {
	if (mapNC.contains(name) && mapNC[name]->hasEntity(entity)) return mapNC[name]->getComponent(entity);
	throw runtime_error("Error : The component \"" + name + "\" is not attached to \"" + entityManager->getName(entity) + "\".");
}

shared_ptr<Component> Environment::getComponent(const string& entityName, const string& name) {
	int ID = entityManager->getEntity(entityName);
	return this->getComponent(ID, name);
}

bool Environment::hasComponent(const string& name, const string& compName) {
	int ID = entityManager->getEntity(name);
	return this->hasComponent(ID, compName);
}

bool Environment::hasComponent(int entity, const string& compName) {
	if (mapNC.contains(compName)) return mapNC[compName]->hasEntity(entity);
	return false;
}

bool Environment::hasTag(int entity, const string& tagName) {
	return entityManager->hasTag(entity, tagName);
}

bool Environment::hasTag(const string& name, const string& tagName) {
	int ID = entityManager->getEntity(name);
	return this->hasTag(ID, tagName);
}

void Environment::addTag(int entity, const std::string& tag, bool share) {
	entityManager->addTag(entity, tag);
	if (share) notify(entity);
}

void Environment::addTag(const std::string& name, const std::string& tag, bool share) {
	int ID = entityManager->getEntity(name);
	if (ID == -1) return;
	entityManager->addTag(ID, tag);
	if (share) notify(ID);
}

void Environment::save(int entity) {
	subscription->save(entity);
}

void Environment::save(const string& name) {
	int ID = entityManager->getEntity(name);
	this->save(ID);
}

void Environment::join(function<void(int)> callback, size_t ID) {
	notifiers[ID] = callback;
}

void Environment::notify(int entity) {
	for (const auto& [_, notifier] : notifiers) {
		notifier(entity);
	}
}

void Environment::notify(size_t ID) {
	if (!notifiers.contains(ID)) return;
	function<void(int)> callback = notifiers[ID];

	vector<int> entities = entityManager->getEntities("");
	// Share the current entities with the new system.
	for (const auto& entity : entities) {
		callback(entity);
	}
}

int Environment::copy(const string& original, const string& copy, bool createFile, bool share) {
	int newEntity = this->createEntity(copy, createFile, false);
	int ID = entityManager->getEntity(original);

	for (const auto& [_, value] : mapNC) {
		if (value->hasEntity(ID)) value->give(ID, newEntity, true);
	}
	if (share) notify(newEntity);
	return newEntity;
}

void Environment::give(const string& component, int giver, int receiver, bool copy, bool share) {
	if (!mapNC.contains(component) || !mapNC[component]->hasEntity(giver)) return;

	mapNC[component]->give(giver, receiver, copy);

	if (share) {
		notify(giver);
		notify(receiver);
	}
}

void Environment::makeSnapshot(const string& snapshotName, const vector<string>& entitiesToSave, const vector<string>& componentsToSave) {
	vector<string> entities = entitiesToSave.empty() ? move(entityManager->getNames()) : entitiesToSave;

	Snapshot snapshot;

	// Lambda function which check if a component should be saved for an entity or not.
	auto saveComponent = [this](const string& entity, const string& name, shared_ptr<ComponentManager> compManager, vector<pair<string, dataVector>>& entityInformation) {
		if (this->hasComponent(entity, name)) {
			shared_ptr<Component> component = this->getComponent(entity, name);

			dataVector data; // Vector of data
			for (const auto& [name, value] : component->getRawData()) {
				data.emplace_back(name, value.second);
			}

			entityInformation.emplace_back(name, move(data));
		}
	};

	for (const auto& entity : entities) {
		vector<pair<string, dataVector>> entityInformation;

		if (componentsToSave.empty()) {
			// Save all
			for (const auto& [name, compManager] : mapNC) {
				saveComponent(entity, name, compManager, entityInformation);
			}
		}
		else {
			// Save the given components
			for (const auto& name : componentsToSave) {
				if (mapNC.contains(name)) {
					saveComponent(entity, name, mapNC[name], entityInformation);
				}
			}
		}
		
		snapshot.entities.insert({ entity, entityInformation }); // Register the entity in the snapshot.
	}

	snapshots[snapshotName] = snapshot;
}

void Environment::loadSnapshot(const string& snapshotName) {
	if (!snapshots.contains(snapshotName)) return;
	Snapshot snapshot = snapshots[snapshotName];

	for (const auto& [key, value] : snapshot.entities) {
		int ID = entityManager->getEntity(key);
		if (ID != -1) { 
			// Valid entity only
			for (const auto& [name, data] : snapshot.entities[key]) {
				if (mapNC.contains(name)) {
					shared_ptr<ComponentManager> compManager = mapNC[name];
					if (compManager->hasEntity(ID)) {
						shared_ptr<Component> component = compManager->getComponent(ID);

						for (const auto& [key, value] : data) {
							component->set(key, value);
						}
					}
				}
			}
		}
	}
}

void Environment::clearSnapshot(const string& snapshotName) {
	snapshots.erase(snapshotName);
}
