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





TEST_CASE ("Constructs and assignments")
{
    REQUIRE (is_move_constructible_v <var <int, double, string>>);
    REQUIRE (is_copy_constructible_v <var <int, double, string>>);
    REQUIRE (is_move_assignable_v <var <int, double, string>>);
    REQUIRE (is_copy_assignable_v <var <int, double, string>>);
    
    GIVEN ("User defined classes A B C D")
    {
        AND_WHEN ("All is copy constructible")
        {
            REQUIRE (is_copy_constructible_v <A>);
            REQUIRE (is_copy_constructible_v <C>);
            REQUIRE (is_copy_constructible_v <D>);
            
            THEN ("Var should also be copy constructible")
            {
                REQUIRE (is_copy_constructible_v <var <A, C, D>>);
                
                var <A, C, D> v0;
                var <A, C, D> v1 {v0};
            }
        }
        
        AND_WHEN ("All but one is copy constructible")
        {
            REQUIRE (is_copy_constructible_v <A>);
            REQUIRE (not is_copy_constructible_v <B>);
            REQUIRE (is_copy_constructible_v <C>);
            
            THEN ("Var should not be copy constructible")
            {
                REQUIRE (not is_copy_constructible_v <var <A, B, C>>);
            }
        }
        
        AND_WHEN ("All is move constructible")
        {
            REQUIRE (is_move_constructible_v <A>);
            REQUIRE (is_move_constructible_v <B>);
            REQUIRE (is_move_constructible_v <D>);
            
            THEN ("Var should also be move constructible")
            {
                REQUIRE (is_move_constructible_v <var <A, B, D>>);
                
                REQUIRE (not is_copy_constructible_v <var <A, B, D>>);
                var <A, B, D> v0;
                var <A, B, D> v1 {move (v0)};
            }
        }
        
        AND_WHEN ("All but one is move constructible")
        {
            REQUIRE (is_move_constructible_v <A>);
            REQUIRE (is_move_constructible_v <B>);
            REQUIRE (not is_move_constructible_v <C>);
            
            THEN ("Var should not be move constructible")
            {
                REQUIRE (not is_move_constructible_v <var <A, B, C>>);
            }
        }
        
        AND_WHEN ("All is move assignable")
        {
            REQUIRE (is_move_assignable_v <B>);
            REQUIRE (is_move_assignable_v <C>);
            REQUIRE (is_move_assignable_v <D>);
            
            THEN ("Var should also be move constructible")
            {
                REQUIRE (is_move_assignable_v <var <B, C, D>>);
                
                
                REQUIRE (not is_copy_assignable_v <var <B, C, D>>);
                var <B, C, D> v0;
                var <B, C, D> v1;
                v0 = move (v1);
            }
        }
        
        AND_WHEN ("All but one is move assignable")
        {
            REQUIRE (not is_move_assignable_v <A>);
            REQUIRE (is_move_assignable_v <B>);
            REQUIRE (is_move_assignable_v <C>);
            
            THEN ("Var should not be move assignable")
            {
                REQUIRE (not is_move_assignable_v <var <A, B, C>>);
            }
        }
        
        
        AND_WHEN ("All is copy assignable")
        {
            REQUIRE (is_copy_assignable_v <A>);
            REQUIRE (is_copy_assignable_v <C>);
            REQUIRE (is_copy_assignable_v <D>);
            
            THEN ("Var should also be copy constructible")
            {
                REQUIRE (is_copy_assignable_v <var <A, C, D>>);
                

                var <A, C, D> v0;
                var <A, C, D> v1;
                v0 = v1;
            }
        }
        
        AND_WHEN ("All but one is copy assignable")
        {
            REQUIRE (is_copy_assignable_v <A>);
            REQUIRE (not is_copy_assignable_v <B>);
            REQUIRE (is_copy_assignable_v <C>);
            
            THEN ("Var should not be copy assignable")
            {
                REQUIRE (not is_copy_assignable_v <var <A, B, C>>);
            }
        }
    }
}


TEST_CASE ("value_type constructs")
{
    WHEN ("basics")
    {
        {
            auto v0 = var <int, double, string> {};
            REQUIRE (v0.active () == -1);
        }
        {
            auto v0 = var <int, double, string> {int {2}};
            REQUIRE (v0.active () == 0);
            int& i = v0;
            REQUIRE (i == 2);
            i = 3;
            REQUIRE ((int&) v0 == 3);
            (int&) v0 = 2;
            REQUIRE (i == 2);
        }
        {
            auto v0 = var <int, double, string> {double {2}};
            REQUIRE (v0.active () == 1);
            double& i = v0;
            REQUIRE (i == 2);
            i = 3;
            REQUIRE ((double&) v0 == 3);
            (double&) v0 = 2;
            REQUIRE (i == 2);
        }
        {
            auto v0 = var <int, double, string> {string {"hej"}};
            REQUIRE (v0.active () == 2);
            string& i = v0;
            REQUIRE (i == "hej");
            i = "då";
            REQUIRE ((string&) v0 == "då");
            (string&) v0 = "hej";
            REQUIRE (i == "hej");
        }
    }
    
    WHEN ("default constructor")
    {
        auto v0 = var <int, double, string> {};
        
        THEN ("v0.active () == -1")
        {
            REQUIRE (v0.active () == -1);
        }
    }
    
    WHEN ("copy constructor")
    {
        
        REQUIRE (not is_move_constructible_v <C>);
        
        {
            auto v0 = var <C, double, string> {C {}};
            REQUIRE (v0.active () == 0);
        }
        {
            auto v0 = var <double, C, string> {C {}};
            REQUIRE (v0.active () == 1);
        }
        {
            auto v0 = var <double, string, C> {C {}};
            REQUIRE (v0.active () == 2);
        }
    }
    
    WHEN ("move constructor")
    {
        
        REQUIRE (not is_copy_constructible_v <B>);
        
        {
            auto v0 = var <B, double, string> {B {}};
            REQUIRE (v0.active () == 0);
        }
        {
            auto v0 = var <double, B, string> {B {}};
            REQUIRE (v0.active () == 1);
        }
        {
            auto v0 = var <double, string, B> {B {}};
            REQUIRE (v0.active () == 2);
        }
    }
    
    WHEN ("copy assign")
    {

        REQUIRE (not is_copy_constructible_v <B>);

        {
            auto v0 = var <B, double, string> {B {}};
            REQUIRE (v0.active () == 0);
        }
        {
            auto v0 = var <double, B, string> {B {}};
            REQUIRE (v0.active () == 1);
        }
        {
            auto v0 = var <double, string, B> {B {}};
            REQUIRE (v0.active () == 2);
        }
    }
}

