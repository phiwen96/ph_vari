#include <catch2/catch.hpp>
#include <ph_vari/vari.hpp>
#include <ph_time/timer.hpp>
#include <ph_type_list/type_list.hpp>
#include <ph_concepts/concepts.hpp>
using namespace std;

/**
 1 move constructible
 1 copy constructible
 0 move assignable
 1 copy assignable
 */
struct A
{
    inline static int alive {0};
    A& operator= (A const&) {return *this;}
    A& operator= (A &&) = delete;
    A (A const&){++alive; /*cout << "A (A&&)" << endl;*/}
    A (A&&){++alive; /*cout << "A (A&&)" << endl;*/}
//    A (A&&) = delete;
    A (){++alive; /*cout << "A ()" << endl;*/}
    ~A (){--alive; /*cout << "~A ()" << endl;*/}

//    A& operator= (A other)
//    {
//        return *this;
//    }
    friend ostream& operator<< (ostream& os, A const& a)
    {
        return os << "A";
    }
};

/**
 1 move constructible
 0 copy constructible
 1 move assignable
 0 copy assignable
 */
struct B
{
    B (B const&) = delete;
    B& operator= (B &&) {return *this;}
    B (B&&){/*cout << "B (B&&)" << endl;*/}
    B (){/*cout << "B ()" << endl;*/}
    ~B (){/*cout << "~B ()" << endl;*/}
    friend ostream& operator<< (ostream& os, B const& d)
    {
        return os << "B";
    }
};

/**
 0 move constructible
 1 copy constructible
 1 move assignable
 1 copy assignable
 */
struct C
{
    C& operator= (C &&) {return *this;}
    C& operator= (C const&) {return *this;}
    C (C const&){/*cout << "C (C const&)" << endl;*/}
//    C (C&&){cout << "C (C&&)" << endl;}
    C (C&&) = delete;
    C (){/*cout << "C ()" << endl;*/}
    ~C (){/*cout << "~C ()" << endl;*/}
    friend ostream& operator<< (ostream& os, C const& d)
    {
        return os << "C";
    }
};


/**
 1 move constructible
 1 copy constructible
 1 move assignable
 1 copy assignable
 */
struct D
{
    D& operator= (D &&) {return *this;}
    D& operator= (D const&) {return *this;}
    D (D const&){/*cout << "D (D const&)" << endl;*/}
    D (D&&){/*cout << "D (D&&)" << endl;*/}
    D (){/*cout << "D ()" << endl;*/}
    ~D (){/*cout << "~D ()" << endl;*/}
    friend ostream& operator<< (ostream& os, D const& d)
    {
        return os << "D";
    }
};

static_assert (is_copy_constructible_v <A>);
static_assert (not is_copy_constructible_v <B>);
static_assert (is_copy_constructible_v <C>);
static_assert (is_copy_constructible_v <D>);

static_assert (is_move_constructible_v <A>);
static_assert (is_move_constructible_v <B>);
static_assert (not is_move_constructible_v <C>);
static_assert (is_move_constructible_v <D>);

static_assert (not is_move_assignable_v <A>);
static_assert (is_move_assignable_v <B>);
static_assert (is_move_assignable_v <C>);
static_assert (is_move_assignable_v <D>);

static_assert (is_copy_assignable_v <A>);
static_assert (not is_copy_assignable_v <B>);
static_assert (is_copy_assignable_v <C>);
static_assert (is_copy_assignable_v <D>);


/**
 1 trivially default constructible
 1 trivially copy constructible
 1 trivially move constructible
 
 1 trivially copy assignable
 1 trivially move assignable
 */
template <int>
struct E
{
    E& operator= (E &&) = default;
    E& operator= (E const&) = default;
    E (E const&) = default;
    E (E&&) = default;
    E () = default;
    ~E () = default;//{/*cout << "~D ()" << endl;*/}
    friend ostream& operator<< (ostream& os, E const& d)
    {
        return os << "E";
    }
};

/**
 0 trivially default constructible
 0 trivially copy constructible
 0 trivially move constructible
 
 0 trivially copy assignable
 0 trivially move assignable
 */
