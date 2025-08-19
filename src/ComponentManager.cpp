#include "ComponentManager.h"

using namespace std;

ComponentManager::ComponentManager(const string& filename) {
	referenceComp = make_shared<Component>(filename);
}

ComponentManager::ComponentManager(shared_ptr<Component> component) {
	referenceComp = make_shared<Component>();
	referenceComp->copy(component);
}

const string& ComponentManager::getName() {
	return referenceComp->getName();
}

const std::string& ComponentManager::getType(const string& data) {
	return referenceComp->getType(data);
}

void ComponentManager::subscribe(int entity) {
	scoped_lock lock(mtx);
	// Check if the entity is already subscribe, in which case we do nothing.
	if (!mapEC.contains(entity)) {
		shared_ptr<Component> component = make_shared<Component>();
		component->copy(referenceComp);
		mapEC.insert({ entity, {component, true} });
	}
}

void ComponentManager::subscribe(int entity, dataVector data) {
	this->subscribe(entity);
	scoped_lock lock(mtx);

	shared_ptr<Component> component = mapEC[entity].first;

	for (const auto& [name, data] : data) {
		component->set(name, data);
	}
}

void ComponentManager::unsubscribe(int entity) {
	scoped_lock lock(mtx);
	mapEC.erase(entity); // Remove the entity, do nothing if entity is not in mapEC.
}

vector<int> ComponentManager::getEntities(bool checkState) {
	vector<int> subscribedEntities;

	// We gathered all the keys of the mapEC
	if (mapEC.empty()) {
		for (const auto& [key, _] : mapEC) {
			if (mapEC[key].second || checkState) subscribedEntities.push_back(key); // Append the key according to checkState
		}
	}
	return subscribedEntities;
}

shared_ptr<Component> ComponentManager::getComponent(int entity) {
	if (!this->hasEntity(entity)) {
		throw runtime_error("Error : The entity " + to_string(entity) + " is not subscribed to the " + referenceComp->getName() + "'s ComponentManager.");
	}
	return mapEC[entity].first;
}

bool ComponentManager::hasEntity(int entity) {
	scoped_lock lock(mtx);
	return mapEC.contains(entity) && mapEC[entity].second; // Return false if the state is false.
}

bool ComponentManager::getState(int entity) {
	scoped_lock lock(mtx);
	// If the entity doesn't exist, return false.
	if (!mapEC.contains(entity)) {
		return false;
	}
	return mapEC[entity].second;
}

void ComponentManager::setState(int entity, bool newState) {
	scoped_lock lock(mtx);
	if (mapEC.contains(entity)) {
		mapEC[entity].second = newState;
	}
}

void ComponentManager::give(int giver, int receiver, bool copy) {
	scoped_lock lock(mtx);
	if (!mapEC.contains(giver)) return; // Giver do not exist, do nothing.
	mapEC[receiver] = mapEC[giver]; // Set both the component and state to the receiver.
	if (!copy) {
		mapEC.erase(giver); // Erase the giver if its not a copy.
	}
}

void ComponentManager::toString(ostream& stream) {
	stringstream ss;
	ss << this->getName() << ":" << endl;
	for (const auto& [key, value] : mapEC) {
		ss << "    ID: " << key << ", State: " << (value.second ? "Active" : "Inactive");
		ss << endl << "        ";
		value.first->toString(ss);
		ss << endl;
	}

	stream << ss.str();
}