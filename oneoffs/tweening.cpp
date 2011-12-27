#include <iostream>
#include <cassert>

#include <vector>
#include <algorithm>
#include <boost/bind.hpp>
#include <boost/utility.hpp>

template<typename T>
void Delete( T& pointer )
{
    boost::checked_delete( pointer );
    pointer = NULL;
}

template<typename ValT>
struct LinearTween
{
    ValT operator()( float time, float duration, ValT start, ValT target )
    {
        return start + ( target - start ) * ( time / duration );
    }
};

/**
 * Quadratic ease-in function
 */
template<typename ValT>
struct QuadInTween
{
    ValT operator()( float time, float duration, ValT start, ValT target )
    {
    return start + ( target - start ) * ( time /= duration ) * time;
    }
};


/**
 * Base interpolation class
 */
class TweenVar
{
public:
    TweenVar()
        : mCompleted( false )
    {
    }

    virtual ~TweenVar()
    {
        std::cout << "Deleted var" << std::endl;
    }

    virtual void update( float newTime ) = 0;

    bool hasCompleted() const
    {
        return mCompleted;
    }

protected:
    bool mCompleted;
};

template<typename TypeT>
class AbstractBasicTweenVar : public TweenVar
{
public:
    AbstractBasicTweenVar( TypeT& v,
                           float duration,
                           TypeT start,
                           TypeT target )
        : TweenVar(),
          mVar( v ),
          mDuration( duration ),
          mStart( start ),
          mTarget( target )
    {
    }

protected:
    TypeT& mVar;
    float mDuration;
    TypeT mStart;
    TypeT mTarget;
};

/**
 * Generic tweener
 */
template<typename TypeT, class Func>
class GenericTweenVar : public AbstractBasicTweenVar<TypeT>
{
public:
    GenericTweenVar( TypeT& v, float duration, TypeT start, TypeT target )
        : AbstractBasicTweenVar<TypeT>( v, duration, start, target ),
          mFunc()
    {
    }

    virtual void update( float newTime )
    {
        this->mVar = mFunc( newTime, 
                             this->mDuration,
                             this->mStart,
                             this->mTarget );
        this->mCompleted  = ( newTime >= this->mDuration );
    }

private:
    Func mFunc;
};

/**
 * Linear easing
 */
template<typename TypeT>
class LinearTweenVar : public GenericTweenVar<TypeT, LinearTween<TypeT> >
{
public:
    LinearTweenVar( TypeT& v, float duration, TypeT start, TypeT target )
        : GenericTweenVar<TypeT,LinearTween<TypeT> >( v, duration, start, target )
    {
    };
};

/**
 * Quadratic ease in
 *//*
template<typename TypeT>
class QuadInTweenVar : public GenericTweenVar<TypeT, QuadInTween<TypeT> >
{
public:
    QuadInTweenVar( TypeT& v, float duration, TypeT start, TypeT target )
        : GenericTweenVar<TypeT, QuadInTween<TypeT> >( v, duration, start, target )
    {
    };
};
*/
/*
class TweenGroup
{
public:
    TweenGroup();
    ~TweenGroup();

    void add( TweenVar* var );
    void update( float newTime );

private:
    std::vector<TweenVar*> mVars;
    bool mDeleteOnCompletion;
};

TweenGroup::TweenGroup()
    : mVars(),
      mDeleteOnCompletion( true )
{
}

TweenGroup::~TweenGroup()
{
    // Delete any tween vars that are still hanging around
    std::for_each( mVars.begin(),
                   mVars.end(),
                   boost::bind( &Delete<TweenVar*>, _1 ) );

    std::cout << "Deleted tween group" << std::endl;
}

void TweenGroup::add( TweenVar *pVar )
{
    mVars.push_back( pVar );
}

void TweenGroup::update( float time )
{
    // Update all the tween vars
    std::for_each( mVars.begin(),
                   mVars.end(),
                   boost::bind( &TweenVar::update, _1, time ) );

    // Remove all finished vars
    if ( mDeleteOnCompletion && mVars.size() > 0 )
    {
        // Save the current end of sequence
        auto oldEndItr = mVars.end();

        // Move all of the tween vars to the end of the vector sequence
        auto newEndItr =
            std::remove_if( mVars.begin(),
                            mVars.end(),
                            boost::bind( &TweenVar::hasCompleted, _1 ) );

        // Now delete the tween vars
        std::for_each( newEndItr,
                       oldEndItr,
                       boost::bind( &Delete<TweenVar*>, _1 ) );
    }
}
*/


int main( int argc, char* argv[] )
{
    int var = 42;
    LinearTweenVar<int> tvar( var, 1.0f, 40, 50 );

    tvar.update( 0.0f );
    tvar.update( 0.5f );
/*
    const float start = 1;
    const float end   = 5;

    const size_t SLICE_COUNT = 11;
    const float TIME_SLICES[SLICE_COUNT] =
        { 0.0f, 0.1f, 0.2f, 0.3f, 0.4f, 0.5f,
          0.6f, 0.7f, 0.8f, 0.9f, 1.0f };

    // Create tweened variables
    float myValue = 0;
    QuadInTweenVar<float> *pVar =
        new QuadInTweenVar<float>( myValue, 1.0f, start, end );

    int iVal = 42;

    // Create a tween var holder
    TweenGroup vars;

    vars.add( pVar );
    vars.add( new GenericTweenVar<int,LinearTween<int>>( iVal, 1.0f, 40, 42 ) );

    for ( size_t i = 0; i < SLICE_COUNT; ++i )
    {
        float t = TIME_SLICES[i];
        vars.update( t );

        std::cout << t << ": float=" << myValue << 
            ", int = " << iVal <<std::endl;
    }*/
}