template <int>
struct F
{
    F& operator= (F &&) {return *this;}
    F& operator= (F const&) {return *this;}
    F (F const&){/*cout << "D (D const&)" << endl;*/}
    F (F&&){/*cout << "D (D&&)" << endl;*/}
    F () {cout << "F ()" << endl;}
    ~F () = default;//{/*cout << "~D ()" << endl;*/}
    friend ostream& operator<< (ostream& os, F const& d)
    {
        return os << "F";
    }
};

/**
 1 trivially default constructible
 0 trivially default destructible
 */
struct G
{
    G& operator= (G &&) {return *this;}
    G& operator= (G const&) {return *this;}
    G (G const&){/*cout << "D (D const&)" << endl;*/}
    G (G&&){/*cout << "D (D&&)" << endl;*/}
    G () = default;
    ~G () {}//{/*cout << "~D ()" << endl;*/}
    friend ostream& operator<< (ostream& os, G const& d)
    {
        return os << "G";
    }
};

/**
 0 trivially default constructible
 0 trivially default destructible
 */
struct H
{
    H& operator= (H &&) {return *this;}
    H& operator= (H const&) {return *this;}
    H (H const&){/*cout << "D (D const&)" << endl;*/}
    H (H&&){/*cout << "D (D&&)" << endl;*/}
    H () {}
    ~H () {}//{/*cout << "~D ()" << endl;*/}
    friend ostream& operator<< (ostream& os, H const& d)
    {
        return os << "H";
    }
};

static_assert (is_trivially_default_constructible_v <E <0>>);
static_assert (not is_trivially_default_constructible_v <F <0>>);

static_assert (is_trivially_copy_constructible_v <E <0>>);
static_assert (not is_trivially_copy_constructible_v <F <0>>);

static_assert (is_trivially_move_constructible_v <E <0>>);
static_assert (not is_trivially_move_constructible_v <F <0>>);

static_assert (is_trivially_copy_assignable_v <E <0>>);
static_assert (not is_trivially_copy_assignable_v <F <0>>);

static_assert (is_trivially_move_assignable_v <E <0>>);
static_assert (not is_trivially_move_assignable_v <F <0>>);








union U0
{
    E <0> e;
    F <0> f;
};




template <typename T, typename... U>
union u
{
    using value_type = T;
    
    value_type value_;
    u <U...> tail_;
    
    u () requires requires {
        requires (is_trivially_default_constructible_v <T> and (is_trivially_default_constructible_v <U> and ...));
    } = default;
    u () requires requires {
        requires (not is_trivially_default_constructible_v <T> or (not is_trivially_default_constructible_v <U> or ...));
    } {
        
    }

    ~u () requires requires {
        requires (is_trivially_destructible_v <T> and (is_trivially_destructible_v <U> and ...));
    } = default;
    ~u () requires requires {
        requires (not is_trivially_destructible_v <T> or (not is_trivially_destructible_v <U> or ...));
    } {
        
    }
};

template <typename T>
union u <T>
{
    using value_type = T;
    
    value_type value_;
    
    u () requires requires {
        requires (is_trivially_default_constructible_v <T>);
    } = default;
    u () requires requires {
        requires (not is_trivially_default_constructible_v <T>);
    } {
        
    }
    
    ~u () requires requires {
        requires (is_trivially_destructible_v <T>);
    } = default;
    ~u () requires requires {
        requires (not is_trivially_destructible_v <T>);
    } {
        
    }
};



template <typename T, typename... U>
struct v
{

    u <T, U...> value_;
    
    v (auto&& arg) : value_ {}
    {
        []<size_t... I>(index_sequence<I...>){
            ((cout << I << endl), ...);
        }(make_index_sequence <sizeof... (U) + 1> ());
    }
};


template <int>
struct alignas (16) big
{
    
};





template <typename...>
struct zero_value_t;

template <typename T>
requires (is_arithmetic_v <T>)
struct zero_value_t <T>
{
    inline static constexpr T value {0};
};

template <typename T>
requires (not is_arithmetic_v <T>)
struct zero_value_t <T>
{
    inline static constexpr T value {};
};

template <typename... T>
constexpr auto zero_value = zero_value_t <T...>::value;




template <auto...>
struct next_value_t;

template <auto t>
constexpr auto next_value = next_value_t <t>::value;

template <auto t>
requires requires {
    t + 1;
}
struct next_value_t <t>
{
    inline static constexpr decltype (t) value = t + 1;
};

