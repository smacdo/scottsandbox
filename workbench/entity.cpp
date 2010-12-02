/////////////////////////////////////////////////////////////////////////////
// HEADER
/////////////////////////////////////////////////////////////////////////////
/**
 * Entity manager. Allows us to define entities and associate arbitrary
 * components to these entities.
 */
#ifndef SCOTT_WORKBENCH_ENTITY_H
#define SCOTT_WORKBENCH_ENTITY_H
#define ENTITY_VERSION 1

#include <map>
#include <set>
#include <string>

#include <cassert>

class EntityManager;
class Entity;
struct Component;
typedef size_t ComponentId;
typedef size_t EntityId;

typedef std::map<EntityId, Component*> EntityComponentMap;
typedef std::map<ComponentId, std::map<EntityId, Component*> > ComponentStore;

class EntityManager
{
public:
    EntityManager( const std::string& name );
    ~EntityManager();

    static EntityManager& getDefault()
    {
        static EntityManager mine("DefaultEntityManager");
        return mine;
    }

    /**
     * Create a new and empty entity
     */
    Entity createEntity();

    EntityId getNextUnusedId();

    void addEntity( const Entity& entity );

    Entity getEntity( const EntityId& id );

    template<typename T>
    void addComponent( const Entity& e, const T& componentInstance );

    template<typename T>
    void setComponent( const Entity& e, const T& componentInstance );

    template<typename T>
    T getComponent( const Entity& e );

    template<typename T>
    void deleteComponent( const Entity& e );

    template<typename T>
    bool hasComponent( const Entity& e ) const;

    // debug
    std::string name() const { return m_name; }

    void clear();

private:
    Component* findComponent( ComponentId cid, EntityId eid );
    void storeComponent( ComponentId cid, EntityId eid, Component* c );
    bool hasComponent( ComponentId cid, EntityId eid ) const;

    EntityId m_nextId;
    ComponentStore m_cstore;

    // Name of the entity manager, used for debugging
    std::string m_name;
};

class Entity
{
public:
    Entity( EntityId id );

    /**
     * Adds an instanced component to the entity.
     */
    template<typename T>
    void addComponent( const T& componentInstance )
    {
        T * storedInstance = new T( componentInstance );
        defaultEntityManager->addComponent( id, storedInstance );
    }

    template<typename T>
    bool hasComponent() const
    {
        return defaultEntityManager->hasComponent<T>( id );
    }

    /**
     * Retrieves a component attached to this entity. This will trigger
     * an assertion if the entity does not have the requested component
     */
    template<typename T>
    T getComponent()
    {
        return defaultEntityManager->getComponent<T>( id );
    }

    /**
     * Deletes a component attached to this enttiy
     */
    template<typename T>
    void deleteComponent()
    {
        defaultEntityManager->deleteComponent( id, T::CID );
    }

    EntityId id() const { return m_id; }

    bool operator == ( const Entity& e ) const
    {
        return m_id == e.m_id;
    }

public:
    static EntityManager * defaultEntityManager;

private:
    EntityId m_id;
};

struct Component
{
};

struct Position : public Component
{
    Position()
        : x(0.0f), y(0.0f)
    {
    }

    Position( float x_, float y_ )
        : x(x_), y(y_)
    {
    }

    static const ComponentId CID;
    float x, y;
};

struct Health : public Component
{
    Health()
        : hp(0)
    {
    }

    Health( size_t hp_ )
        : hp( hp_ )
    {
    }

    static const ComponentId CID;
    size_t hp;
};

#endif

/////////////////////////////////////////////////////////////////////////////
// Implementation
/////////////////////////////////////////////////////////////////////////////
#include <iostream>

EntityManager* Entity::defaultEntityManager = &(EntityManager::getDefault());

Entity::Entity( EntityId id_ )
    : m_id( id_ )
{
    assert( defaultEntityManager != NULL );
}

EntityManager::EntityManager( const std::string& name )
    : m_nextId( 1 ),
      m_name( name )
{
    std::cout << "Constructed entity manager " << name << std::endl;
}

EntityManager::~EntityManager()
{
    if ( Entity::defaultEntityManager == this )
    {
        Entity::defaultEntityManager = NULL;
    }

    clear();
}

