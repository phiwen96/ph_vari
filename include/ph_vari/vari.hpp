#ifndef VARI_HPP
#define VARI_HPP
#include <ph_type_list/type_list.hpp>
#include <ph_macros/macros.hpp>



template <typename T>
struct transform_t
{
    
};

template <typename T>
inline static constexpr transform_t <T> transform {};






template <int, typename...>
union vari {};

template <typename T, typename... U>
struct var
{
    #define MAX 10
    inline static constexpr int size = sizeof... (U) + 1;
    int active {-1};

    vari <0, T, U...> value;
    
    var () = default;
    
    template <typename P>
    var (P&& arg) : value {forward <P> (arg)}, active {type_list_t <T, U...>::template find <P>}
    {
//        cout << active << endl;
    }
    int af = requires {
        requires (1 == 1);
    };
    template <typename P>
    inline static constexpr bool vari_value_type_exists = requires {
        requires (type_list_t <T, U...>::template find <P> >= 0);
    };
    
    template <typename P>
    requires requires () {
        requires (vari_value_type_exists <P>);
    }
    inline static constexpr int get_vari_index_from_type = type_list_t <T, U...>::template find <P>;
    
    
    

//    template <typename P>
//    inline static constexpr
    
    
    
    template <typename P>
    requires (vari_value_type_exists <P>)
    auto operator= (transform_t <P> const&) -> auto&
    {
        /**
         vari <int, string> v = int {2};
         v = "hej"   ERROR
         
         v = transform <string>;
         v = "hej";
         */
    }
    
    
    
    /**
     set current value equal to
     */
    template <typename P>
    requires (vari_value_type_exists <P>)
    auto operator= (P&& p) -> auto&
    {
        
        cout << "lkmk" << endl;
        
        /**
         If active, delete current vari and all before (?)
         */
//        value.template deinit_value <0> ();
        if (active == get_vari_index_from_type <P>)
        {
            if constexpr (requires (P& pp){pp = forward <P> (p);})
            {
                return value.template get <get_vari_index_from_type <P>> ().value = forward <P> (p);
            } else
            {
                throw runtime_error ("not assignable");
            }
        } else if (active >= 0)
        {
            switch (active)
            {
                #define X(n) \
                    value.template deinit_value <n> ();
                    SWITCH_CASE (MAX)
                #undef X
            }
            auto& a = value.template get <get_vari_index_from_type <P>> ();
            new (&a.value) P {forward <P> (p)};
            active = get_vari_index_from_type <P>;
            return a.value;
        } else
        {
            auto& a = value.template get <get_vari_index_from_type <P>> ();
            new (&a.value) P {forward <P> (p)};
            active = get_vari_index_from_type <P>;
            return a.value;
        }
    }
    
    
    
    template <typename P, typename... Q>
    auto operator= (var <P, Q...>& other)
    {
        
#define DECL2(z, n, k) \
    case n: \
        if constexpr (requires {value.template get <k> ().value = other.value.template get <n> ().value;}) \
        { \
            cout << "tju" << endl; \
            value.template get <k> ().value = other.value.template get <n> ().value; \
        } else \
        { \
            throw runtime_error ("not assignable!"); \
        } \
        break;
        //                cout << "hi" << endl, \
//                cout << "no" << endl);


        switch (active)
        {
            #define X(n) \
                switch (other.active) \
                { \
                    SWITCH_CASE_2 (DECL2, 10, n) \
                }
                
            SWITCH_CASE(10)
        }
        
        
        

        cout << "hora" << endl;
//        return get_current().value = other.get_current.value ();
//#define X(n) \
//return value.template get <
//        SWITCH_CASE (active)
    }
    
    
    
    friend ostream& operator<< (ostream& os, var const& v)
    {
//        os << v.value.template print <2> ();
        
        switch (v.active)
        {
#define X(n) \
os << v.value.template print <n> ();

    SWITCH_CASE (MAX)
#undef X
        }

        return os;
    }
    #undef MAX
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
    
//    template <int i>
//    requires (i != I)
//    auto get () -> auto&
//    {
//        return _tail.template get <i> ();
//    }
    template <int i>
//    requires (i == I)
    auto get () -> auto&
    {
        if constexpr (i != I)
            return _tail.template get <i> ();
        else
            return *this;
    }
    
//    template <typename P>
//    requires (not is_same_v <T, P>)
//    auto get () -> auto&
//    {
//        return _tail.template get <P> ();
//    }
    
    template <typename P>
//    requires (is_same_v <T, P>)
    auto get () -> auto&
    {
        if constexpr (not not is_same_v <T, P>)
            return _tail.template get <P> ();
        else
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
        
//        cout << "~vari " << "[" << I << "]" << " ()" << endl;
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
//    requires (i == I)
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
//        cout << "~vari " << "[" << I << "]" << " ()" << endl;
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