template <auto...>
struct previous_value_t;

template <auto t>
requires requires {
    t - 1;
}
struct previous_value_t <t>
{
    inline static constexpr decltype (t) value = t - 1;
};

template <auto... t>
constexpr auto previous_value = previous_value_t <t...> {};

template <auto...>
struct finished_value_t;

template <auto a>
struct finished_value_t <a>
{
    template <auto b>
    inline static constexpr bool value = a == b;
};

//template <auto a, auto b>
//constexpr auto finished_value = finished_value_t <a, b>::value;

template <auto...>
struct value_t;

template <auto... t>
constexpr auto value = value_t <t...> {};

template <auto t>
struct value_t <t>
{
    constexpr auto operator+ (decltype (t) o) -> value_t const&
    {
        return value <next_value <t + o>>;
    }
};



struct te {};

template <char t>
struct value_t <t>
{
    constexpr auto operator+ (decltype (t) o) -> value_t const&
    {
        return value <next_value <t - o>>;
    }
};

template <auto...>
struct begin_value_t;

template <auto a, auto b>
struct begin_value_t <a, b>
{
    using value_type = decltype (a);
    inline static constexpr value_type value = a;
};

template <auto... t>
constexpr auto begin_value = begin_value_t <t...>::value;



template <auto...>
struct end_value_t;

template <auto a, auto b>
struct end_value_t <a, b>
{
    using value_type = decltype (b);
    inline static constexpr value_type value = b;
};

template <auto... t>
constexpr auto end_value = end_value_t <t...>::value;



template <typename... T>
struct size_of_t
{
    inline static constexpr size_t value = sizeof... (T);
};

template <typename... T>
constexpr auto size_of = size_of_t <T...>::value;

template <typename... T>
struct range_values_t
{
    using begin_type = begin_value_t <0, sizeof... (T)>;
    using end_type = end_value_t <0, sizeof... (T)>;
    
    using begin_value_type = typename begin_type::value_type;
    using end_value_type = typename end_type::value_type;
    
    using value_type = common_type_t <begin_value_type, end_value_type>;
    
    inline static constexpr value_type begin_value = begin_type::value;
    inline static constexpr value_type end_value = end_type::value;

};

//template <typename... T>
//struct range_values_t <size_of <T...>>
//{
//
//};

template <auto a, auto b>
struct range_values_t <begin_value_t <a, b>, end_value_t <a, b>>
{
    using begin_value_type = typename begin_value_t <a, b>::value_type;
    using end_value_type = typename end_value_t <a, b>::value_type;
    using begin_type = begin_value_t <a, b>;
    using end_type = end_value_t <a, b>;
    using value_type = common_type_t <begin_value_type, end_value_type>;
    
    inline static constexpr value_type begin_value = begin_value_t <a, b>::value;
    inline static constexpr value_type end_value = end_value_t <a, b>::value;
};

template <auto a, auto b>
constexpr auto range_values = range_values_t <begin_value_t <a, b>, end_value_t <a, b>> {};


template <auto...>
struct advance_value_t;

template <auto t>
requires requires {
    t + 1;
}
struct advance_value_t <t>
{
    using value_type = decltype (t + 1);
    inline static constexpr value_type value = t + 1;
};

template <auto... t>
constexpr auto advance_value = advance_value_t <t...>::value;

template <auto...>
struct retreat_value_t;

template <auto t>
requires requires {
    t - 1;
}
struct retreat_value_t <t>
{
    using value_type = decltype (t - 1);
    inline static constexpr value_type value = t - 1;
};

template <auto... t>
constexpr auto retreat_value = retreat_value_t <t...>::value;

template <typename... T>
struct value_advancer
{
    using value_type = typename range_values_t <T...>::value_type;
    
    template <auto n>
    inline static constexpr value_type value = advance_value_t <n>::value;

};

template <auto a, auto b>
struct value_advancer <range_values_t <begin_value_t <a, b>, end_value_t <a, b>>>
{
    using value_type = typename range_values_t <begin_value_t <a, b>, end_value_t <a, b>>::value_type;
    
    template <auto n>
    inline static constexpr value_type value = advance_value_t <n>::value;
};

