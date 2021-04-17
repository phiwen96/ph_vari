#include <catch2/catch.hpp>
#include <ph_vari/vari.hpp>
#include <ph_time/timer.hpp>
#include <ph_type_list/type_list.hpp>
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


template <auto... J>
struct seq_t
{
    
//    inline static constexpr int value = I;
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




template <auto...>
struct range_values_t;

template <auto a, auto b>
struct range_values_t <a, b>
{
    using begin_value_type = typename begin_value_t <a, b>::value_type;
    using end_value_type = typename end_value_t <a, b>::value_type;
    using begin_type = begin_value_t <a, b>;
    using end_type = end_value_t <a, b>;
    using value_type = common_type_t <begin_value_type, end_value_type>;
    
    inline static constexpr value_type begin_value = begin_value_t <a, b>::value;
    inline static constexpr value_type end_value = end_value_t <a, b>::value;
};

template <auto... t>
constexpr auto range_values = range_values_t <t...> {};


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

template <typename... Range_values>
struct value_advancer;

template <auto a, auto b>
struct value_advancer <range_values_t <a, b>>
{
    using value_type = typename range_values_t <a, b>::value_type;
    
    template <auto n>
    inline static constexpr value_type value = advance_value_t <n>::value;
};





template <typename...>
struct value_retreater;

template <auto a, auto b>
struct value_retreater <range_values_t <a, b>>
{
    using value_type = typename range_values_t <a, b>::value_type;
    
    template <auto n>
    inline static constexpr value_type value = advance_value_t <n>::value;
};



template <typename...>
struct value_transformer_t;

template <auto start, auto end>
requires requires {
    start < end;
    requires start < end;
}
struct value_transformer_t <range_values_t <start, end>>
{
//    using value_type = common_type_t <typename range_values_t <start, end>::begin_value_type, typename range_values_t <start, end>::end_value_type>;
    using value_type = int;
    inline static constexpr value_type begin = start;
    
    template <auto n>
    inline static constexpr value_type advance = next_value <n>;
    
    template <value_type n>
    inline static constexpr value_type retreat = previous_value <n>;
    
    template <value_type n>
    inline static constexpr bool done = finished_value_t <range_values_t <start, end>::end>::template value <n>;
};

//template <auto start, auto end>
//requires requires {
//    start > end;
//    requires start > end;
//}
//struct value_transformer_t <range_values_t <start, end>>
//{
//    using range_type = range_values_t <start, end>;
//    using value_type = int;
//
//    inline static constexpr value_type begin = range_type::begin;
//
//    template <value_type n>
//    inline static constexpr value_type advance = previous_value <n>;
//
//    template <value_type n>
//    inline static constexpr value_type retreat = next_value <n>;
//
//    template <value_type n>
//    inline static constexpr bool done = finished_value_t <range_type::end>::template value <n>;
//};



template <auto start_, auto end_>
struct value_transformer_t <
    range_values_t <start_, end_>,
    finished_value_t <range_values_t <start_, end_>::end_value>,
    value_advancer <range_values_t <start_, end_>>,
    value_retreater <range_values_t <start_, end_>>
>
{
    using range_type = range_values_t <start_, end_>;
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
    range_values_t <start_, end_>,
    finished_value_t <range_values_t <start_, end_>::end_value>,
    value_advancer <range_values_t <start_, end_>>,
    value_retreater <range_values_t <start_, end_>>
>;



template <auto t, auto v>
constexpr auto transform_next_value = t.next_value <v>;


//template <auto t>
//struct next_value_t <t>
//{
//    inline static constexpr decltype (t) value = transform_values <t>.next_value <t>;
//};






template <auto start, decltype (start) end>
consteval auto make_seq () -> auto
{
    using transformer = make_value_transformer <start, end>;

    constexpr auto fun = [] <auto currEnd, auto... I> (auto fun) consteval -> auto
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

template <auto max>
constexpr seq_t seq = make_seq <zero_value <decltype (max)>, max> ();





TEST_CASE ("")
{
    [] <typename... T> (tuple <T...> t)
    {
        auto fun = [](auto& g) {
            cout << g << endl;
        };
        
        [&t] <auto... I> (seq_t <I...>, auto f)
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
        [&t] <auto... I> (seq_t <I...>, auto f)
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

