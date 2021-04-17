#ifndef VARI_HPP
#define VARI_HPP
#include <ph_type_list/type_list.hpp>
#include <ph_macros/macros.hpp>
#include "vari_macros.hpp"
#include <ph_type/type.hpp>






template <typename...>
struct var;

template <typename... T>
consteval bool is_var_v (var <T> const&... v) {return true;}

template <typename T>
consteval bool is_var_v (T const& v) {return false;}

template <typename T>
constexpr bool is_var = is_var_v (declval<T>());


template <int, typename...>
union vari {};

template <typename T, typename... U>
struct var <T, U...>
{
    #define CURRENT_VALUE(i) value_.template get <i> ().value
    #define CURRENT_TYPE(i) typename value_type::template get_value_type_with_index <i>
    #define MAX 30
    static_assert (sizeof... (U) + 1 <= MAX, "LIMIT!!!");
    inline static constexpr int size = sizeof... (U) + 1;
    
    template <typename P>
    constexpr auto same_type () const -> bool
    {
        switch (active_)
        {
        #define X(_, i, __) \
            case i: \
if constexpr (is_same_v <CURRENT_TYPE (i), P>) \
{ \
return true; \
} \
                break;
        BOOST_PP_REPEAT (MAX, X, _)
        #undef X
        }
        return false;
    }
    
    

    
    
    using value_type = vari<0, T, U...>;
    
    template <int i>
    using get_type_with_index = typename value_type::template get_value_type_with_index <i>;

    value_type value_;
    int active_ {-1};
    
    
    
    auto active () const -> int
    {
        return active_;
    }
    
    var () = default;
    
    
    var (var&& other)
        requires requires {
            requires (is_move_constructible_v<T> and (is_move_constructible_v<U> and ...));
        }
        : active_ {other.active_}
    {
            switch (active_)
            {
            #define X(_, i, __) \
                case i: \
                    new (&CURRENT_VALUE (i)) CURRENT_TYPE (i) {move (other.CURRENT_VALUE (i))}; \
                    break;
            BOOST_PP_REPEAT (MAX, X, _)
            #undef X
            }
    }
    
    
    var (var const& other)
        requires requires {
            requires (is_copy_constructible_v<T> and (is_copy_constructible_v<U> and ...));
        }
        : active_ {other.active_}
    {
        switch (active_)
        {
        #define X(_, i, __) \
            case i: \
                CURRENT_VALUE (i) = other.CURRENT_VALUE (i); \
                break;
        BOOST_PP_REPEAT (MAX, X, _)
        #undef X
        }
//        cout << "mohaha" << endl;
    }
    
    auto operator= (var const& other) -> var&
        requires requires () {
            requires (is_copy_assignable_v<T> and (is_copy_assignable_v<U> and ...));
        }
    {
        if (active_ >= 0)
        {
            
            if (active_ != other.active_)
            {
                /**
                    call current value's destructor
                 */
                switch (active_)
                {
                #define X0(_, rhs, lhs) \
                    case rhs: \
                        if constexpr (requires {CURRENT_VALUE (lhs) = other.CURRENT_VALUE (rhs);}) \
                        { \
                            CURRENT_VALUE (lhs) = other.CURRENT_VALUE (rhs); \
                        } \
                        break;
                        
                #define X(_, i, __) \
                    case i: \
                        CURRENT_VALUE (i).~decltype (CURRENT_VALUE (i)) (); \
                        switch (other.active_) \
                        { \
                            BOOST_PP_REPEAT (MAX, X0, _) \
                        } \
                        break;
                        
                BOOST_PP_REPEAT (MAX, X, _)
                #undef X0
                #undef X
                }
                active_ = other.active_;
            } else
            {
                switch (active_)
                {
                #define X(_, i, __) \
                    case i: \
                        CURRENT_VALUE (i) = other.CURRENT_VALUE (i); \
                        break;
                        
                BOOST_PP_REPEAT (MAX, X, _)
                #undef X
                }
            }
        } else
        {
            switch (other.active_)
            {
            #define X(_, i, __) \
                case i: \
                    if constexpr (requires {CURRENT_TYPE (i) {other.CURRENT_VALUE (i)};}) \
                    { \
                        new (&value_.template get <i> ().value) decltype (CURRENT_VALUE (i)) {other.CURRENT_VALUE (i)}; \
                    } \
                    break;
                    
            BOOST_PP_REPEAT (MAX, X, _)
            #undef X
            }
            active_ = other.active_;
        }
        
        
        
        
        
        
        return *this;
    }
    