template <typename... T>
auto type_watcher (T&&... t)
{
    return [...t = forward <T> (t)] <typename... U> ()
    {
        constexpr auto me = [] <typename A, typename... B> (auto&& me, auto&& fun) constexpr
        {
            if constexpr (requires {fun.template operator () <A> ();})
            {
                if (not fun.template operator () <A> ())
                {
                    if constexpr (sizeof... (B) > 0)
                    {
                        me.template operator () <B...> (move (me), forward <decltype (fun)> (fun));
                    }
                }
            } else if constexpr (sizeof... (B) > 0)
            {
                me.template operator () <B...> (move (me), forward <decltype (fun)> (fun));
            }
        };
        
        (..., me.template operator () <U...> (move (me), t));
    };
}

template <typename T>
concept is_string = requires {
    requires (is_same_v <T, string>);
};

TEST_CASE ("")
{
    auto fun =
    type_watcher ([]<is_string T>(){cout << "0" << endl; return false;}, []<typename T>(){cout << "1" << endl; return false;},
                  []<typename T>(){cout << "2" << endl; return false;}, []<typename T>(){cout << "3" << endl; return false;});
    
    fun.template operator () <int, double, string> ();
}

template <typename... T>
struct types
{
    template <typename... F>
    constexpr types (F&&... f)
    {
        [...f = forward <F> (f)] () -> auto
        {
            auto me = [] <typename C, typename... D, typename F1, typename F2> (F1&& me, F2&& fun)
            {
                if constexpr (requires {fun.template operator () <C> ();})
                {
                    if constexpr (not fun.template operator () <C> ())
                    {
                        if constexpr (sizeof... (D) > 0)
                        {
                            me.template operator () <D...> (forward <F1> (me), forward <F2> (fun));
                        }
                    }
                }
            };
            
            (..., [] <typename F1, typename F2> (F1&& me, F2&& fun)
            {
                if constexpr (requires {me.template operator () <T...> (move (me), forward <F2> (fun));})
                {
                    me.template operator () <T...> (move (me), forward <F2> (fun));
                }
            
            } (move (me), f));
        } ();
    }
};




template <typename...>
struct has_type_t;

template <>
struct has_type_t <>
{
    
};

template <template <typename...> typename T>
struct has_type_t <T <>>
{
    
};

template <typename A>
struct has_type_t <A>
{
    template <typename T>
    inline static constexpr bool value = is_same_v <T, A>;
};

template <template <typename...> typename T, typename A, typename... B>
struct has_type_t <T <A, B...>>
{
    template <typename F>
    inline static constexpr bool value = has_type_t <A, B...>::template value <F>;
};

template <typename A, typename... B>
struct has_type_t <A, B...>
{
private:
    template <typename...> struct types_ {};
    template <typename...>
    struct same_;

    template <typename C, typename... D, typename... E>
    struct same_ <types_ <C, D...>, types_ <E...>>
    {
        template <typename F>
        inline static constexpr bool value = is_same_v <F, C> or same_ <types_ <D...>, types_ <E..., C>>::template value <F>;
    };

    template <typename... D>
    struct same_ <types_ <>, types_ <D...>>
    {
        template <typename F>
        inline static constexpr bool value = false;
    };
    
public:
    template <typename F>
    inline static constexpr bool value = same_ <types_ <A, B...>, types_ <>>::template value <F>;
};

static_assert (has_type_t <int, double, float>::template value <int> == true, "");
static_assert (has_type_t <int, double, float>::template value <double> == true, "");
static_assert (has_type_t <int, double, float>::template value <float> == true, "");
static_assert (has_type_t <int, double, float>::template value <char> == false, "");





template <typename...>
struct begin_type_t;

template <>
struct begin_type_t <>
{
    
};

template <template <typename...> typename T>
struct begin_type_t <T <>>
{
    
};

template <template <typename...> typename T, typename A, typename... B>
struct begin_type_t <T <A, B...>>
{
    using type = A;
    inline static constexpr int value = 0;
};

template <typename A, typename... B>
struct begin_type_t <A, B...>
{
    using type = A;
    inline static constexpr int value = 0;
};

static_assert (is_same_v <bool, typename begin_type_t <tuple <bool, int, string>>::type>, "");
static_assert (is_same_v <bool, typename begin_type_t <bool, int, string>::type>, "");

