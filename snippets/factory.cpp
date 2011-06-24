template<typename T>
class Factory
{
public:
    typedef typename T base_type;

    class CreatorBase
    {
    public:
        base_type* operator()() { return create(); }
        virtual base_type* create() const = 0;
    };

    template<typename D>
    class Creator : public CreatorBase
    {
    public:
        typedef typename D derived_type;
        virtual base_type* create() const { return new derived_type(); }
    };

    
    typedef std::map<std::string, CreatorBase*> ObjectMap;

    void register( const std::string& name, CreatorBase* obj )
    {
        ObjectMap::iterator itr = m_objects.find( name );
        assert( itr == m_objects.end() );

        m_objects[name] = obj;
    }

    BaseType* create( const std::string& name )
    {
        ObjectMap::iterator itr = m_objects.find( name );
        assert( itr != m_objects.end() );

        return (*(*itr).second)();
    }

private:
    ObjectMap m_objects;
};

class Object {};
class GameObject : public Object {};

int main()
{
    Factory<Object> f;

    f.register_object( "GameObject",
                       new Factory<Object>::Creator<GameObject>() );

    GameObject *go = f.create( "GameObject" );

}
