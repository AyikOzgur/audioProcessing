#pragma once

const double PI = 3.141592653589793238460;

struct Complex
{
    double real;
    double imag;

    Complex(double r = 0.0, double i = 0.0) : real(r), imag(i) {}

    // Operator overloads for complex number arithmetic
    Complex operator+(const Complex& other) const
    {
        return Complex(real + other.real, imag + other.imag);
    }

    Complex operator*(const Complex& exp) const
    {
        return Complex(real * exp.real - imag * exp.imag, real * exp.imag + imag * exp.real);
    }
};