template <typename... T>
using begin_type = typename begin_type_t <T...>::type;

template <typename...>
struct end_type_t;

template <>
struct end_type_t <>
{
    
};

template <template <typename...> typename T>
struct end_type_t <T <>>
{
    
};

template <typename A, typename... B>
struct end_type_t <A, B...>
{
    using type = typename end_type_t <B...>::type;
    inline static constexpr int value = end_type_t <B...>::value + 1;
};

template <template <typename...> typename T, typename A, typename... B>
struct end_type_t <T <A, B...>>
{
    using type = typename end_type_t <T <B...>>::type;
    inline static constexpr int value = end_type_t <T <B...>>::value + 1;
};

template <typename A>
struct end_type_t <A>
{
    using type = A;
    inline static constexpr int value = 0;
};

template <template <typename...> typename T, typename A>
struct end_type_t <T <A>>
{
    using type = A;
    inline static constexpr int value = 0;
};

static_assert (is_same_v <string, typename end_type_t <tuple <bool, int, string>>::type>, "");
static_assert (is_same_v <char, typename end_type_t <bool, int, char>::type>, "");
static_assert (end_type_t <int, char, double>::value == 2, "");
static_assert (end_type_t <tuple <int, char, double>>::value == 2, "");

//static_assert (is_same_v <string, typename end_type <bool, int, string>::type>, "");

template <typename... T>
using end_type = typename end_type_t <T...>::type;




template <typename...>
struct pop;

template <typename A, typename... B>
struct pop <A, B...>
{
    template <template <typename...> typename T>
    using front = typename pop <T <A, B...>>::front;
    
    template <template <typename...> typename T>
    using back = typename pop <T <A, B...>>::back;
};

template <>
struct pop <> {};

template <template <typename...> typename T>
struct pop <T <>> {};

template <template <typename...> typename T, typename A, typename... B>
struct pop <T <A, B...>>
{
private:
    template <typename...>
    struct back_;
    
    template <typename C, typename... D, typename... E>
    struct back_ <T <C, D...>, T <E...>>
    {
        using type = typename back_ <T <D...>, T <E..., C>>::type;
    };

    template <typename C, typename... D>
    struct back_ <T <C>, T <D...>>
    {
        using type = T <D...>;
    };
    
public:
    using front = T <B...>;
    using back = typename back_ <T <B...>, T <A>>::type;
};

//template <typename A, typename... B>
//struct pop
//{
//
//    using back = typename pop <types <A, B...>>
//};

template <template <typename...> typename T, typename... U>
struct pop_transform_to
{
    using type = pop <typename T <U...>::transform_from>;
};


static_assert (is_same_v <types <int, double>, typename pop <types <int, double, string>>::back>, "");
static_assert (is_same_v <types <int, double>, typename pop <int, double, string>::back <types>>, "");

static_assert (is_same_v <types <double, string>, typename pop <types <int, double, string>>::front>, "");
static_assert (is_same_v <types <double, string>, typename pop <int, double, string>::front <types>>, "");

template <typename... A>
using pop_back = typename pop <A...>::back;

template <typename... A>
using pop_front = typename pop <A...>::front;


template <typename...>
struct push;

template <template <typename...> typename T, typename... A>
struct push <T <A...>>
{
    template <typename N>
    using back = T <A..., N>;
    
    template <typename N>
    using front = T <N, A...>;
};

template <typename...>
struct back_t;

template <typename A, typename... B>
struct back_t <A, B...>
{
    using type = typename back_t <B...>::type;
};

template <template <typename...> typename T, typename A, typename... B>
struct back_t <T <A, B...>>
{
    using type = typename back_t <T <B...>>::type;
};

template <typename A>
struct back_t <A>
{
    using type = A;
};

template <template <typename...> typename T, typename A>
struct back_t <T <A>>
{
    using type = A;
};

template <>
struct back_t <>
{

};

template <template <typename...> typename T>
struct back_t <T <>>
{

};

static_assert (is_same_v <int, typename back_t <string, char, int>::type>, "");
static_assert (is_same_v <int, typename back_t <tuple <string, char, int>>::type>, "");

template <typename... T>
using back = typename back_t <T...>::type;

template <typename...>
struct front_t;