    auto operator= (var&& other) -> var&
        requires requires {
            requires (is_move_assignable_v<T> and (is_move_assignable_v<U> and ...));
        }
    {
        if (active_ >= 0)
        {
            
            if (active_ != other.active_)
            {
                /**
                    call current value's destructor
                 */
                switch (active_)
                {
                #define X0(_, rhs, lhs) \
                    case rhs: \
                        if constexpr (requires {CURRENT_VALUE (lhs) = move (other.CURRENT_VALUE (rhs));}) \
                        { \
                            CURRENT_VALUE (lhs) = move (other.CURRENT_VALUE (rhs)); \
                        } \
                        break;
                        
                #define X(_, i, __) \
                    case i: \
                        CURRENT_VALUE (i).~decltype (CURRENT_VALUE (i)) (); \
                        switch (other.active_) \
                        { \
                            BOOST_PP_REPEAT (MAX, X0, _) \
                        } \
                        break;
                        
                BOOST_PP_REPEAT (MAX, X, _)
                #undef X0
                #undef X
                }
                active_ = other.active_;
            } else
            {
                switch (active_)
                {
                #define X(_, i, __) \
                    case i: \
                        CURRENT_VALUE (i) = move (other.CURRENT_VALUE (i)); \
                        break;
                        
                BOOST_PP_REPEAT (MAX, X, _)
                #undef X
                }
            }
        } else
        {
            switch (other.active_)
            {
                
            #define X(_, i, __) \
                case i: \
                    if constexpr (requires {CURRENT_TYPE (i) {move (other.CURRENT_VALUE (i))};}) \
                    { \
                        new (&value_.template get <i> ().value) decltype (CURRENT_VALUE (i)) {move (other.CURRENT_VALUE (i))}; \
                    } \
                    break;
                    
            BOOST_PP_REPEAT (MAX, X, _)
            #undef X
            }
            active_ = other.active_;
        }
        
        
        
        
        
        
        return *this;
    }
    
    template <typename P>
    var (P&& arg)
        requires requires {
            requires type_list_t <T, U...>::template find <P> >= 0;
            P {move (arg)};
        }
        : value_ {move (arg)}, active_ {type_list_t <T, U...>::template find <P>}
    {
        
    }
    
    template <typename P>
    var (P const& arg)
        requires requires {
            requires type_list_t <T, U...>::template find <P> >= 0;
            P {arg};
        }
        : value_ {arg}, active_ {type_list_t <T, U...>::template find <P>}
    {
        
    }
    
    /**
     set current value equal to
     */
    template <typename P>
    requires requires {
        requires false;
//        requires vari_value_type_exists <P>;
    }
    auto operator= (P&& p) -> auto&
    {
        /**
         If active_, delete current vari and all before (?)
         */
//        value.template deinit_value <0> ();
        if (active_ == get_vari_index_from_type <P>)
        {
            if constexpr (requires (P& pp){pp = forward <P> (p);})
            {
                return value_.template get <get_vari_index_from_type <P>> ().value = forward <P> (p);
            } else
            {
                throw runtime_error ("not assignable");
            }
        } else if (active_ >= 0)
        {
            switch (active_)
            {
            #define X(n) \
                case n: \
                    value_.template deinit_value <n> (); \
                break;
                    
                    FOR (MAX, X)
            #undef X
            }
            auto& a = value_.template get <get_vari_index_from_type <P>> ();
            new (&a.value) P {forward <P> (p)};
            active_ = get_vari_index_from_type <P>;
            return a.value;
        } else
        {
            auto& a = value_.template get <get_vari_index_from_type <P>> ();
            new (&a.value) P {forward <P> (p)};
            active_ = get_vari_index_from_type <P>;
            return a.value;
        }
    }
    
    
    template <typename... B>
    var (var <B...> const& other) requires requires {
        requires false;
    } : value_ {[&other]()->auto const& {
        switch (other.active_)
        {
        #define X0(z, i, arg) \
            case i: \
                return other.CURRENT_VALUE (i); \
                break;
        BOOST_PP_REPEAT (MAX, X0, _)
        #undef X0
        }
    }}
    {
        cout << "hi" << endl;
        active_ = other.active_;
    }
    
    
    
    
    
    
    
    
    template <typename... B>
    var (var <B...> && other)
    requires requires {
        requires false;
    }
    : var ()
    {
        switch (other.active_)
        {
        #define ALIAS(i) typename var <B...>::vari_type::template get_by_index <i>
        #define X(z, i, arg) \
            case i: \
                if constexpr (is_same_v <ALIAS (i), decltype (value_.template get <ALIAS (i)> ().value)> and requires (ALIAS (i)& a0, ALIAS (i)& a1){swap (a0, a1);}) \
                { \
                    active_ = get_vari_index_from_type <ALIAS (i)>; \
new (&value_.template get <ALIAS (i)> ().value) ALIAS (i) {move (other.value_.template get <i> ().value)}; \
                } \
                break;
        BOOST_PP_REPEAT (MAX, X, _)
        #undef ALIAS
        #undef X
        #undef X0
        }
    }
    