void EntityManager::clear()
{
    std::cout << "CLEARING ENTITY MANAGER" << std::endl;
    size_t components = 0;
    size_t instances  = 0;

    //
    // Clean up allocated entities and instances
    //  (refactor into utility function?
    //
    ComponentStore::iterator itr = m_cstore.begin();

    for ( ; itr != m_cstore.end(); ++itr )
    {
        // Traverse each component's list of entites and delete the
        // entity's associated component instance
        EntityComponentMap::iterator inner = itr->second.begin();

        for ( ; inner != itr->second.end(); ++inner )
        {
            delete inner->second;
            instances++;
        }

        components++;
    }

    std::cout << "Cleared " << components << " components and "
              << instances << " instances" << std::endl;
}

Entity EntityManager::createEntity()
{
    return Entity( m_nextId++ );
}

EntityId EntityManager::getNextUnusedId()
{
    return m_nextId++;
}

void EntityManager::addEntity( const Entity& entity )
{
    // registers entities
    entity == entity;
}

template<typename T>
void EntityManager::addComponent( const Entity& e, const T& componentInstance )
{
    if ( hasComponent( T::CID, e.id() ) )
    {
        assert( false && "Entity already has component" );
    }

    setComponent( e, componentInstance );
}

template<typename T>
void EntityManager::setComponent( const Entity& e, const T& componentInstance )
{
    EntityComponentMap& store        = m_cstore[ T::CID ];
    EntityComponentMap::iterator itr = store.find( e.id() );

    if ( itr != store.end() )
    {
        // There is an instance of the component stored for this
        // entity. Use the assignment operator to avoid having to delete
        // the object
        T* storedInstance = reinterpret_cast<T*>(itr->second);
        *storedInstance = componentInstance;
    }
    else
    {
        *(store[ e.id() ]) = componentInstance;
    }
}

template<typename T>
T EntityManager::getComponent( const Entity& e )
{
    Component* c = findComponent( T::CID, e.id() );

    assert( c != NULL );
    return T( *(reinterpret_cast<T*>( c )) );
}

template<typename T>
void EntityManager::deleteComponent( const Entity& e )
{

}

template<typename T>
bool EntityManager::hasComponent( const Entity& e ) const
{
    return hasComponent( T::CID, e.id() );
}

Component * EntityManager::findComponent( ComponentId cid, EntityId eid )
{
    EntityComponentMap& store = m_cstore[ cid ];
    return store[eid];
}

bool EntityManager::hasComponent( ComponentId cid, EntityId eid ) const
{
    ComponentStore::const_iterator m = m_cstore.find( cid );
    bool didFind = true;

    if ( m == m_cstore.end() )
    {
        didFind = false;
    }
    else
    {
        didFind = ( m->second.find(eid) !=  m->second.end() );
    }

    return didFind;
}

const ComponentId Position::CID = 1;
const ComponentId Health::CID   = 2;


/////////////////////////////////////////////////////////////////////////////
// Unit Tests
/////////////////////////////////////////////////////////////////////////////
#include <googletest/googletest.h>

TEST(EntityManager,DefaultEntityManagerIsSet)
{
    EntityManager manager("notdefault");
    EXPECT_EQ( "notdefault", manager.name() );

    EXPECT_FALSE( NULL == Entity::defaultEntityManager );
    EXPECT_EQ( "DefaultEntityManager",
               Entity::defaultEntityManager->name() );
}

TEST(EntityManager,CreateNewEntity)
{
    EntityManager& em = EntityManager::getDefault();
    Entity e = em.createEntity();

    // verify valid id
    EXPECT_NE( (EntityId) 0, e.id() );
}

TEST(EntityManager,CreateNewEntityReturnsNewId)
{
    EntityManager& em = EntityManager::getDefault();
    Entity a = em.createEntity();
    Entity b = em.createEntity();

    // verify are valid
    EXPECT_NE( (EntityId) 0, a.id() );
    EXPECT_NE( (EntityId) 0, b.id() );

    // make sure ids are not equal
    EXPECT_NE( a.id(), b.id() );
}

TEST(EntityManager,EntityShouldNotHaveComponentThatWasNotAdded)
{
    EntityManager& em = EntityManager::getDefault();
    Entity e = em.createEntity();

    EXPECT_FALSE( em.hasComponent<Health>( e ) );
}

TEST(EntityManager,AddComponentToEntity)
{

}

TEST(EntityManager,AddComponentToEntityTwiceFails)
{

}

TEST(EntityManager,AddComponentToEntityMeansComponentExists)
{
    EntityManager& em = EntityManager::getDefault();
    Entity e = em.createEntity();

    em.addComponent( e, Health(42) );

    EXPECT_TRUE( em.hasComponent<Health>( e ) );
}
TEST(EntityManager,DoesItWorkTest)
{
}