template <typename A, typename... B>
struct front_t <A, B...>
{
    using type = A;
};

template <template <typename...> typename T, typename A, typename... B>
struct front_t <T <A, B...>>
{
    using type = A;
};

template <typename A>
struct front_t <A>
{
    using type = A;
};

template <template <typename...> typename T, typename A>
struct front_t <T <A>>
{
    using type = A;
};

template <>
struct front_t <>
{

};

template <template <typename...> typename T>
struct front_t <T <>>
{

};

static_assert (is_same_v <double, typename front_t <double, char, int>::type>, "");
static_assert (is_same_v <double, typename front_t <tuple <double, char, int>>::type>, "");

template <typename... T>
using front = typename front_t <T...>::type;


template <typename...>
struct advance_type;

template <template <typename...> typename T, typename... A>
struct advance_type <T <A...>>
{
    template <typename N>
    using type = T <A..., N>;
};




template <typename... T>
struct retreat_type;

template <>
struct retreat_type <> {};

template <template <typename...> typename T>
struct retreat_type <T <>> {};

template <typename A, typename... B>
struct retreat_type <A, B...>
{
    template <template <typename...> typename T>
    using type = pop_back <T <A, B...>>;
};

template <template <typename...> typename T, typename A, typename... B>
struct retreat_type <T <A, B...>>
{
    using type = pop_back <T <A, B...>>;
};

template <typename... T>
struct type_transformer_t;

template <typename...>
struct range_types_t;

template <typename A, typename... B>
struct range_types_t <begin_type_t <A, B...>, end_type_t <A, B...>>
{
    using begin_t = begin_type <A, B...>;
    using end_t = end_type <A, B...>;
    
//    template <typename N>
//    using expand_range = next_transform <range_types_t <A, B..., N>>;

    template <typename... N>
    using transform = type_transformer_t <N...>;
    
    template <typename...>
    struct transform_from;
    
//    template <template <typename...> typename T, typename... U>
//    struct transform_from <T <U...>>
//    {
//        using type = next_transform <range_types_t <next_transform, U...>>;
//    };
    
    
//    using pop_last =
    
};


template <typename...>
struct type_advancer;

template <typename... T>
struct type_advancer <range_types_t <begin_type_t <T...>, end_type_t <T...>>>
{
    template <typename N>
    using type = typename range_types_t <begin_type_t <T...>, end_type_t <T...>>::template transform <T..., N>;
//    using type = typename push <>::template back <N>;
};

template <typename... T>
struct type_retreater;

template <typename... T>
struct type_retreater <range_types_t <begin_type_t <T...>, end_type_t <T...>>>
{
    using _range = range_types_t <begin_type_t <T...>, end_type_t <T...>>;
//    template <typename N>
    using type = typename pop <_range>::back;//pop_transform_to <_range>;
};


template <typename... T>
struct finish_type_t;

template <typename T, typename... U>
struct finish_type_t <T, U...>
{
    using type = end_type <T, U...>;
};



template <typename...>
struct empty_type_t;

template <typename T, typename... U>
struct empty_type_t <T, U...>
{
    inline static constexpr bool value = false;
};

template <template <typename...> typename T, typename A, typename... B>
struct empty_type_t <T <A, B...>>
{
    inline static constexpr bool value = false;
};

template <>
struct empty_type_t <>
{
    inline static constexpr bool value = true;
};

template <template <typename...> typename T>
struct empty_type_t <T <>>
{
    inline static constexpr bool value = true;
};

template <typename... T>
constexpr auto empty_type = empty_type_t <T...>::value;






template <typename F>
concept works = requires (F&& fun){
    {fun.template operator () <C> ()};
};

template <typename A, typename... B>
struct type_transformer_t <range_types_t <begin_type_t <A, B...>, end_type_t <A, B...>>>
{
    using range_types_ = range_types_t <begin_type_t <A, B...>, end_type_t <A, B...>>;
    using finished_type_ = typename range_types_::end_t;
    using type_advancer_ = type_advancer <range_types_>;
    
