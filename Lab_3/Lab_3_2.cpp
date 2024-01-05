#include <iostream> 
using namespace std;
template <typename t, int R, int C>
struct Matr {
    Matr<t, R / 2, C / 2> a; Matr<t, R / 2, C / 2> b;
    Matr<t, R / 2, C / 2> c; Matr<t, R / 2, C / 2> d;
    Matr(t* m, int cols = C) :
        a(m, cols), b(m + C / 2, cols), c(m + R / 2 * cols, cols), d(m + R / 2 * cols + C / 2, cols) {} Matr() {}
    ostream& print(ostream& o) {
        a.print(o << "[ ") << "] ";
        b.print(o << "[ ") << "] ";
        c.print(o << "[ ") << "] ";
        d.print(o << "[ ") << "] ";
        return o;
    }
};
template <typename t>
struct Matr<t, 1, 1> {
    t elem;
    Matr(t* m, int init_cols) :elem(*m) {}
    Matr() :elem(0) {}
    ostream& print(ostream& o) {
        return o << elem << " ";
    }
};

// Оператор сложения для общего случая 
template <typename t, int R, int C>
Matr<t, R, C> operator + (const Matr<t, R, C>& m1, const Matr<t, R, C>& m2) {
    Matr<t, R, C> res;
    res.a = m1.a + m2.a; res.b = m1.b + m2.b;
    res.c = m1.c + m2.c; res.d = m1.d + m2.d;
    return res;
}

// Оператор сложения для 1x1 матриц  
template <typename t>
Matr<t, 1, 1> operator + (const Matr<t, 1, 1>& m1, const Matr<t, 1, 1>& m2) {
    Matr<t, 1, 1> res;
    res.elem = m1.elem + m2.elem;
    return res;
}

// Оператор вычитания для общего случая
template <typename T, int R, int C>
Matr<T, R, C> operator-(const Matr<T, R, C>& m1, const Matr<T, R, C>& m2) {
    Matr<T, R, C> res;
    res.a = m1.a - m2.a;
    res.b = m1.b - m2.b;
    res.c = m1.c - m2.c;
    res.d = m1.d - m2.d;
    return res;
}

template <typename T, int R, int C>
Matr<T, R, C> operator-(const Matr<T, R, C>& m1) {
    Matr<T, R, C> res;
    res.a = -m1.a;
    res.b = -m1.b;
    res.c = -m1.c;
    res.d = -m1.d;
    return res;
}
// Унарный оператор минус для 1x1 матриц
template <typename T>
Matr<T, 1, 1> operator-(const Matr<T, 1, 1>& m1) {
    Matr<T, 1, 1> res;
    res.elem = -m1.elem;
    return res;
}
// Оператор вычитания для 1x1 матриц
template <typename T>
Matr<T, 1, 1> operator-(const Matr<T, 1, 1>& m1, const Matr<T, 1, 1>& m2) {
    Matr<T, 1, 1> res;
    res.elem = m1.elem - m2.elem;
    return res;
}


// Оператор умножения для общего случая
template <typename t, int R, int C>
Matr<t, R, C> operator * (const Matr<t, R, C>& m1, const Matr<t, R, C>& m2) {
    Matr<t, R, C> res;
    res.a = m1.a * m2.a + m1.b * m2.c;
    res.b = m1.a * m2.b + m1.b * m2.d;
    res.c = m1.c * m2.a + m1.d * m2.c;
    res.d = m1.c * m2.b + m1.d * m2.d;
    return res;
}

// Оператор умножения для 1x1 матриц
template <typename t>
Matr<t, 1, 1> operator * (const Matr<t, 1, 1>& m1, const Matr<t, 1, 1>& m2) {
    Matr<t, 1, 1> res;
    res.elem = m1.elem * m2.elem;
    return res;
}

template <typename t, int R, int C>
Matr<t, R * 2, C * 2> operator ^ (const Matr<t, R, C>& m1, const Matr<t, R, C>& m2) {
    Matr<t, R * 2, C * 2> res;

    // Верхний левый блок
    res.a = m1;

    // Верхний правый блок (нулевая матрица)
    res.b = Matr<t, R, C>();

    // Нижний левый блок (нулевая матрица)
    res.c = Matr<t, R, C>();

    // Нижний правый блок
    res.d = m2;

    return res;
}



// Функция для вычисления обратной матрицы для общего случая
template <typename t, int R, int C>
Matr<t, R, C> Inverse(const Matr<t, R, C>& m1) {
    Matr<t, R, C> res;
    res.a = Inverse(m1.a-m1.b*Inverse(m1.d)*m1.c);
    res.b = -Inverse(m1.a-m1.b*Inverse(m1.d)*m1.c)*m1.b*Inverse(m1.d);
    res.c = -Inverse(m1.d)*m1.c*Inverse(m1.a-m1.b*Inverse(m1.d)*m1.c);
    res.d = Inverse(m1.d)+Inverse(m1.d)*m1.c*Inverse(m1.a-m1.b*Inverse(m1.d)*m1.c)*m1.b*Inverse(m1.d);
    return res;
}
// Функция для вычисления обратной матрицы 1x1
template <typename t>
Matr<t, 1, 1> Inverse(const Matr<t, 1, 1>& m1) {
    Matr<t, 1, 1> res;
    res.elem = 1 / m1.elem;
    return res;
}

int main() {
    const int N = 4;
    float m1[N][N] = { {1,2,3,4},{0,1,2,3},{0,0,1,2},{0,0,0,1} }; Matr<float, N, N> a((float*)m1);
    float m2[N][N] = { {1,2,3,4},{0,1,2,3},{0,0,1,2},{0,0,0,1} }; Matr<float, N, N> b((float*)m2);


    /* Matr<float, N, N> c = a * b;
    cout << "Matrix a:" << endl;
    a.print(cout);
    cout << "\nMatrix b:" << endl;
    b.print(cout);
    cout << "\nresult:\n";
    c.print(cout);*/


    Matr<float, N * 2, N * 2> result = a ^ b;
    result.print(cout);
    cout << "\nresultinverse\n";
    auto resultinerse = Inverse(result);
    resultinerse.print(cout);
    cout << "\nMatrix result * resultinverse:" << endl;
    (result * resultinerse).print(cout);
    /*  Matr<float, N, N> invertedA = Inverse(a);
    cout << "\nInverse of Matrix a:" << endl;
    invertedA.print(cout);
    cout << "\nMatrix a * inverse:" << endl;
    (invertedA * a).print(cout);*/
  

    return 0;
}


