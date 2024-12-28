#include <iostream>
#include <vector>

#define ERROR_MESSAGE std::invalid_argument("Error: vectors must be the same size")

// --- Your code here

template <typename T>
std::ostream& operator<<(std::ostream& ostream, std::vector<T> operand) {
    ostream << "[";
    for(int i=0; i < operand.size(); i++) {
        if (i != operand.size() - 1)
            ostream << operand[i] << ", ";
        else
            ostream << operand[i];
    }
    ostream << "]";
    return ostream;
}

template <typename add_1, typename add_2>
std::vector<double> operator+(const std::vector<add_1> a, const std::vector<add_2> b) {
    std::vector<double> out_vector;
    if (a.size() != b.size())
        throw ERROR_MESSAGE;
    for (int i = 0; i < a.size(); i++){
        out_vector.push_back(a[i] + b[i]);
    }
    return out_vector;
}

template <typename sub_1, typename sub_2>
std::vector<double> operator-(const std::vector<sub_1> a, const std::vector<sub_2> b) {
    std::vector<double> out_vector;
    if (a.size() != b.size())
        throw ERROR_MESSAGE;
    for (int i = 0; i < a.size(); i++){
        out_vector.push_back(a[i] - b[i]);
    }
    return out_vector;
}

template <typename mul_1, typename mul_2>
std::vector<double> operator*(const std::vector<mul_1> a, const std::vector<mul_2> b) {
    std::vector<double> out_vector;
    if (a.size() != b.size())
        throw ERROR_MESSAGE;
    for (int i = 0; i < a.size(); i++){
        out_vector.push_back(a[i] * b[i]);
    }
    return out_vector;
}

template <typename mul_scalar, typename mul_vector>
std::vector<double> operator*(const std::vector<mul_vector> a, const mul_scalar b) {
    std::vector<double> out_vector;
    for (int i = 0; i < a.size(); i++){
        out_vector.push_back(a[i] * b);
    }
    return out_vector;
}

template <typename T_mul_scalar, typename T_mul_vector>
std::vector<double> operator*(const T_mul_scalar b, const std::vector<T_mul_vector> a) {
    std::vector<double> out_vector;
    for (int i = 0; i < a.size(); i++){
        out_vector.push_back(a[i] * b);
    }
    return out_vector;
}

template <typename div_scalar, typename div_vector>
std::vector<double> operator/(const std::vector<div_vector> a, const div_scalar b) {
    std::vector<double> out_vector;
    if (b == 0)
        throw ERROR_MESSAGE;
    for (int i = 0; i < a.size(); i++){
        out_vector.push_back(a[i] / b);
    }
    return out_vector;
}

template <typename con_1, typename con_2>
std::vector<double> operator,(const std::vector<con_1> a, const con_2 b) {
    std::vector<double> out_vector;
    for (int i = 0; i < a.size(); i++)
        out_vector.push_back(a[i]);
    for (int i = 0; i < b.size(); i++)
        out_vector.push_back(b[i]);
    return out_vector;
}


// ---
