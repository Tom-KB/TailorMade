#include "Component.h"

using namespace std;

Component::Component(const string& filename) : placeholder("") {
	ifstream fileJSON(filename);
	if (!fileJSON) {
		cerr << "Component : cannot open the component file \"" << filename << "\"." << endl;
		return;
	}

	nlohmann::json file;
	try {
		file = nlohmann::json::parse(fileJSON);
	}
	catch (const exception& e) {
		cerr << "Component : cannot parse the component file \"" << filename << "\" : " << e.what() << endl;
		return;
	}

	if (!file.contains("name")) {
		cerr << "Component : the component file \"" << filename << "\" has no \"name\" field, it is ignored." << endl;
		return;
	}
	componentName = file["name"];

	// Load the components information from the given component's file
	if (!file.contains("data")) return; // A component with a name but no data is valid, just empty.
	for (const auto& data : file["data"].items()) {
		// Name + Type (both as strings)
		string name = data.key();
		string type = data.value().get<string>();

		try {
			dataMap.insert({ name, {type, strToType(type)} }); // {data's name, { type's name, type's default value }}
		}
		catch (exception& e) {
			cerr << "Component \"" << componentName << "\" (" << filename << ") : data \"" << name
				 << "\" has an unknown type \"" << type << "\", it is ignored." << endl;
		}
	}

	fileJSON.close();
}

Component::Component(const string& name, dataUnMap dataDump) : placeholder("") {
	componentName = name;

	for (const auto& data : dataDump) {
		dataMap.insert(data);
	}
}

void Component::copy(shared_ptr<Component> component) {
	componentName = component->componentName; 
	//Data copy 
	for (const auto& [key, value] : component->dataMap) { 
		dataMap.insert({ key, value });
	}
}

const string& Component::getName() {
	return componentName;
}

const string& Component::getType(const string& name) {
	if (!dataMap.contains(name)) return placeholder; // ""
	return dataMap[name].first;
}

vector<string> Component::getNames() {
	vector<string> result;

	for (const auto& [key, _] : dataMap) {
		result.push_back(key);
	}
	return result;
}

const dataUnMap& Component::getRawData() {
	return dataMap;
}

void Component::toString(ostream& stream) {
	stringstream ss;
	ss << this->getName() << ":" << endl;

	for (const auto& [key, value] : dataMap) {
		ss << "Name: " << key << ", Type: " << value.first << ", Value: ";
		valueToStream(ss, value.second);
		ss << endl;
	}

	stream << ss.str();
}

void Component::add(const string& name, const string& type) {
	dataMap.insert({ name, {type, strToType(type)} });
}