    template <typename T>
    using advance = typename type_advancer <range_types_>::template type <T>;
    
//    template <typename T>
    using retreat = typename type_retreater <range_types_>::type;
    
    
    template <typename... Funcs>
    constexpr type_transformer_t (Funcs&&... fun)
    {
        [...funs = forward <Funcs> (fun)] ()
        {
            auto me = [] <typename C, typename... D, typename F1, typename F2> (F1&& me, F2&& fun)
            {
                if constexpr (requires {
                    {fun.template operator () <C> ()};
                })
                {
                    if (not fun.template operator () <C> ())
                    {
                        if constexpr (sizeof... (D) > 0)
                        {
                            me.template operator () <D...> (forward <F1> (me), forward <F2> (fun));
                        }
                    }
                }
            };
            
            (..., [] <typename F1, typename F2> (F1&& me, F2&& fun)
            {
                if constexpr (requires {
                    {me.template operator () <A, B...> (forward <F1> (me), forward <F2> (fun))};
                })
                {
                    me.template operator () <A, B...> (forward <F1> (me), forward <F2> (fun));
                }
            
            } (move (me), funs));
        } ();
    }
};

template <typename... T>
using make_type_transformer = type_transformer_t <range_types_t <begin_type_t <T...>, end_type_t <T...>>>;



TEST_CASE ("")
{
    using transformer = make_type_transformer <int, double, string, char>;
    transformer t =
    {
        
        [] <is_string T> () constexpr {
            cout << "hi" << endl;
            if constexpr (is_same_v <T, int>) {
                cout << "int" << endl;
            } else if constexpr (is_same_v <T, double>)
            {
                cout << "double" << endl;
            }
            return true;
        },
        
        [] <typename T> () constexpr {
            cout << "hi" << endl;
            if constexpr (is_same_v <T, int>) {
                cout << "int" << endl;
            } else if constexpr (is_same_v <T, double>)
            {
                cout << "double" << endl;
            }
            return true;
        }
        
    };
    
    
}

template <typename...>
struct seq_t;

template <typename A, typename... B>
struct seq_t <A, B...>
{
    
};

template <typename start, typename end>
consteval auto make_seq_t () -> auto
{
    using transformer = make_type_transformer <start, end>;//make_value_transformer <start, end>;

    constexpr auto fun = [] <typename currEnd, typename... I> (auto fun) consteval -> auto
    {
        if constexpr (transformer::template done_value <currEnd>)
        {
            return seq_t <I...> {};
        }
        else
        {
            return fun.template operator () <transformer::template advance <currEnd>, I..., currEnd> (fun);
        }
    };
    
    return fun.template operator() <start> (fun);
}

template <typename T, typename... U>
struct type_advancer <T, U...>
{
    
};


template <typename T, typename... U>
struct type_retreater <T, U...>
{
    
};

template <typename...>
struct value_retreater;

template <auto a, auto b>
struct value_retreater <range_values_t <begin_value_t <a, b>, end_value_t <a, b>>>
{
    using value_type = typename range_values_t <begin_value_t <a, b>, end_value_t <a, b>>::value_type;
    
    template <auto n>
    inline static constexpr value_type value = advance_value_t <n>::value;
};

template <typename... T>
struct value_transformer_t
{
    
    using range_type = range_values_t <begin_value_t <0, sizeof... (T)>, end_value_t <0, sizeof... (T)>>;
    using finished_type = finished_value_t <range_type::begin_value, range_type::end_value>;

};

template <auto start_, auto end_>
struct value_transformer_t <
    range_values_t <begin_value_t <start_, end_>, end_value_t <start_, end_>>,
    finished_value_t <range_values_t <begin_value_t <start_, end_>, end_value_t <start_, end_>> ::end_value>,
    value_advancer <range_values_t <begin_value_t <start_, end_>, end_value_t <start_, end_>>>,
    value_retreater <range_values_t <begin_value_t <start_, end_>, end_value_t <start_, end_>>>
>
{
    using range_type = range_values_t <begin_value_t <start_, end_>, end_value_t <start_, end_>>;
    using finished_type = finished_value_t <range_type::end_value>;
    using begin_type = typename range_type::begin_type;
    using advancer_type = value_advancer <range_type>;
    using retreater_type = value_retreater <range_type>;
    
    inline static constexpr typename range_type::begin_value_type begin_value = range_type::begin_value;
    
    template <auto n>
    inline static constexpr typename value_advancer <range_type>::value_type advance = value_advancer <range_type>::template value <n>;
    
    template <auto n>
    inline static constexpr typename value_retreater <range_type>::value_type retreat = value_retreater <range_type>::template value <n>;

    template <auto n>
    inline static constexpr bool done_value = finished_type::template value <n>;
};

