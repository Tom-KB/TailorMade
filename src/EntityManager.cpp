#include "EntityManager.h"
#include "Environment.h"

using namespace std;

EntityManager::EntityManager() : directory(directory), count(-1), placeholder("") {
}

EntityManager::EntityManager(const string& directory) : directory(directory), count(-1), placeholder("") {

	vector<string> files = getAllFilesFromDirectory(directory); // Return every files in the directory's folder and its sub-folders

	for (const auto& file : files) {
		ifstream entityFile(file);

		if (!entityFile) {
			throw runtime_error("Error : Can't read the file \"" + file + "\"");
		}

		nlohmann::json entityJSON = nlohmann::json::parse(entityFile); // Parse the JSON of the entity's file
		vector<string> namesVector;
		if (entityJSON.contains("name")) {
			namesVector.push_back(entityJSON["name"]);
		}
		else if (entityJSON.contains("names")) {
			namesVector = entityJSON["names"];
		}
		else {
			continue;
		}

		vector<string> tags;
		if (entityJSON.contains("tags")) {
			tags = entityJSON["tags"];
		}

		for (const auto& name : namesVector) {
			// Check if multiple entities should be generated
			if (entityJSON.contains("generate")) {
				int ID = -1;
				for (int i = 0; i < entityJSON["generate"]; ++i) {
					ID = this->createEntity(name + to_string(i));
					if (ID != -1) {
						for (const auto& tag : tags) {
							this->addTag(ID, tag);
						}
					}
				}
			}
			else {
				int ID = createEntity(name);
				if (ID != -1) {
					for (const auto& tag : tags) {
						this->addTag(ID, tag);
					}
				}
			}
		}

		entityFile.close();
	}
}

int EntityManager::getEntity(const string& name) {
	if (!entities.contains(name)) {
		return -1; // Wrong entity's name, no error, just -1 [no entities could have this ID].
	}
	return entities[name];
}

vector<int> EntityManager::getEntities(const string& prefixOrTag, bool isPrefix) {
	vector<int> result;
	// For an empty prefix we return all the entities
	if (prefixOrTag == "") {
		if (isPrefix) {
			for (const auto& [_, id] : entities) {
				result.push_back(id);
			}
		}
		return result; // If it's a tag, result is empty.
	}

	// If prefix is true then we search for entities with name which begins with the given prefix.
	if (isPrefix) {
		auto start = entities.lower_bound(prefixOrTag);

		string upperBound = prefixOrTag;
		upperBound.back()++; // Upper bound of our prefix.

		auto end = entities.lower_bound(upperBound);

		for (; start != end; ++start) {
			result.push_back(start->second);
		}
	}
	// Otherwise we search for the entities of the given tag.
	else if (tags.contains(prefixOrTag)) {
		result = vector<int>(tags[prefixOrTag].begin(), tags[prefixOrTag].end());
	}

	return result;
}

vector<string> EntityManager::getNames() {
	vector<string> result;
	for (const auto& [name, _] : entities) {
		result.push_back(name);
	}
	return result;
}

const std::string& EntityManager::getName(int entity) {
	// If the entity does not exist we return an empty string.
	if (entity < names.size()) {
		return names[entity];
	}
	return placeholder;
}

int EntityManager::createEntity(const string& name, bool createFile) {
	if (!entities.contains(name)) {
		if (createFile) {
			nlohmann::json newEntityJSON;
			newEntityJSON["name"] = name;

			// Write it inside the root directory.
			ofstream newEntityFile(directory + "/" + name + ".json");
			newEntityFile << newEntityJSON.dump(4);
		}
		if (!availableIDs.empty()) {
			entities.insert({ name, availableIDs.front() });
			names[availableIDs.front()] = name;
			availableIDs.pop(); // Removed the newly used IDs.
		}
		else {
			names.push_back(name);
			entities.insert({ name, ++count });
		}
		return count;
	}
	return -1; // No entity created.
}

void EntityManager::removeEntity(const string& name) {
	// Remove the entity from the map, and tags.
	if (!entities.contains(name)) return;
	int ID = entities[name];

	availableIDs.push(entities[name]);
	// Tags removal.
	for (const auto& [key, value] : tags) {
		if (value.contains(ID)) tags[key].erase(ID);
	}

	// Map of entities removal.
	entities.erase(name);
}

void EntityManager::toString(ostream& stream) {
	stringstream ss;

	for (const auto& [key, value] : entities) {
		ss << "Name: " << key << ", ID: " << value << ", tags: [";
		bool first = true;
		for (const auto& [tagName, list] : tags) {
			if (list.contains(value)) {
				if (first) {
					ss << tagName << "";
					first = false;
				}
				else {
					ss << ", " << tagName;
				}
			}
		}

		ss << "]" << endl;
	}

	stream << ss.str();
}

bool EntityManager::hasTag(int entity, const string& tag) {
	if (tags.contains(tag)) {
		return tags[tag].contains(entity);
	}
	return false;
}

void EntityManager::addTag(int entity, const string& tag) {
	tags[tag].insert(entity);
}