    template <typename A>
    requires requires () {
        requires is_same_v <A, T> or (is_same_v <A, U> or ...);
    }
    auto get () -> auto&
    {
        return value_.template get <A> ().value;
    }
    
    template <typename A>
    requires requires () {
        requires is_same_v <A, T> or (is_same_v <A, U> or ...);
    }
    auto get () -> auto const&
    {
        return value_.template get <A> ().value;
    }
    
    template <typename A>
    requires requires {
        requires false;
    }
    operator A const& () const
    {
        if (active_ < 0)
        {
            throw runtime_error ("not active_!");
        }
        
        switch (active_)
        {
        #define X(_, i, __) \
            case i: \
                if constexpr (is_convertible_v <CURRENT_TYPE (i), A>) \
                    return CURRENT_VALUE (i); \
                else \
                    throw runtime_error ("not convertible"); \
                break;
                
            BOOST_PP_REPEAT (MAX, X, _)
        #undef X
        }
    }
    
    template <typename A>
    operator A & ()
    requires requires {
        requires type_list_t <T, U...>::template find <A> >= 0;
    }
    {
        return value_.template get <A> ().value;
    }
    
    template <typename A>
    operator A & ()
    requires requires {
        requires type_list_t <T, U...>::template find <A> == -1;
    }
    {
        if (active_ < 0)
        {
            throw runtime_error ("not active_!");
        }

        switch (active_)
        {
        #define X(_, i, __) \
            case i: \
                if constexpr (is_convertible_v <CURRENT_TYPE (i), A>) \
                    return CURRENT_VALUE (i); \
                else \
                    throw runtime_error ("not convertible"); \
                break;

            BOOST_PP_REPEAT (MAX, X, _)
        #undef X

        }
    }
    
    template <typename A>
    requires requires {
        requires false;
    }
    operator A && ()
    {
        if (active_ < 0)
        {
            throw runtime_error ("not active_!");
        }
        
        switch (active_)
        {
        #define X(_, i, __) \
            case i: \
                if constexpr (is_convertible_v <CURRENT_TYPE (i), A>) \
                    return move (CURRENT_VALUE (i)); \
                else \
                    throw runtime_error ("not convertible"); \
                break;
                
            BOOST_PP_REPEAT (MAX, X, _)
        #undef X
            
        }
    }
    
    
    
    
    
    int af = requires {
        requires (1 == 1);
    };
    template <typename P>
    inline static constexpr bool vari_value_type_exists = requires {
        requires (type_list_t <T, U...>::template find <P> >= 0);
    };
    
//    template <int i>
//    using vari_type =
    
    template <typename P>
//    requires requires () {
//        requires (vari_value_type_exists <P>);
//    }
    inline static constexpr int get_vari_index_from_type = type_list_t <T, U...>::template find <P>;
    
    
    

//    template <typename P>
//    inline static constexpr
    
    
    
//    template <typename P>
//    requires (vari_value_type_exists <P>)
//    auto operator= (transform_t <P> const&) -> auto&
//    {
//        /**
//         vari <int, string> v = int {2};
//         v = "hej"   ERROR
//
//         v = transform <string>;
//         v = "hej";
//         */
//    }
    
    
    
    
    
    
    
    
    template <typename P, typename... Q>
    requires requires {
        requires false;
    }
    auto operator= (var <P, Q...> const & other) -> var&
        requires requires {
            requires false;
        }
    {
        #define if_left_active__equal_comparible_to_right_active_(lhs, rhs) \
        active_ = lhs; \
        value_.template get <lhs> ().value = other.value_.template get <rhs> ().value; \
        return *this;
                
        #define IF_TRUE(lhs, rhs) \
            if constexpr (requires {value_.template get <lhs> ().value = other.value_.template get <rhs> ().value;}) \
            { \
                if_left_active__equal_comparible_to_right_active_ (lhs, rhs) \
            }
                
        #define IF_FALSE(lhs, rhs) \
            else \
            { \
               throw runtime_error ("not doable!"); \
            } \

                
        #define is_left_active__equal_comparible_to_right_active_(z, rhs, lhs) \
            case rhs: \
                IF_TRUE (lhs, rhs) \
                IF_FALSE (lhs, rhs) \
                break;
        //                cout << "hi" << endl, \
//                cout << "no" << endl);



        switch (active_)
        {
            #define X(z, lhs, nested) \
                case lhs: \
                switch (other.active_) \
                { \
                    BOOST_PP_REPEAT (MAX, nested, lhs) \
                } \
                break;
                
                BOOST_PP_REPEAT (MAX, X, is_left_active__equal_comparible_to_right_active_)
                
            #undef if_left_active__equal_comparible_to_right_active_
            #undef IF_TRUE
            #undef IF_FALSE
            #undef is_left_active__equal_comparible_to_right_active_
            #undef X
        }
    }
    

