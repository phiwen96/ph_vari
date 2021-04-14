#include <catch2/catch.hpp>
#include <ph_vari/vari.hpp>
#include <ph_time/timer.hpp>
using namespace std;

struct A
{
    A (){cout << "A ()" << endl;}
    ~A (){cout << "~A ()" << endl;}

};

struct B
{
    B (){cout << "B ()" << endl;}
    ~B (){cout << "~B ()" << endl;}
};

struct C
{
    C (){cout << "C ()" << endl;}
    ~C (){cout << "~C ()" << endl;}
};



TEST_CASE("is not class")
{
 
}
