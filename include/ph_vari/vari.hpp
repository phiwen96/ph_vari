#ifndef VARI_HPP
#define VARI_HPP
#include <ph_type_list/type_list.hpp>
#include <ph_macros/macros.hpp>









template <int, typename...>
union vari {};

template <typename T, typename... U>
struct var
{
    inline static constexpr int size = sizeof... (U) + 1;
    int active {-1};

    vari <0, T, U...> value;
    
    var () = default;
    
    template <typename P>
    var (P&& arg) : value {forward <P> (arg)}, active {type_list_t <T, U...>::template find <P>}
    {
        cout << active << endl;
    }
    
    
    template <typename P>
    requires (type_list_t <T, U...>::template find <P> >= 0)
    auto operator= (P&& p) -> auto&
    {
        static constexpr int set_equal_at = type_list_t <T, U...>::template find <P>;
        
        cout << "lkmk" << endl;
        
        /**
         If active, delete current vari and all before (?)
         */
//        value.template deinit_value <0> ();
        if (active >= 0)
        {
            if (active == set_equal_at)
            {
                if constexpr (requires () {
                    true;
                })
                {
                    cout << "tjo" << endl;
                }
//                return value.template get <set_equal_at> ().value = forward <P> (p);
            } else
            {
                switch (active)
                {
                    #define X(n) \
                        value.template deinit_value <n> ();
                        SWITCH_CASE (200)
                    #undef X
                }
                auto& a = value.template get <set_equal_at> ();
                new (&a.value) P {forward <P> (p)};
                return a.value;
            }

            
            /**
             1 1 2 0
             1 1 1 2
             */
        }
        
        
//        return value.template set_equal <P> (forward <P> (p));
//        new (value.template init <set_equal_at, P> ().value = forward <P> (p)) ;
        
        
        
        
        
        
//        value.set_equal (forward <T> (t), active);
//        return *this;
    }
    friend ostream& operator<< (ostream& os, var const& v)
    {
//        os << v.value.template print <2> ();
        
        switch (v.active)
        {
#define X(n) \
os << v.value.template print <n> ();

    SWITCH_CASE (200)
#undef X
        }

        return os;
    }
};

//template <typename T>
//struct var <T>
//{
//    vari <T> value;
//
//};




struct emptyy {};

template <int I, typename T, typename... U>
//requires ((is_assignable_v<U, P> || ...))
union vari <I, T, U...>
{
    using value_type = T;
    using tail_type = vari <I + 1, U...>;
    emptyy _;
    value_type value;
    tail_type _tail;
    
//    vari ()
//    {
//
//    }
  
    
    vari (T&& t) : value {forward <T> (t)}
    {
        
    }
    
    template <typename P>
    vari (P&& p) : _tail {forward <P> (p)}
    {
//        cout << "tjo" << endl;
    }
    
    
    
    template <int i, typename P>
    requires (i == I and is_same_v <P, value_type>)
    auto init () -> auto&
    {
        return *this;
    }
    
    template <int i, typename P>
    requires (i != I and (not is_same_v <P, value_type>))
    auto init () -> auto&
    {
        return _tail.template init <i, P> ();
    }
    
    template <int from, int to>
    auto deinit_tail () -> void
    {
        
        
        if constexpr (I + 1 <= to)
        {
            _tail.template deinit_tail <from, to> ();
            
            if constexpr (from <= I)
            {
                _tail.~tail_type ();
            }
        }
    }
    
    template <int i>
    requires (i >= 0 and i != I)
    auto deinit_value () -> void
    {
        _tail.template deinit_value <i> ();
    }
    
    template <int i>
    requires (i == I)
    auto deinit_value () -> void
    {
        value.~T ();
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
    
//    template <int i>
//    auto init_tail () -> void
//    {
////        if constexpr (i > I)
////        {
//////            new (&_tail) tail_type {};
////        }
//
//
//    }

    
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
        
//        if constexpr (not is_trivially_destructible_v<T>)
//        {
//            if (active)
//                value.~T ();
//        }
        return _tail = forward <P> (p);
    }
    
    
    
    
//    vari& operator= (auto&& t)
//    {
//
//    }
    
    ~vari ()
    {
        
        cout << "~vari " << "[" << I << "]" << " ()" << endl;
    }
    
    template <int i>
    string print () const
    {
        if constexpr (I < i)
            return "1 " + _tail.template print <i> ();
        
        else if constexpr (I == i)
            return "2 " + _tail.template print <i> ();
        else if constexpr (I > i)
            return "0 " + _tail.template print <i> ();
    }
};

template <int I, typename T>
union vari <I, T>
{
    using value_type = T;
    
    
    
    emptyy _;
    T value;
    
    
    
    vari ()
    {
        
    }
    
    vari (T&& t) : value {forward <T> (t)}
    {
        
    }
    
    template <int i, typename P>
    requires (i == I and is_same_v <P, value_type>)
    auto init () -> void
    {
        
    }
    
    template <int from, int to>
    auto deinit_tail () -> void
    {
//        value.~T ();
    }
    
    template <int i>
    auto deinit_value () -> void
    {
        value.~T ();
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
        cout << "~vari " << "[" << I << "]" << " ()" << endl;
    }
    
    template <int i>
    string print () const
    {
        if constexpr (I > i)
            return "0";
        else if constexpr (I == i)
            return "2";
    }
};


#endif
