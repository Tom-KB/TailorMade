/**
 * @file Component.h
 * Project TailorMade
 * @author Thomas K/BIDI
 * @version 2.0
 */


#ifndef _COMPONENT_H
#define _COMPONENT_H

#include <TM_Tools.h>
 /**
 * @file Component.h
 * @brief Component implementation
 *
 * @details This Component class is a general implementation of every possible components of TailorMade.
 * @details It's the main class of this project since its the one used to edit and retrieve the entity's data.
 * @details With a useful format for setting (set({data's name}, {data's value})) and getting (get<Type>({data's name})) the data.
 */


class Component {
public: 
    Component() = default;

    /**
     * @brief Constructor of the component from a file.
     * @param filename Full path towards the component's file.
     */
    Component(const std::string& filename);
    
    /**
     * @brief Constructor of the component from its name and data.
     * @param name Name of the component.
     * @param data Data of the component.
     * @see dataUnMap
     */
    Component(const std::string& name, dataUnMap dataDump);

    /**
     * @brief Copy the information of the given component in the current one.
     * @details Easy way to clone a component's data towards a new one.
     * @param component A shared_ptr towards the component you want as the model.
     */
    void copy(std::shared_ptr<Component> component);
    
    /**
     * @brief Return the component's name.
     */
    const std::string& getName();
    
    /**
     * @brief Return a string with the type of the given data (or "" if the data doesn't exist).
     * @param name Data's name.
     */
    const std::string& getType(const std::string& name);
    
    /**
     * @brief Return a vector with the names of every data from this component.
     */
    std::vector<std::string> getNames();
    
    /**
     * @brief Return a const reference of the component's data structure.
     * @details Can be used for serialization of the data or to make a copy of the component.
     */
    const dataUnMap& getRawData();
    
    /**
     * @brief Append to the given stream a serialized version of the data.
     * @details Useful for debugging or logging.
     * @param stream The stream on which the informations should be append.
     */
    void toString(std::ostream& stream);
    
    /**
     * @brief Return the value of the data from its name and type.
     * @details That's a generic function.
     * @warning It's on you to give the right type when you call this method.
     * @param name Data's name.
     */
    template<typename Type>
    Type get(const std::string& name);
    
    /**
     * @brief Set the value of the desired data with the given value.
     * @details That's a generic function.
     * @param name Data's name.
     * @param value Data's value.
     */
    template<typename Type>
    void set(const std::string& name, Type value);

    /**
     * @brief Add a new data to the component.
     * @details Convert automatically the serialized type to the real data.
     * @details The data is set with the default value of the given type.
     * @param name Data's name.
     * @param type Serialized version of the data's type.
     */
    void add(const std::string& name, const std::string& type);

protected:
    /**
     * The name of the component.
     */
    std::string componentName;

    /**
     * Link the data's name to the pair <type, value> in an unordered_map.
     */
    dataUnMap dataMap;

    /**
     * Lock used by the components, let them the possibility to be used in thread.
     */
    std::mutex mtx;

private:
    /**
     * Placeholder string initialized at "", used when a parameter is not valid for certain methods.
     */
    std::string placeholder;
};

template<typename Type>
inline Type Component::get(const std::string& name) {
    std::scoped_lock lock(mtx);
    try {
        if (!dataMap.contains(name)) {
            // No data with this name
            throw std::runtime_error("Error : no data with the name \"" + name + "\".");
        }

        std::variant<ECS_Types> data = dataMap[name].second;
        return std::get<Type>(data); // Be careful of bad conversion here
    }
    catch (std::exception& e) {
        std::cerr << "Component : " << e.what() << std::endl;
        return Type{};
    }
}

template<typename Type>
inline void Component::set(const std::string& name, Type value) {
    std::scoped_lock lock(mtx);
    try {
        if (!dataMap.contains(name)) {
            //No data with this name
            throw std::runtime_error("Error : no data with the name \"" + name + "\".");
        }
        dataMap[name].second = value;
    }
    catch (std::exception& e) {
        std::cerr << "Component : " << e.what() << std::endl; // Be careful, some values will throw an error if you try to implicit cast towards them.
        // For example, Vector3 ---> integer won't work.
    }
}

#endif //_COMPONENT_H