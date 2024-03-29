#ifndef CTFASTDELEGAT_H
#define CTFASTDELEGAT_H
#include "QDebug"
#include <QList>
//  Контейнер для хранения до 2-х аргументов.
//class QList;

struct NIL {};

class IArguments
{
public:
    virtual ~IArguments() {}
};

template< class T1 = NIL, class T2 = NIL > class Arguments : public IArguments
{
public:
      T1 arg1;
      T2 arg2;

      Arguments()
      {}
      Arguments( T1 i_arg1 ) : arg1( i_arg1 )
      {}
      Arguments( T1 i_arg1, T2 i_arg2 ) : arg1( i_arg1 ), arg2( i_arg2 )
      {}
};

//  Контейнер для хранения указателя на метод.
class IContainer
{
public:
    virtual void Call( IArguments* ) = 0;
    virtual ~IContainer(){}
};

template< class T, class M > class Container : public IContainer
{

private:

    typedef void (T::*U)(void);
    T* m_class; U m_method;

public:

    Container()
    {}
    Container( T* c, M m ) : m_class( c ), m_method( m )
    {}

    virtual ~Container(){}

    virtual void Call( IArguments* i_args )
    {
      (m_class->*m_method)();
    }
};

  //Специализация для метода без аргументов.
//template< class T, class M >
//  class Container< T, void (T::*)(void) > : public IContainer
//{
//  typedef void (T::*U)(void);
//  public: Container( T* c, M m ) : m_class( c ), m_method( m ) {}
//  private: T* m_class; U m_method;
//  public: virtual void Call( IArguments* i_args )
//  {
//    (m_class->*m_method)();
//  }
//};

//  Специализация для метода с одним аргументом.
template< class T, class A1 > class Container< T, void (T::*)(A1) > : public IContainer
{
private:
    typedef void (T::*M)(A1);
    typedef Arguments<A1> A;

    T* m_class; M m_method;
public:
    Container( T* c, M m ) : m_class( c ), m_method( m )
    {}
    virtual ~Container(){}

    virtual void Call( IArguments* i_args )
    {
        A* a = dynamic_cast< A* >( i_args );
        //assert( a );
        if( a ) (m_class->*m_method)( a->arg1 );
    }
};

//  Специализация для метода с двумя аргументами
template< class T, class A1, class A2 > class Container< T, void (T::*)(A1,A2) > : public IContainer
{
private:
    typedef void (T::*M)(A1,A2);
    typedef Arguments<A1,A2> A;
    T* m_class; M m_method;
  public:
    Container( T* c, M m ) : m_class( c ), m_method( m )
    {}
    virtual ~Container(){}

    virtual void Call( IArguments* i_args )
    {
        A* a = dynamic_cast< A* >( i_args );
        //assert( a );
        if( a ) (m_class->*m_method)( a->arg1, a->arg2 );
    }
};

//  Собственно делегат.
class ctFastDelegat
{
    //TODO: NEED CHECK FOR MEMMORY LEAKS!!!
private:
    //IContainer* m_container;
    QList<IContainer*>* m_containers;
public:

    ctFastDelegat() : m_containers( 0 )
    {
        m_containers = 0;
    }

    ~ctFastDelegat()
    {
        qDebug()<<"TryDestroyDelegat";
        //if( m_container )
        //    delete m_container;

        if(m_containers && m_containers->count() > 0)
        {
            QList<IContainer*>::iterator iter;

            for(iter = m_containers->begin(); iter != m_containers->end(); ++iter)
            {
                delete (*iter);
            }

            m_containers->erase(m_containers->begin(), m_containers->end());
            delete m_containers;
        }
    }

//    template< class T, class U > void Connect( T* i_class, U i_method )
//    {
//        if( m_container ) delete m_container;
//        m_container = new Container< T, U >( i_class, i_method );
//    }

    template< class T, class U > void AppendConnect(T* i_class, U i_method)
    {
        if(!m_containers) {m_containers = new QList<IContainer*>();}

        m_containers->append(new Container<T, U>(i_class, i_method));
    }

