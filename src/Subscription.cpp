#include "Subscription.h"

using namespace std;

Subscription::Subscription(const string& directory, shared_ptr<EntityManager> entityManager, shared_ptr<unorMapCM> compManagers) : directory(directory), entityManager(entityManager), managers(compManagers) {

	vector<string> files = getAllFilesFromDirectory(directory); // Return every files in the directory's folder and its sub-folders

	for (const auto& file : files) {
		ifstream subsFile(file);

		nlohmann::json subsJSON = nlohmann::json::parse(subsFile);

		if (!subsJSON.contains("generated") && subsJSON.contains("entity")) {
			entitiesFP[subsJSON["entity"]] = file;
			// We skipped subscriptions for unknown entities
			if (entityManager->getEntity(subsJSON["entity"]) == -1) continue;
		}

		vector<int> IDs;

		// Firstly, we check if its a tag attributed subscription.
		if (subsJSON.contains("tags")) {
			// Get every entities from that tag
			vector<string> tags = subsJSON["tags"];
			// We get every entities of the given tags
			for (const auto& tag : tags) {
				vector<int> localIDs = entityManager->getEntities(tag, false);
				IDs.insert(IDs.end(), localIDs.begin(), localIDs.end());
			}
		}
		else if (subsJSON.contains("generated") && subsJSON["generated"]) {
			// Search by prefix, for the generated entities
			IDs = entityManager->getEntities(subsJSON["entity"]);
		}
		else {
			// Otherwise, by complete name
			IDs.push_back(entityManager->getEntity(subsJSON["entity"]));
		}

		// We check if a default state is given and apply it to the components
		bool defaultState = true;

		if (subsJSON.contains("state")) defaultState = subsJSON["state"];

		nlohmann::json components = subsJSON["components"];
		for (const auto& component : components) {
			string name = component["name"];

			if (!managers->contains(name)) continue; // Skip the unknown components.

			shared_ptr<ComponentManager> compManager = managers->at(name);

			dataVector data;

			for (const auto& [key, value] : component["data"].items()) {
				data.push_back({ key, valueToType(value, compManager->getType(key)) });
			}

			for (const auto& entity : IDs) {
				compManager->subscribe(entity, data);
				if (!defaultState) {
					compManager->setState(entity, false);
				}
			}
		}
	}
}

void Subscription::save(int entity) {
	string name = entityManager->getName(entity);
	string path;
	if (entitiesFP.contains(name)) {
		path = entitiesFP[name];
	}
	else {
		path = directory + "/" + name + ".json";
	}

	ofstream subsFile(path);

	nlohmann::ordered_json subsJSON;
	subsJSON["entity"] = name;

	vector<shared_ptr<Component>> components;

	// Get the list of the entity's components
	for (const auto& [_, manager] : *managers) {
		if (manager->hasEntity(entity)) {
			components.push_back(manager->getComponent(entity));
		}
	}

	vector<nlohmann::ordered_json> componentsJSON;

	for (const auto& component : components) {
		nlohmann::ordered_json dict;
		dict["name"] = component->getName();
		dict["data"] = serializeType(component->getRawData());
		componentsJSON.push_back(dict);
	}

	subsJSON["components"] = componentsJSON;

	subsFile << subsJSON.dump(4);
}