template <typename... T>
using value_transformer = value_transformer_t <T...>;

template <auto start_, auto end_>
using make_value_transformer = value_transformer_t <
     range_values_t <begin_value_t <start_, end_>, end_value_t <start_, end_>>,
    finished_value_t <range_values_t  <begin_value_t <start_, end_>, end_value_t <start_, end_>>::end_value>,
    value_advancer <range_values_t <begin_value_t <start_, end_>, end_value_t <start_, end_>>>,
    value_retreater <range_values_t <begin_value_t <start_, end_>, end_value_t <start_, end_>>>
>;





template <auto t, auto v>
constexpr auto transform_next_value = t.next_value <v>;


//template <auto t>
//struct next_value_t <t>
//{
//    inline static constexpr decltype (t) value = transform_values <t>.next_value <t>;
//};




template <auto...>
struct seq_v;

template <auto i, auto... j>
struct seq_v <i, j...>
{
    constexpr auto operator () (auto&& fun) const -> void//-> auto&
    {
        if (fun (i))
        {
//            return *this;
        } else if constexpr (sizeof... (j) > 0)
        {
            return seq_v <j...> {} (forward <decltype (fun)> (fun));
        } else
        {
//            return *this;
        }
    }
};










template <typename...>
struct seq_size_t;

template <auto... J>
struct seq_size_t <seq_v <J...>>
{
    inline static constexpr size_t value = sizeof... (J);
};





template <auto start, decltype (start) end>
consteval auto make_seq () -> auto
{
    using transformer = make_value_transformer <start, end>;

    constexpr auto fun = [] <auto currEnd, auto... I> (auto fun) consteval -> auto
    {
        if constexpr (transformer::template done_value <currEnd>)
        {
            return seq_v <I...> {};
        }
        else
        {
            return fun.template operator () <transformer::template advance <currEnd>, I..., currEnd> (fun);
        }
    };
    
    return fun.template operator() <start> (fun);
}



template <auto max>
constexpr seq_v seq = make_seq <zero_value <decltype (max)>, max> ();


template <typename... T>
struct Te
{
    auto te ()
    {
        
    }
};

template <typename... T>
struct seq_t
{
    decltype (make_seq <0, sizeof... (T)>) seq_value = make_seq <0, sizeof... (T)>;
    
};

TEST_CASE ("")
{
    auto ad = make_value_transformer <0, 10> {};
    
    constexpr auto aa = make_seq <0, 10> ();
    
    aa ([](auto const& i) constexpr {/*cout << i << endl;*/if (i == 3)return true; return false;});
    
    return;
    
    [] <auto... I> (seq_v <I...> aa) {
        
    } (aa);
    cout << endl;
    
}


TEST_CASE ("")
{
    return;
    [] <typename... T> (tuple <T...> t)
    {
        auto fun = [](auto& g) {
            cout << g << endl;
        };
        
        [&t] <auto... I> (seq_v <I...>, auto f)
        {
            ((f (get <I> (t))), ...);
            (cout << ... << get <I> (t));
        } (seq <sizeof... (T)>, fun);
    } (tuple {1, 2, 3, 4, 5});
    
    [] <typename... T> (tuple <T...> t)
    {
        auto fun = [](auto& a) {
            cout << a << endl;
        };
        [&t] <auto... I> (seq_v <I...>, auto f)
        {
            ((f (get <I> (t))), ...);
            (cout << ... << get <I> (t));
        } (seq <sizeof... (T)>, fun);
    } (tuple {1, 2, 3});
    
    cout << is_same_v <type_at <0, int, double, string>, int> << endl;
    cout << is_same_v <type_at <1, int, double, string>, int> << endl;

//    cout << sizeof (big <0>) << endl;
//    cout << sizeof (u <big <0>, big <1>, big <2>>) << endl;
    u <E <0>, E <1>> u0;
    u <F <1>, E <0>> u1;
//    cout << is_trivially_default_constructible_v <H> << endl;
    u1.tail_.value_ = E <0> {};
    v <F <1>, E <0>> v0 {2};
}

