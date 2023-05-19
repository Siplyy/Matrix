#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cmath>

using std::cin;
using std::cout;
using std::string;
using std::endl;
using std::vector;
using std::cerr;

//Errors
struct Exception {
    std::string err;
};

struct NSquareMatrix : public Exception {
    NSquareMatrix() { err = "The matrix is not square"; }
};

struct SizeMismatchErr : public Exception {
    SizeMismatchErr() { err = "Action cannot be performed with matrices of different sizes"; }
};

template<typename T>
class Matrix {
public:
    //Basic constructor
    Matrix() {
        this->n = 0;
        this->m = 0;
        this->data = NULL;
    };

    //Console constructor that only reads the size
    Matrix(size_t N, size_t M) {
        this->n = N;
        this->m = M;
    };

    //Console constructor
    Matrix(size_t N, size_t M, vector<vector<T> > ma) {
        this->n = N;
        this->m = M;
        this->data = ma;
    };

    //Copy constructor
    Matrix(Matrix<T> &matrix) {
        this->n = matrix.n;
        this->m = matrix.m;
        this->data = matrix.data;
    };

    //File constructor
    Matrix(string filename) {
        std::ifstream in(filename);
        if (in.is_open()) {
            size_t N, M;
            in >> N >> M;
            this->n = N;
            this->m = M;
            vector<vector<T> > a;
            for (size_t i = 0; i < N; ++i) {
                vector<T> a2;
                for (size_t j = 0; j < M; ++j) {
                    T k = 0;
                    in >> k;
                    a2.push_back(k);
                }
                a.push_back(a2);
            }
            this->data = a;
        } else {
            cerr << "Error opening file";
        }
    };

    //File constructor that reads only values
    Matrix(size_t N, size_t M, string filename) {
        std::ifstream in(filename);
        this->n = N;
        this->m = M;
        if (in.is_open()) {
            vector<vector<T> > a;
            for (size_t i = 0; i < N; ++i) {
                vector<T> a2;
                for (size_t j = 0; j < M; ++j) {
                    T k = 0;
                    in >> k;
                    a2.push_back(k);
                }
                a.push_back(a2);
            }
            this->data = a;
        } else {
            cerr << "Error opening file";
        }
    };

    //Type matrix constructor
    Matrix(string Type, size_t N, size_t M) {
        string type = Type;
        this->n = N;
        this->m = M;
        if (type == "0") {
            vector<vector<T> > matrz;
            for (size_t i = 0; i < N; ++i) {
                vector<T> a2;
                for (size_t j = 0; j < M; ++j) {
                    a2.push_back(0);
                }
                matrz.push_back(a2);
            }
            this->data = matrz;
        }
        if ((type == "1") && (N == M)) {
            vector<vector<T> > matrz;
            for (size_t i = 0; i < N; ++i) {
                vector<T> a2;
                for (size_t j = 0; j < M; ++j) {
                    if (i == j) {
                        a2.push_back(1);
                    } else {
                        a2.push_back(0);
                    }
                }
                matrz.push_back(a2);
            }
            this->data = matrz;
        }
        if (n != m) {
            std::cerr << "Non-standard matrix introduced";
        }
    };

    //Assignment operator
    Matrix<T> &operator=(const Matrix<T> &B) {
        if ((n = B.n) && (m = B.m)) {
            for (size_t i = 0; i < n; ++i) {
                for (size_t j = 0; j < m; ++j) {
                    data[i][j] += B.data[i][j];
                }
            }
            return *this;
        } else {
            SizeMismatchErr err;
            throw err;
        }
    };

    //Addition operator
    Matrix<T> &operator+(const Matrix<T> &B) {
        if ((n == B.n) && (m == B.m)) {
            for (size_t i = 0; i < n; ++i) {
                for (size_t j = 0; j < m; ++j) {
                    data[i][j] += B.data[i][j];
                }
            }
            return *this;
        } else {
            SizeMismatchErr err;
            throw err;
        }
    };

    //subtraction operator
    const Matrix<T> &operator-(const Matrix<T> &B) {
        if (n == B.n and m == B.m) {
            for (size_t i = 0; i < n; ++i) {
                for (size_t j = 0; j < m; ++j) {
                    data[i][j] -= B.data[i][j];
                }
            }
            return *this;
        } else {
            SizeMismatchErr error;
            throw error;
        }
    };

    //Comparison operator for two matrices
    bool operator==(const Matrix<T> &B) {
        if ((n = B.n) && (m = B.m)) {
            for (size_t i = 0; i < n; ++i) {
                for (size_t j = 0; j < m; ++j) {
                    if (data[i][j] != B.data[i][j]) {
                        cout << "The matrices are different" << endl;
                        return false;
                    }
                }
            }
            cout << "The matrices are equal";
            return true;
        } else {
            SizeMismatchErr error;
            throw error;
        }
    };

    //
    bool operator!=(const Matrix<T> &B) {
        if ((n = B.n) && (m = B.m)) {
            for (size_t i = 0; i < n; ++i) {
                for (size_t j = 0; j < m; ++j) {
                    if (data[i][j] != B.data[i][j]) {
                        cout << "The matrices are different" << endl;
                        return true;
                    }
                }
            }
            cout << "The matrices are equal";
            return false;
        } else {
            SizeMismatchErr error;
            throw error;
        }
    };

