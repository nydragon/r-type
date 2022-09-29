#pragma once 

#include <array>
#include <bitset>
#include <vector>
#include <memory>

constexpr int maxBitsetSize = 16;

class Entity;
class Component;

using ComponentID = std::size_t;
using Bitset = std::bitset<maxBitsetSize>;
using ComponentArray = std::array<Component *, maxBitsetSize>;


inline ComponentID getComponentID()
{
    static ComponentID lastID = 0;
    return lastID++;
}

template <typename T> inline ComponentID getComponentID() noexcept
{
    static ComponentID ID = getComponentID();
    return ID;
};

class Component
{
    public:
        Entity* entity;

        virtual void init() {}
        virtual void update() {}
        virtual void draw() {}

        virtual ~Component() {}
};

class Entity
{
private: 
    bool active = true;
    
    std::vector<Component> components;


    Bitset componentBitset;
    ComponentArray componentArray;

public:
    void draw () {
        for (auto &c : this->components) {
            c.draw();
        }
    }

    void update () {
        for (auto &c : this->components) {
            c.update();
        }
    }

    template<typename T>
    bool hasComponent() {
        return componentBitset[getComponentID<T>()];
    }

    // template <typename T>
    // T& addComponent()
    // {
    //     T* c(new T());
    //     c->entity = this;
    //     std::unique_ptr<Component> uPtr { c };
    //     components.emplace_back(std::move(uPtr));
    //     componentArray[getComponentID<T>()] = c;
    //     componentBitset[getComponentID<T>()] = true;
    //     c->init();
    //     return *c;
    // }

    template<typename T>
    T& getComponent() const {
        auto ptr(componentArray[getComponentID<T>()]);
        return *static_cast<T*>(ptr);
    }

    void destroy() {
        this->active = false;
    }

    bool isActive() const {
        return this->active;
    }       
};