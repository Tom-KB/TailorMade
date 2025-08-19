#include "System.h"

using namespace std;

System::System(shared_ptr<Environment> environment, bool autoUpdate) : ID(nextID++), change(false), environment(environment) {
	if (autoUpdate) environment->join([this](int entity) { this->newEntity(entity); }, ID);
}

void System::newEntity(int entity) {
	// Check the desired components and/or tags
	scoped_lock lock(mtx);

	// Verify if the entity is already present, in which case we have to remove it first and then check back if we need it.
	// Handle the changement of state of a component.
	if (entities.contains(entity)) entities.erase(entity);

	// Firstly, we checked the tags (faster).
	if (!desiredTags.empty()) {
		for (const auto& tag : desiredTags) {
			// If one of the desired tag is present, we add the entity to the list.
			if (environment->hasTag(entity, tag)) {
				entities.insert(entity);
				change = true;
				return;
			}
		}
	}

	// No tag present, we check the components.
	if (!rejectedComponents.empty()) {
		for (const auto& component : rejectedComponents) {
			// If at list one of this components is possess by the entity, we reject it.
			if (environment->hasComponent(entity, component)) {
				return;
			}
		}
	}

	if (!desiredComponents.empty()) {
		for (const auto& component : desiredComponents) {
			// If at list one of this components is missing, we reject the entity.
			if (!environment->hasComponent(entity, component)) {
				return;
			}
		}
		entities.insert(entity);
		change = true;
	}
}

void System::changeEnvironment(shared_ptr<Environment> environment) {
	this->environment = environment;
}

const size_t& System::getID() {
	return ID;
}

bool System::getChange() {
	scoped_lock lock(mtx); // Avoid multiple access to the change value.
	if (change) {
		change = false;
		return true;
	}
	return false;
}

void System::addComponent(const string& name) {
	// No verification of the actual presence of a component, should be handled by the developer.
	desiredComponents.push_back(name);
	environment->notify(ID);
}

void System::addComponents(vector<string> names) {
	for (const auto& name : names) {
		desiredComponents.push_back(name);
	}
	environment->notify(ID);
}

void System::addRejected(const string& name) {
	// Same thing for the rejected components.
	rejectedComponents.push_back(name);
	environment->notify(ID);
}

void System::addRejects(vector<string> names) {
	for (const auto& name : names) {
		rejectedComponents.push_back(name);
	}
	environment->notify(ID);
}

void System::addTag(const string& tagName) {
	// Same thing for the tags.
	desiredTags.push_back(tagName);
	environment->notify(ID);
}

void System::addTags(vector<string> names) {
	for (const auto& name : names) {
		desiredTags.push_back(name);
	}
	environment->notify(ID);
}
