#ifndef VARI_HPP
#define VARI_HPP
#include <ph_type_list/type_list.hpp>
#include <ph_macros/macros.hpp>









template <int, int, typename...>
union vari {};

template <typename T, typename... U>
struct var
{
    inline static constexpr int size = sizeof... (U) + 1;
    int active {-1};

    vari <0, -1, T, U...> value;
    
    var () = default;
    
    
    template <typename P>
    requires (type_list_t <T, U...>::template find <P> >= 0)
    auto operator= (P&& t) -> auto&
    {
        cout << "lkmk" << endl;
        
        /**
         If active, delete current vari and all before (?)
         */
        if (active >= 0)
        {
            switch (active)
            {
                #define X(n) \
                    cout << n << endl;
                    
                    SWITCH_CASE (200, cout << "")
                #undef X
            }
        } else
        {
            value.template init <type_list_t <T, U...>::template find <P>, P> ();
        }
        
        
        
        
        
        
//        value.set_equal (forward <T> (t), active);
        return *this;
    }
};

//template <typename T>
//struct var <T>
//{
//    vari <T> value;
//
//};




struct emptyy {};

template <int I, int construct, typename T, typename... U>
//requires ((is_assignable_v<U, P> || ...))
union vari <I, construct, T, U...>
{
    using value_type = T;
    using tail_type = vari <I + 1, construct, U...>;
    emptyy _;
    value_type value;
    tail_type _tail;
    
    vari ()
        requires (construct > 0 and construct != I)
    {
            new (&_tail) tail_type;
    }
    
    vari ()
        requires (construct == I)
    {
            new (&value) value_type {};
    }
    
    vari ()
    requires (construct == -1)
    {
        
    }
    
    template <int i, typename P>
    requires (i == I and is_same_v <P, value_type>)
    auto init () -> void
    {
        cout << "yes" << endl;
    }
    
    template <int i, typename P>
    requires (i != I and (not is_same_v <P, value_type>))
    auto init () -> void
    {cout << "not" << endl;
        _tail.template init <i, P> ();
    }
    
    template <int i>
    requires (i != I)
    auto get () -> auto&
    {
        return _tail.template get <i> ();
    }
    template <int i>
    requires (i == I)
    auto get () -> auto&
    {
        return *this;
    }
    
    template <typename P>
    requires (not is_same_v <T, P>)
    auto get () -> auto&
    {
        return _tail.template get <P> ();
    }
    
    template <typename P>
    requires (is_same_v <T, P>)
    auto get () -> auto&
    {
        return *this;
    }
    
    

    
    auto operator= (T&& t) -> auto&
    {
        value = forward <decltype (t)> (t);
        return value;
    }
    
    
    
    void set_equal (T&& t, int i)
    {
        if (i == I)
        {
            
            value = forward <T> (t);
        } else
        {

            clear_value();
        
            new (_tail) decltype (_tail) {i};
        }
    }
    
    template <typename P>
    auto set_equal (P&& p, int i) -> void
    {
        
    }
    
    
    constexpr auto clear_value () -> void
    {
        if constexpr (not is_trivially_destructible_v <T>)
        {
            value.~T ();
        }
    }
    
    template <typename P>
    requires ((is_assignable_v<U, P> || ...))
    auto operator= (P&& p) -> auto&
    {
//        if constexpr (not is_trivially_destructible_v <P>)
//            _t.~T();
        bool active = true;
        
        if constexpr (not is_trivially_destructible_v<T>)
        {
            if (active)
                value.~T ();
        }
        return _tail = forward <P> (p);
    }
    
    
    
    
//    vari& operator= (auto&& t)
//    {
//
//    }
    
    ~vari ()
    {
        
        cout << "~kiss ()" << endl;
    }
};

template <int I, int construct, typename T>
union vari <I, construct, T>
{
    using value_type = T;
    
    
    
    emptyy _;
    T value;
    
    
    
    vari ()
        requires (construct == I)
    {
            new (&value) T {};
    }
    
    template <int i, typename P>
    requires (i == I and is_same_v <P, value_type>)
    auto init () -> void
    {
        
    }
    
    template <int i>
    requires (i == I)
    auto get () -> auto&
    {
        return *this;
    }

    template <typename P>
    requires (is_same_v <T, P>)
    auto get () -> auto&
    {
        return *this;
    }
    
    vari& operator= (auto&& t)
    {
        value = forward <decltype (t)> (t);
        return *this;
    }
    
    
    
    ~vari ()
    {

    }
};


#endif