    void Call()
    {
        if(m_containers)
        {
            Arguments<>* t_args = new Arguments<>();
            QList<IContainer*>::iterator m_iterator;
            for(m_iterator = m_containers->begin(); m_iterator != m_containers->end(); ++m_iterator)
            {
                (*m_iterator)->Call(t_args);
            }
            delete t_args;
        }
    }

//    void operator()()
//    {
//        Arguments<>* t_args = new Arguments<>();
//        m_container->Call(t_args);
//        delete t_args;
//    }

   template< class T1 > void Call( T1 i_arg1 )
    {
        if(m_containers)
        {
            Arguments<T1>* t_args = new Arguments<T1>(i_arg1);
            QList<IContainer*>::iterator m_iterator;
            for(m_iterator = m_containers->begin(); m_iterator != m_containers->end(); ++m_iterator)
            {
                (*m_iterator)->Call(t_args);
            }
            delete t_args;
        }
    }

//    template< class T1 > void operator()( T1 i_arg1 )
//    {
//        Arguments<T1>* t_args = new Arguments< T1 >( i_arg1 );
//        m_container->Call(t_args);
//        delete t_args;
//    }

    template< class T1, class T2 >  void Call( T1 i_arg1, T2 i_arg2 )
     {
         if(m_containers)
         {
             Arguments< T1, T2 >* t_args = new Arguments< T1, T2 >( i_arg1, i_arg2 );
             QList<IContainer*>::iterator m_iterator;
             for(m_iterator = m_containers->begin(); m_iterator != m_containers->end(); ++m_iterator)
             {
                 (*m_iterator)->Call(t_args);
             }
             delete t_args;
         }
     }

//    template< class T1, class T2 > void operator()( T1 i_arg1, T2 i_arg2 )
//    {
//        Arguments< T1, T2 >* t_args = new Arguments< T1, T2 >( i_arg1, i_arg2 );
//        m_container->Call(t_args);
//        delete t_args;
//    }

    bool IsHasActions()
    {
        if(m_containers && m_containers->count() > 0) return true;

        return false;
    }
};

#endif // CTFASTDELEGAT_H


//class ctFastDelegat
//{
////public:
////    ctFastDelegat();

////    template <class T, void (T::*TMethod)(int)>
////    static ctFastDelegat from_method(T* object_ptr)
////    {
////        ctFastDelegat d;
////        d.object_ptr = object_ptr;
////        d.stub_ptr = &method_stub<T, TMethod>; // #1
////        return d;
////    }

////    void operator()(int a1) const
////    {
////        return (*stub_ptr)(object_ptr, a1);
////    }

////private:
////    typedef void (*stub_type)(void* object_ptr, int);

////    void* object_ptr;
////    stub_type stub_ptr;

////    template <class T, void (T::*TMethod)(int)>
////    static void method_stub(void* object_ptr, int a1)
////    {
////        T* p = static_cast<T*>(object_ptr);
////        return (p->*TMethod)(a1); // #2
////    }
//public:
//  ctFastDelegat()
//    : object_ptr(0)
//    , stub_ptr(0)
//  {}

//  template < class T, void (T::*TMethod)(int) >
//    static ctFastDelegat from_method(T* object_ptr)
//  {
//    ctFastDelegat d;
//    d.object_ptr = object_ptr;
//    d.stub_ptr = &delegate_stub_t< T, TMethod >::method_stub; // #1

//    return d;
//  }

//  void operator()(int a1) const
//  {
//    return (*stub_ptr)(object_ptr, a1);
//  }

//private:
//  typedef void (*stub_type)(void* object_ptr, int);

//  void* object_ptr;
//  stub_type stub_ptr;

//  template < class T, void (T::*TMethod)(int) >
//  struct delegate_stub_t
//  {
//    static void method_stub(void* object_ptr, int a1)
//    {
//      T* p = static_cast< T* >(object_ptr);
//      return (p->*TMethod)(a1); // #2

//    }
//  };
//};
