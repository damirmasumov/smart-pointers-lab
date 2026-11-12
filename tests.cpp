#include "tests.h"
#include <iostream>
#include <chrono>
#include <memory>
#include "UnqPtr.h"
#include "ShrdPtr.h"
#include "DynamicArray.h"

class Base {
public:
    int value;
    Base(int v = 0) : value(v) {}
    virtual ~Base() = default;
};

class Derived : public Base {
public:
    Derived(int v) : Base(v) {}
};

int tests_passed = 0;
int tests_total = 0;

void Check(bool condition) {
    tests_total++;
    if (condition) {
        tests_passed++;
    } else {
        std::cout << "Failed test #" << tests_total << "\n";
    }
}

void RunFunctionalTests() {
    tests_passed = 0;
    tests_total = 0;

    UnqPtr<int> u1(new int(10));
    Check(u1 && *u1 == 10);

    UnqPtr<int> u2 = static_cast<UnqPtr<int>&&>(u1);
    Check(!u1);
    Check(u2 && *u2 == 10);

    UnqPtr<int> u3(new int(20));
    ShrdPtr<int> s1(static_cast<UnqPtr<int>&&>(u3));
    Check(s1.UseCount() == 1);

    {
        ShrdPtr<int> s2 = s1;
        Check(s1.UseCount() == 2);
        Check(s2.UseCount() == 2);
    }
    Check(s1.UseCount() == 1);

    UnqPtr<Derived> derived_unq(new Derived(55));
    UnqPtr<Base> base_unq = static_cast<UnqPtr<Derived>&&>(derived_unq);
    Check(base_unq->value == 55);

    UnqPtr<Derived> derived_unq2(new Derived(77));
    ShrdPtr<Derived> derived_shrd(static_cast<UnqPtr<Derived>&&>(derived_unq2));
    ShrdPtr<Base> base_shrd = derived_shrd;
    Check(base_shrd->value == 77);
    Check(base_shrd.UseCount() == 2);

    DynamicArray<ShrdPtr<int>> arr;
    UnqPtr<int> u4(new int(100));
    ShrdPtr<int> s_arr(static_cast<UnqPtr<int>&&>(u4));
    
    arr.PushBack(s_arr);
    Check(s_arr.UseCount() == 2);
    Check(*arr[0] == 100);

    std::cout << "Functional tests: " << tests_passed << " / " << tests_total << " passed\n";
}

LoadTestResult RunSingleLoadTest(size_t elements_count) {
    LoadTestResult result;
    result.elements_count = elements_count;

    auto start = std::chrono::high_resolution_clock::now();
    int** raw_arr = new int*[elements_count];
    for (size_t i = 0; i < elements_count; ++i) {
        raw_arr[i] = new int(i);
    }
    for (size_t i = 0; i < elements_count; ++i) {
        delete raw_arr[i];
    }
    delete[] raw_arr;
    auto end = std::chrono::high_resolution_clock::now();
    result.time_raw = std::chrono::duration<double>(end - start).count();

    start = std::chrono::high_resolution_clock::now();
    DynamicArray<ShrdPtr<int>> my_arr(elements_count);
    for (size_t i = 0; i < elements_count; ++i) {
        UnqPtr<int> temp(new int(i));
        my_arr.PushBack(ShrdPtr<int>(static_cast<UnqPtr<int>&&>(temp)));
    }
    end = std::chrono::high_resolution_clock::now();
    result.time_shrd = std::chrono::duration<double>(end - start).count();

    start = std::chrono::high_resolution_clock::now();
    std::shared_ptr<int>* stl_arr = new std::shared_ptr<int>[elements_count];
    for (size_t i = 0; i < elements_count; ++i) {
        stl_arr[i] = std::make_shared<int>(i);
    }
    delete[] stl_arr;
    end = std::chrono::high_resolution_clock::now();
    result.time_stl = std::chrono::duration<double>(end - start).count();

    return result;
}

std::vector<LoadTestResult> RunAllLoadTests() {
    std::vector<LoadTestResult> results;
    results.push_back(RunSingleLoadTest(10));
    results.push_back(RunSingleLoadTest(1000));
    results.push_back(RunSingleLoadTest(100000));
    results.push_back(RunSingleLoadTest(1000000));
    return results;
}