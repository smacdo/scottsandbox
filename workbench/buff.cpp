/////////////////////////////////////////////////////////////////////////////
// HEADER
/////////////////////////////////////////////////////////////////////////////
#ifndef SCOTT_WORKBENCH_BUFF_H
#define SCOTT_WORKBENCH_BUFF_H
#define BUFF_VERSION 1

#include <map>
#include <set>
#include <string>

#include <iostream>
#include <cassert>

class Creature
{
private:
    std::vector<AbstractBuff*> m_buffs;
};

class CreatureManager
{
public:
    // 
    // for each creature in the world
    //  - apply buffs
    //
    void tick();


private:
    std::vector<Creature*> m_creatures;
};

class AbstractBuff
{
    // ref because creature must always exist during the lifespan
    // of a buff. Cannot be null - creature must delete the buff first
    AbstractBuff( Creature& creature );

    std::string name();
    size_t duration();
    size_t timeLeft();

protected:
    /**
     * Called when the buff is initially applied
     */
    void onStart();

    /**
     * Called every tick that the buff is active
     */
    void onTick();

    void onStop();
};

/////////////////////////////////////////////////////////////////////////////
// Implementation
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Unit Tests
/////////////////////////////////////////////////////////////////////////////
#include <googletest/googletest.h>

TEST(EntityManagerTest,DefaultEntityManagerIsSet)
{
    EXPECT_TRUE( true );
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
