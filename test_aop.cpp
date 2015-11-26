#include "aop.h"
#include "timer.h"

#include <chrono>
#include <functional>
#include <iostream>
using namespace std;

struct AA
{
    void Before(int i)
    {
        cout << "Before from AA " << i << endl;
    }

    void After(int i)
    {
        cout << "After from AA " << i << endl;
    }
};

struct BB
{
    void Before(int i)
    {
        cout << "Before from BB " << i << endl;
    }

    void After(int i)
    {
        cout << "After from BB " << i << endl;
    }
};

struct CC
{
    void Before()
    {
        cout << "Before from CC " << endl;
    }

    void After()
    {
        cout << "After from CC " << endl;
    }
};

struct DD
{
    void Before()
    {
        cout << "Before from DD " << endl;
    }

    void After()
    {
        cout << "After from DD " << endl;
    }
};

void GT()
{
    cout << "real GT function " << endl;
}

void HT(int a)
{
    cout << "real HT function: " << a << endl;
}

void test_aop()
{
    std::function<void(int)> f = std::bind(&HT, std::placeholders::_1);
    invoke<AA,BB>(std::function<void(int)>(std::bind(&HT, std::placeholders::_1)), -1);

    invoke<AA,BB>(f,1);

    invoke<CC,DD>(&GT);
    invoke<AA,BB>(&HT,1);

    invoke<AA,BB>([](int i) {}, 1);

    invoke<CC,DD>([]{});
}


struct Time_Elapsed_Aspect
{
    void Before(int i)
    {
        m_last_time = m_t.elapsed();
    }
    void After(int i)
    {
        cout << "time elapsed: " << m_t.elapsed() - m_last_time << endl;
    }

private:
    double m_last_time;
    Timer m_t;
};

struct Logging_Aspect
{
    void Before(int i)
    {
        cout << "entering " << endl;
    }
    void After(int i)
    {
        std::cout << "leaving " << endl;
    }
};

void foo(int a)
{
    cout << "real foo function: " << a << endl;
}

int main()
{
    test_aop();

    invoke<Logging_Aspect, Time_Elapsed_Aspect>(&foo, 1);

    cout << "----------------------" << endl;

    invoke<Time_Elapsed_Aspect, Logging_Aspect>(&foo, 1);
    return 0;
}
