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
    var <A, B, C, D> k {A {}};
    cout << k << endl;
    cout << "====" << endl;
    k = B{};
    cout << "====" << endl;
    /**
     NOT
     k = string {};
     */
}