    friend void swap (var & lhs, var & rhs)
    requires requires {
        requires false;
    }
    {
        using std::swap;
        if constexpr (not requires {swap (lhs.value_.template get <0> ().value, rhs.value_.template get <0> ().value);})
        {
        throw runtime_error ("not swappable");
        }
//        swap (lhs.active_, rhs.active_);
//        swap (lhs.value_.template get <i> ().value, rhs.value_.template get <i> ().value); \

#define X0(_, rhs, lhs) \
case rhs: \
if constexpr (not requires {swap (lhs.value_.template get <lhs> ().value, rhs.value_.template get <rhs> ().value);}) \
{ \
throw runtime_error ("not swappable"); \
} \
else \
{ \
swap (lhs.value_.template get <lhs> ().value, rhs.value_.template get <rhs> ().value); \
swap (lhs.active_, rhs.active_); \
} \
break;
        switch (lhs.active_)
        {
        #define X(_, i, __) \
            case i: \
                switch (rhs.active_) \
                { \
                    BOOST_PP_REPEAT (MAX, X0, i) \
                } \
                break;
    //        swap (lhs.value_, rhs.value_);
//            BOOST_PP_REPEAT (MAX, X, _)
        }
    }
    
    template <typename... B>
    requires requires {
        requires false;
    }
    friend void swap (var & lhs, var <B...> & rhs)
        requires requires {
            requires false;
        }
    {
        using std::swap;
        
    #define X1(z, rhs_i, lhs_i) \
        case rhs: \
            if constexpr (requires {swap (lhs.value.template get <lhs_i> (), rhs.value.template get <rhs_i> ());}) \
            { \
                lhs.active_ = lhs; \
                rhs.active_ = lhs; \
                swap (lhs.value.template get <lhs_i> (), rhs.value.template get <rhs_i> ()); \
            } else \
            { \
                throw runtime_error ("not swappable"); \
            } \
        break;
        
    
        switch (lhs.active_)
        {
        #define X0(z, n, arg) \
            case n: \
                switch (rhs.active_) \
                { \
                    BOOST_PP_REPEAT (MAX, X1, n) \
                } \
            break;
                BOOST_PP_REPEAT (MAX, X0, k)
        #undef X1
        #undef X0

        }
    }

    template <typename P, typename... Q>
    auto operator= (var <P, Q...>&& other) -> var&
        requires requires {
            requires false;
        }
    {
        swap (*this, other);
        return *this;
        #define if_left_active__equal_comparible_to_right_active_(lhs, rhs) \
            value_.template get <lhs> ().value = move (other.value_.template get <rhs> ().value); \
            return *this;
                
        #define IF_TRUE(lhs, rhs) \
            if constexpr (requires {value_.template get <lhs> ().value = other.value_.template get <rhs> ().value;}) \
            { \
                if_left_active__equal_comparible_to_right_active_ (lhs, rhs) \
            }
                
        #define IF_FALSE(lhs, rhs) \
            else \
            { \
               throw runtime_error ("not doable!"); \
            } \

                
        #define is_left_active__equal_comparible_to_right_active_(z, rhs, lhs) \
            case rhs: \
                IF_TRUE (lhs, rhs) \
                IF_FALSE (lhs, rhs) \
                break;
        //                cout << "hi" << endl, \
//                cout << "no" << endl);



        switch (active_)
        {
            #define X(z, lhs, nested) \
                case lhs: \
                switch (other.active_) \
                { \
                    BOOST_PP_REPEAT (MAX, nested, lhs) \
                } \
                break;
                
                BOOST_PP_REPEAT (MAX, X, is_left_active__equal_comparible_to_right_active_)
            #undef X
//            FOR(10, X)
        }
        
        
        

        cout << "hora" << endl;
//        return get_current().value = other.get_current.value ();
//#define X(n) \
//return value.template get <
//        SWITCH_CASE (active_)
    }

    
    
