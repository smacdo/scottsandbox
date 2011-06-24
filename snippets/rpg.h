
typedef std::map< std::string, VALUE > Attributes;

enum ScriptEvent
{
    OnCreate,
    OnLoad,
    OnCommand,
    OnDestroy,
    OnInRange,
    OnOutOfRange,
    OnCollide,
    OnSteal,
    OnDispel,
    OnAttack,
    OnDrop,
    OnPickup,
    OnUse,
    OnEquip,
    OnUnequip,
    OnLogin,
    OnLogout,
    OnSell,
    OnBuy,
    OnTick,
    OnCustomTrigger,
    OnTimer,
    OnDeath,
    OnResurrect,
    OnEnterRegion,
    OnLeaveRegion,
    OnSpellTarget,
    OnSpellCast,
    OnTalk,
    OnStart,
    OnStop,
    OnCombatStart,
    OnCombatStop,
    OnDeathBlow,
    OnCombatDamageCalc
};

void ScriptRef
{
public:
    ScriptRef( ScriptEvent event, Shared<ScriptData> script );

protected:
    ScriptEvent        m_event;
    ScriptContext      m_context;
    Shared<ScriptData> m_script;
};

struct Vitals
{
    Vitals( size_t hp, size_t mp, size_t stam )
        : health( hp ),    maxHealth( hp ),
          mana( mp ),      maxMana( mp ),
          stamina( stam ), maxStamina( stam )
    {
    }

    size_t health,  maxHealth;
    size_t mana,    maxMana;
    size_t stamina, maxStamina;
};

class Actor
{

protected:
    String m_name;
    Race   m_race;
    Vitals m_vitals;

};

class Character
{

protected:
    String m_name;
    String m_title;

    uint fame;
    uint karma;
    uint kills;
};

class ItemRef
{
    UID refid();
    
    ActorRef owner();

    bool IsA( ItemClass c );
    bool isStackable();
    int  maxStackSize();

    bool isEquippableBy( Character c );

    ItemData * blueprint();

    Money price();

    // augments are light copied
    Augment getAugment( size_t slot );
    void putAugment( size_t slot, const Augment& augment );

protected:
    // Ref specific data
    UID        m_refid;
    ActorRef   m_creator;
    Attributes m_attr;
    uint       m_stackSize;

    // Data copied from blueprint
    UID  m_itemid;
    Vec3 m_position;

};