    //Operator for comparing matrices with typical ones
    bool operator==(string a) {
        if (a == "0") {
            Matrix<T> B(a, n, m);
            for (size_t i = 0; i < n; ++i) {
                for (size_t j = 0; j < m; ++j) {
                    if (data[i][j] != B.data[i][j]) {
                        cout << "The matrices are different" << endl;
                        return false;
                    }
                }
            }
            cout << "The matrices are equal" << endl;
            return true;
        }
        if (a == "1") {
            if (n == m) {
                Matrix<T> B(a, n, m);
                for (size_t i = 0; i < n; ++i) {
                    for (size_t j = 0; j < m; ++j) {
                        if (data[i][j] != B.data[i][j]) {
                            return false;
                        }
                    }
                }
                return true;
            } else {
                return false;
            }
        } else {
            std::cerr << "Non-standard matrix introduced";
        }
    };

    // != Operator for comparing matrices with typical ones
    bool operator!=(string a) {
        if (a == "0") {
            Matrix<T> B(a, n, m);
            for (size_t i = 0; i < n; ++i) {
                for (size_t j = 0; j < m; ++j) {
                    if (data[i][j] != B.data[i][j]) {
                        cout << "The matrices are different" << endl;
                        return true;
                    }
                }
            }
            cout << "The matrices are equal" << endl;
            return false;
        }
        if (a == "1") {
            if (n == m) {
                Matrix<T> B(a, n, m);
                for (size_t i = 0; i < n; ++i) {
                    for (size_t j = 0; j < m; ++j) {
                        if (data[i][j] != B.data[i][j]) {
                            return true;
                        }
                    }
                }
                return false;
            } else {
                return false;
            }
        } else {
            cerr << "Non-standard matrix introduced";
        }
    };

    // Matrix multiplication operator
    Matrix<T> operator*(const Matrix<T> &B) {
        if (m != B.n) {
            SizeMismatchErr error;
            throw error;

        }
        Matrix<T> D("0", n, B.m);
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = 0; j < B.m; ++j) {
                size_t k = 0;
                size_t l = 0;
                while ((k < m) && (l < B.n)) {
                    D.data[i][j] += data[i][k] * B.data[l][j];
                    k += 1;
                    l += 1;
                }
            }
        }
        this->data = D.data;
        this->n = D.n;
        this->m = D.m;
        return *this;
    }

    // Сreating an inverse matrix
    Matrix<T> operator!() {
        if (n != m) {
            NSquareMatrix err;
            throw err;
        }
        if (this->det() == 0) {
            cerr << "This matrix doesn't have inverse matrix";
        }
        T det = this->det();
        Matrix<T> Result("0", n, n);
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = 0; j < n; ++j) {
                Result.data[i][j] = this->AlgComplement(i, j);
            }
        }
        Result.Transpose();
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = 0; j < n; ++j) {
                Result.data[i][j] /= det;
            }
        }
        return Result;
    }

    // Matrix transposition
    Matrix<T> Transpose() {
        Matrix<T> trans("0", m, n);
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = 0; j < m; ++j) {
                trans.data[i][j] = this->data[j][i];
            }
        }
        return trans;
    }

    //Counting the determinant
    T det() {
        if (n != m) {
            NSquareMatrix err;
            throw err;
        }
        if (n == 1)
            return data[0][0];
        T sum = 0;
        for (size_t j = 0; j < n; ++j) {
            sum += data[0][j] * this->AlgComplement(0, j);
        }
        return sum;
    }

    //Creation of a minor
    T Minor(size_t i_, size_t j_) {
        if (n == m) {
            Matrix<T> matrix("0", n - 1, m - 1);
            size_t k = 0;
            for (size_t i = 0; i < n; ++i) {
                size_t h = 0;
                for (size_t j = 0; j < n; ++j) {
                    if (i != i_ && j != j_) {
                        matrix.data[k][h] = data[i][j];
                        h++;
                    }
                }
                if (i != i_)
                    k++;
            }
            return matrix.det();
        } else {
            NSquareMatrix error;
            throw error;
        }
    }

    //Counting Algebraic Complement
    T AlgComplement(size_t i, size_t j) {
        return pow(-1, i + j) * this->Minor(i, j);
    }

    //Matrix output
    void Matrix_print() {
        if (this->data.size() != 0) {
            int counter = 0;
            int N, M;
            N = this->n;
            M = this->m;
            vector<vector<T> > Data;
            Data = this->data;
            for (int i = 0; i < N; ++i) {
                if (counter != 0) {
                    cout << endl;
                }
                counter += 1;
                for (int j = 0; j < M; ++j) {
                    cout << Data[i][j] << " ";
                }
            }
            cout << endl;
        } else {
            cout << "Матрица пуста" << endl;
        }
    }

private:
    size_t n; // lines
    size_t m; // columns
    vector<vector<T> > data; // values
};

int main() {
    try {
        cout << "Enter dimensions of matrix A" << endl;
        size_t n1, m1;
        cin >> n1 >> m1;
        cout << "Enter the data of matrix A" << endl;
        vector<vector<double> > data1;
        for (size_t i = 0; i < n1; ++i) {
            vector<double> a1;
            for (size_t j = 0; j < m1; ++j) {
                double a = 0;
                cin >> a;
                a1.push_back(a);
            }
            data1.push_back(a1);
        }
        Matrix<double> A(n1, m1, data1);
        cout << A.det();
    }
    catch (Exception &err) {
        cout << err.err;
    }
}
//    cout << "Enter dimensions of matrix B" << endl;
//    size_t n2, m2;
//    cin >> n2 >> m2;
//    cout << "Enter the data of matrix B" << endl;
//    vector<vector<double> > data2;
//    for (size_t i = 0; i < n2; ++i) {
//        vector<double> a2;
//        for (size_t j = 0; j < m2; ++j) {
//            double a = 0;
//            cin >> a;
//            a2.push_back(a);
//        }
//        data2.push_back(a2);
//    }
//    Matrix<double> B(n2,m2,data2);