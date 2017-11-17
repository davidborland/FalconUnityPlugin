/*=========================================================================

  Name:        ForceContainer.h

  Author:      David Borland, The Renaissance Computing Institute (RENCI)

  Copyright:   The Renaissance Computing Institute (RENCI)

  Description: Class to contain and keep track of force effects and force
               effect ids.

=========================================================================*/


#ifndef FORCECONTAINER_H
#define FORCECONTAINER_H


#include <unordered_map>
#include <forward_list>


template <class T>
class ForceContainer {
public:
    ForceContainer() {}

    // Add a force effect. Return id for this effect
    int Add(T forceEffect) {
        int id;

        if (availableIds.empty()) {
            // Ids are all used, so add a new one
            id = forceEffects.size();
        }
        else {
            // At least one id has been used and returned, so take one from the list
            id = availableIds.front();
            availableIds.pop_front();
        }

        // Set the force effect
        forceEffects[id] = forceEffect;

        // Return the id
        return id;
    }

    // Return a pointer to the force effect with the given id
    T* Get(int id) {
        return &forceEffects[id];
    }

    // Iterator at the front of the force effects
    typename std::unordered_map<int, T>::iterator Begin() {
        return forceEffects.begin();
    }

    // Iterator at the end of the force effects
    typename std::unordered_map<int, T>::iterator End() {
        return forceEffects.end();
    }

    // Remove the force effect with the given id
    void Remove(int id) {
        int num = forceEffects.erase(id);

        if (num > 0) {
            // Valid id, so add it to the available id list
            availableIds.push_front(id);
        }
    }

    // Remove all force effects
    void RemoveAll() {
        forceEffects.clear();
        availableIds.clear();   
    }

protected:
    // Map with an integer key id for force effects
    std::unordered_map<int, T> forceEffects;

    // List to keep track of ids that have been used and returned
    std::forward_list<int> availableIds;
};


#endif