TEST_CASE (".get <T> ()")
{
    GIVEN ("var")
    {
//        var <int, double, char, string> v0;
        
    }
}

TEST_CASE ("")
{
    var <A, int, C, D> v0;
//    var <A, int, C, D> v0 {A {}};
//    var <A, int, C, D> v0 {int {4}};
//    var <A, int, C, D> v1 {A {}};
//    var <A, int, C, D> v2 {C {}};
//    var <A, int, C, D> v3 {D {}};
}

TEST_CASE ("test vari with vector")
{
//    vector <var <A, B, C, D>> v;
//    v.push_back (A {});
//    REQUIRE (v.back().active() == 0);
}

TEST_CASE ("")
{
//    var <int, double, string> v0;
//    REQUIRE (v0.active() == -1);
////
//    var <int, double, string> v1 {int {0}};
//    REQUIRE (v1.active() == 0);
    
    
}

TEST_CASE ("")
{
//    var <D, int, C, A> v0 {C {}};
//
//    REQUIRE (type <int> == v0);
//    REQUIRE (not (type <C> == v0));
//
//    REQUIRE (type <D> != v0);
//    REQUIRE (not (type <int> != v0));
}


TEST_CASE ("call actives (if active) destructor")
{
//    return;
//    A::alive = 0;
//
//    {
//        var <D, int, C, A> v0 {A {}};
//    }
//
//    REQUIRE (A::alive == 0);
//
//    {
//        var <D, int, C, A> v0 {A {}};
//        REQUIRE (A::alive == 1);
//        v0 = A {};
//        REQUIRE (A::alive == 1);
//        v0 = D {};
//        REQUIRE (A::alive == 0);
//    }
}

TEST_CASE ("")
{
//    return;
//    var <D, int, C, A> v0 {int {3}};
//    REQUIRE (v0.get <int> () == 3);
}

TEST_CASE ("call moved vars active destructor")
{
//    return;
//    A::alive = 0;
//
//    {
//        var <D, int, C, A> v0 {var <int, double, string> {int {2}}};
//        cout << v0.get<int>() << endl;
//    }
//
//    REQUIRE (A::alive == 0);
}


TEST_CASE ("1")
{
    
//    return;
//    var <D, int, C, A> v0 {int {4}};
//    assert (v0.get <int> () == 4);
//    v0.get <int> () = 3;
//    assert (v0.get <int> () == 3);
//    int i0 = v0;
//    assert (i0 == 3);
//    int& i1 = v0;
//    ++i1;
//    assert (v0.get<int>() == 4);
//    v0.get<int>() = 3;
//    assert(i1 == 3);
}



TEST_CASE("vari vs std::variant")
{
    return;
    
   /**/

//    cout << sizeof (variant <int, double, char, string>) << endl;
//    cout << sizeof (var <int, double, char, string>) << endl;
//    int max = 10000000;
////    int max = 0;
//
//    {
//        Timer <true> t ("variant");
//        for (int i = 0; i < max; ++i)
//        {
//            variant <int, double, char, string> k {string {}};
//        }
//    }
//    {
//        Timer <true> t ("vari");
//        for (int i = 0; i < max; ++i)
//        {
//            var <int, double, char, string> k {string {}};
//        }
//    }
//
//    {
//        variant <int, double, char, string> k {string {}};
//
//        Timer <true> t ("variant move assign");
//
//        for (int i = 0; i < max; ++i)
//        {
//            k = double {3};
//        }
//    }
//    {
//        var <int, double, char, string> k {string {}};
//
//        Timer <true> t ("vari move assign");
//        for (int i = 0; i < max; ++i)
//        {
//            k = double {3};
//        }
//    }
//
//    {
//        variant <int, double, char, string> k {string {}};
//
//        Timer <true> t ("variant if type");
//
//        for (int i = 0; i < max; ++i)
//        {
//            if (k.index() == 3)
//            {
//
//            }
//        }
//    }
//    {
//        var <int, double, char, string> k {string {}};
//
//        Timer <true> t ("vari if type");
//        for (int i = 0; i < max; ++i)
//        {
//            if (type <string> == k)
//            {
//
//            }
//        }
//    }
//
//
////    var <A, int, C, D> k {C {}};
//    var <A, int, C, D> k {int {4}};
//    cout << k << endl;
//    var <A, C, B, int> k2 {int{7}};
//    k = k2;
    return;
    
//    cout << k << endl;
//    cout << "====" << endl;
//    k = A{};
//    cout << k << endl;
//
//    cout << "====" << endl;
////    k = B {};
//    cout << k << endl;

    /**
     NOT
     k = string {};
     */
}