    auto print () const -> void
    {
        switch (active_)
        {
            #define X(n) \
                case n: \
                cout << value_.template print <n> (); \
                break;

                            FOR (MAX, X)
            #undef X
        }
    }
    
    friend ostream& operator<< (ostream& os, var const& v)
    {
//        os << v.value.template print <2> ();
        if (v.active_ < 0)
        {
            os << "bajs";
            return os;
        }
        
        switch (v.active_)
        {
            #define X(_, n, __) \
                case n: \
                os << v.value_.template get <n> ().value; \
                break;
                            BOOST_PP_REPEAT (MAX, X, _)
            #undef X
        }

        return os;
    }
    
    
    
    ~var ()
    {
        if (active_ < 0)
        {
            return;
        }
        
        switch (active_)
        {
        #define X(_, i, __) \
            case i: \
                value_.template get <i> ().value.~decltype(value_.template get <i> ().value)(); \
                break;
                
                BOOST_PP_REPEAT (MAX, X, _)
        #undef X
        }
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
    
    template <typename P>
    using get_by_type = conditional_t <is_same_v <P, T>, T, typename tail_type::template get_by_type <P>>;
    
    template <int i>
    using get_value_type_with_index = conditional_t <i == I, value_type, typename tail_type::template get_value_type_with_index <i>>;
  

    
    vari () : _ {}
    {
        
    }
 
    
//    vari (int i, vari && other)
//    {
//        if constexpr (I == 0)
//        {
//            if (i == -1)
//            {
//                return;
//            }
//        }
//        if (i == I)
//        {
//            new (&value) T {move (other.value)};
//        } else
//        {
//            new (&_tail) tail_type {i, move (other._tail)};
//        }
//    }

    
//    vari (int i, vari const& other)// : value {other.value}, _tail {other.tail}
//    requires requires () {
//        new (&value) T {other.value};
//        new (&_tail) tail_type {i, other._tail};
//    }
//    {
//        if (i == I)
//        {
//            new (&value) T {other.value};
//        } else
//        {
//            new (&_tail) tail_type {i, other._tail};
//        }
//    }
    
    
    vari (value_type&& t)
        requires requires {
            value_type {move(t)};
        }
        : value {move (t)}
    {
        
    }
    
    vari (value_type const& t)
        requires requires {
            value_type {t};
        }
        : value {t}
    {
        
    }
    
    template <typename P>
    vari (P&& p)
    requires requires (P&& v) {
        tail_type {forward <P> (v)};
    }
    : _tail {forward <P> (p)}
    {

    }
    
    
    

    


    template <int i>
    auto get () -> auto&
    {
        if constexpr (i != I)
            return _tail.template get <i> ();
        else
            return *this;
    }

    template <typename P>
    auto get () -> auto&
    {
        if constexpr (not is_same_v <T, P>)
            return _tail.template get <P> ();
        else
            return *this;
    }
    
    template <int i>
    auto get () const -> auto const&
    {
        if constexpr (i != I)
            return _tail.template get <i> ();
        else
            return *this;
    }

    template <typename P>
    auto get () const -> auto const&
    {
        if constexpr (not not is_same_v <T, P>)
            return _tail.template get <P> ();
        else
        return *this;
    }
    

    
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
    
    template <typename P>
    using get_by_type = T;
    
    template <int i>
    using get_value_type_with_index = value_type;
    
    emptyy _;
    T value;

    
    vari () : _ {}
    {
        
    }
    
    vari (T&& t) : value {forward <T> (t)}
    {
        
    }
    
    vari (T const& t) : value {t}
    {
        
    }
    
    template <int i>
    auto get () -> auto&
    {
        return *this;
    }

    template <typename P>
    auto get () -> auto&
    {
        return *this;
    }
    
    template <int i>
    auto get () const -> auto const&
    {
        return *this;
    }

    template <typename P>
    auto get () const -> auto const&
    {
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



//template <typename T, typename name


#endif
