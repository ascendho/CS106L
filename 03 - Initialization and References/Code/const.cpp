// Compile and run this code using the following command:
// g++ -std=c++23 -o const const.cpp && ./const

#include <iostream>
#include <vector>

int main()
{
    std::vector<int> vec{ 1, 2, 3 };  /// a normal vector
    const std::vector<int> const_vec{ 1, 2, 3 };  /// a const vector
    std::vector<int>& ref_vec{ vec };  /// a reference to 'vec'
    const std::vector<int>& const_ref{ vec };  /// a const reference

    vec.push_back(3);
    const_vec.push_back(3); // This line will cause a compilation error because const_vec is a const vector
    ref_vec.push_back(3);
    const_ref.push_back(3); // This line will also cause a compilation error because const_ref is a const reference

    return 0;
}