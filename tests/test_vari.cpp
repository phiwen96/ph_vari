#include <catch2/catch.hpp>
#include <ph_vari/vari.hpp>
#include <ph_time/timer.hpp>
#include <ph_type_list/type_list.hpp>
using namespace std;

struct A
{
    A (A&&){cout << "A (A&&)" << endl;}
    A (){cout << "A ()" << endl;}
    ~A (){cout << "~A ()" << endl;}

    A& operator= (A other)
    {
        return *this;
    }
};

struct B
{
    B (B&&){cout << "B (B&&)" << endl;}
    B (){cout << "B ()" << endl;}
    ~B (){cout << "~B ()" << endl;}
};

struct C
{
    C (C&&){cout << "C (C&&)" << endl;}
    C (){cout << "C ()" << endl;}
    ~C (){cout << "~C ()" << endl;}
};

struct D
{
    D (D&&){cout << "D (D&&)" << endl;}
    D (){cout << "D ()" << endl;}
    ~D (){cout << "~D ()" << endl;}
};



TEST_CASE("is not class")
{

//    cout << sizeof (variant <int, double, char, string>) << endl;
//    cout << sizeof (var <int, double, char, string>) << endl;
//    int max = 10000000;
    int max = 0;

    {
        Timer <true> t ("variant");
        for (int i = 0; i < max; ++i)
        {
            variant <int, double, char, string> k {string {}};
        }
    }
    {
        Timer <true> t ("vari");
        for (int i = 0; i < max; ++i)
        {
            var <int, double, char, string> k {string {}};
        }
    }
    
    
//    var <A, int, C, D> k {C {}};
    var <A, int, C, D> k {int {4}};
    cout << k << endl;
    var <A, C, B, int> k2 {int{7}};
    k = k2;
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


