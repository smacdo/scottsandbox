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

#include <iostream>

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
        std::cout << "health default ctor, this="
            << (size_t)(this) << std::endl;
    }

    Health( size_t hp_ )
        : hp( hp_ )
    {
        std::cout << "health value ctor, this="
            << (size_t)(this) << std::endl;
    }

    Health( const Health& h )
        : hp( h.hp )
    {
        std::cout << "health copy ctor, this="
            << (size_t)(this) << ", other=" << (size_t)(&h) << std::endl;
    }

    const Health& operator = ( const Health& h )
    {
        std::cout << "health assign op" << std::endl;
        hp = h.hp;

        return *this;
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

//=========================================================================
// Entity class
//=========================================================================
Entity::Entity( EntityId id_ )
    : m_id( id_ )
{
    assert( defaultEntityManager != NULL );
}

//=========================================================================
// Entity Manager class
//=========================================================================
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
            std::cout << "Deleting " << (size_t)(inner->second) << std::endl;
            delete inner->second;
            instances++;
        }

        components++;
    }

    // Reset the entity map
    m_cstore = ComponentStore();
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
    //
    // Locate the map that contains all of the entities having a
    // component of type T. If this has not been created (itr = null),
    // then make sure to initialize it
    //
    ComponentStore::iterator csItr = m_cstore.find( T::CID );

    if ( csItr == m_cstore.end() )
    {
        m_cstore[ T::CID ] = EntityComponentMap();
        csItr             = m_cstore.find( T::CID );
    }

    //
    // Grab the map of entities containing component 'T'. From here,
    // associate the entitiy id to a newly created instance copied from
    // componentInstance
    //
    EntityComponentMap& eCompStore           = csItr->second;
    EntityComponentMap::iterator eCompMapItr = eCompStore.find( e.id() );

    if ( eCompMapItr != eCompStore.end() )
    {
        // There is an instance of the component stored for this
        // entity. Use the assignment operator to avoid having to delete
        // the object
        T* storedInstance = reinterpret_cast<T*>(eCompMapItr->second);
        *storedInstance = componentInstance;
    }
    else
    {
        eCompStore[ e.id() ] = new T( componentInstance );
    }
}

template<typename T>
T EntityManager::getComponent( const Entity& e )
{
    Component* c = findComponent( T::CID, e.id() );

    assert( c != NULL && "Entity's component must exist" );
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
    EntityComponentMap& store              = m_cstore[ cid ];
    EntityComponentMap::const_iterator itr = store.find( eid );

    if ( itr == store.end() )
    {
        return NULL;
    }
    else
    {
        return itr->second;
    }
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

class EntityManagerTest : public ::testing::Test
{
public:
    EntityManagerTest()
        : em( EntityManager::getDefault() )
    {
    }

protected:
    virtual void SetUp()
    {

    }

    virtual void TearDown()
    {
        em.clear();
    }

    EntityManager& em;
};

TEST_F(EntityManagerTest,DefaultEntityManagerIsSet)
{
    EntityManager manager("notdefault");
    EXPECT_EQ( "notdefault", manager.name() );

    EXPECT_FALSE( NULL == Entity::defaultEntityManager );
    EXPECT_EQ( "DefaultEntityManager",
               Entity::defaultEntityManager->name() );
}

TEST_F(EntityManagerTest,CreateNewEntity)
{
    Entity e = em.createEntity();

    // verify valid id
    EXPECT_NE( (EntityId) 0, e.id() );
}

TEST_F(EntityManagerTest,CreateNewEntityReturnsNewId)
{
    Entity a = em.createEntity();
    Entity b = em.createEntity();

    // verify are valid
    EXPECT_NE( (EntityId) 0, a.id() );
    EXPECT_NE( (EntityId) 0, b.id() );

    // make sure ids are not equal
    EXPECT_NE( a.id(), b.id() );
}

TEST_F(EntityManagerTest,EntityShouldNotHaveComponentThatWasNotAdded)
{
    Entity e = em.createEntity();

    EXPECT_FALSE( em.hasComponent<Health>( e ) );
}

TEST_F(EntityManagerTest,AddComponentToEntityAndFetchIt)
{
    Entity e = em.createEntity();
    em.addComponent( e, Health(42) );

    Health h = em.getComponent<Health>( e );
    EXPECT_EQ( (size_t) 42, h.hp );

    std::cout << "DONE" << std::endl;
}

TEST_F(EntityManagerTest,AddComponentToEntityTwiceFails)
{
    int i = 0;
    i += 2;
}

TEST_F(EntityManagerTest,AddComponentToEntityMeansComponentExists)
{
    Entity e = em.createEntity();

    em.addComponent( e, Health(42) );

    EXPECT_TRUE( em.hasComponent<Health>( e ) );
}
TEST_F(EntityManagerTest,DoesItWorkTest)
{
}
