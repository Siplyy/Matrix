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
struct NSquareMatrixException : public std::runtime_error {
public:
    NSquareMatrixException(const std::string &message) : std::runtime_error(message) {}
};

struct SizeMismatchException : public std::runtime_error {
    SizeMismatchException(const std::string &message) : std::runtime_error(message) {}
};

template<typename T>
class Matrix {
public:
    //Basic constructor
    Matrix() {
        this->n = 0;
        this->m = 0;
        this->data = nullptr;
    };

    //Console constructor that only reads the size
    Matrix(size_t N, size_t M) {
        this->n = N;
        this->m = M;
    };

    //Console constructor
    Matrix(size_t N, size_t M, const vector<vector<T> > values) {
        this->n = N;
        this->m = M;
        this->data = values;
    };

    //Copy constructor
    Matrix(const Matrix<T> &matrix) {
        this->n = matrix.n;
        this->m = matrix.m;
        vector<vector<T> > values = matrix.data;
        this->data = values;
    };

    //File constructor
    Matrix(string filename) {
        std::ifstream in(filename);
        if (in.is_open()) {
            size_t N, M;
            in >> N >> M;
            if ((N == 0) || (M == 0)) {
                throw std::runtime_error("The dimensions of the given matrix cannot be equal to 0.");
            }
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
            throw std::runtime_error("file is not open");
        }
    };

    //File constructor that reads only values
    Matrix(size_t N, size_t M, string filename) {
        std::ifstream in(filename);
        this->n = N;
        this->m = M;
        if (in.is_open()) {
            vector<vector<T> > values;
            for (size_t i = 0; i < N; ++i) {
                vector<T> line;
                for (size_t j = 0; j < M; ++j) {
                    T value = 0;
                    in >> value;
                    line.push_back(value);
                }
                values.push_back(line);
            }
            this->data = values;
        } else {
            throw std::runtime_error("file is not open");
        }
    };

    //Type matrix constructor
    Matrix(string Type, size_t N, size_t M) {
        string type = Type;
        this->n = N;
        this->m = M;
        if (type == "0") {
            vector<vector<T> > values(n, vector<T>(m, 0));
            this->data = values;
        } else if ((type == "1") && (N == M)) {
            vector<vector<T> > values;
            for (size_t i = 0; i < N; ++i) {
                vector<T> line;
                for (size_t j = 0; j < M; ++j) {
                    if (i == j) {
                        line.push_back(1);
                    } else {
                        line.push_back(0);
                    }
                }
                values.push_back(line);
            }
            this->data = values;
        } else { throw std::runtime_error("Non-standard matrix introduced"); }
    };

    //Assignment operator
    Matrix<T> &operator=(const Matrix<T> &B) {
        if ((this->n != B.n) || (this->m != B.m)) {
            throw SizeMismatchException("Action cannot be performed with matrices of different sizes");
        }
        for (size_t i = 0; i < this->n; ++i) {
            for (size_t j = 0; j < this->m; ++j) {
                this->data[i][j] = B.data[i][j];
            }
        }
        return *this;
    };

    //Comparison operator for two matrices
    bool operator==(const Matrix<T> &B) {
        if ((this->n != B.n) || (this->m != B.m)) {
            return false;
        }
        for (size_t i = 0; i < this->n; ++i) {
            for (size_t j = 0; j < this->m; ++j) {
                if (this->data[i][j] != B.data[i][j]) {
                    return false;
                }
            }
        }
        return true;
    };

    //
    bool operator!=(const Matrix<T> &B) {
        if ((this->n != B.n) || (this->m != B.m)) {
            return false;
        }
        for (size_t i = 0; i < this->n; ++i) {
            for (size_t j = 0; j < this->m; ++j) {
                if (this->data[i][j] != B.data[i][j]) {
                    return true;
                }
            }
        }
        return false;
    };

    //Operator for comparing matrices with typical ones
    bool operator==(string a) {
        if (a == "0") {
            Matrix<T> B(a, this->n, this->m);
            for (size_t i = 0; i < this->n; ++i) {
                for (size_t j = 0; j < this->m; ++j) {
                    if (this->data[i][j] != B.data[i][j]) {
                        return false;
                    }
                }
            }
            return true;
        }
        if (a == "1") {
            if (this->n == this->m) {
                Matrix<T> B(a, this->n, this->m);
                for (size_t i = 0; i < this->n; ++i) {
                    for (size_t j = 0; j < this->m; ++j) {
                        if (this->data[i][j] != B.data[i][j]) {
                            return false;
                        }
                    }
                }
                return true;
            } else {
                return false;
            }
        } else {
            throw std::runtime_error("Non-standard matrix introduced");
        }
    };

    // != Operator for comparing matrices with typical ones
    bool operator!=(string a) {
        if (a == "0") {
            Matrix<T> B(a, this->n, this->m);
            for (size_t i = 0; i < this->n; ++i) {
                for (size_t j = 0; j < this->m; ++j) {
                    if (this->data[i][j] != B.data[i][j]) {
                        return true;
                    }
                }
            }
            return false;
        }
        if (a == "1") {
            if (this->n == this->m) {
                Matrix<T> B(a, this->n, this->m);
                for (size_t i = 0; i < this->n; ++i) {
                    for (size_t j = 0; j < this->m; ++j) {
                        if (this->data[i][j] != B.data[i][j]) {
                            return true;
                        }
                    }
                }
                return false;
            } else {
                return false;
            }
        } else {
            throw std::runtime_error("Non-standard matrix introduced");
        }
    };

    //Addition operator
    Matrix<T> operator+(const Matrix<T> &B) {
        if ((this->n != B.n) || (this->m != B.m)) {
            throw SizeMismatchException("Action cannot be performed with matrices of different sizes");
        }
        Matrix<T> Result("0", this->n, this->m);
        for (size_t i = 0; i < this->n; ++i) {
            for (size_t j = 0; j < this->m; ++j) {
                Result.data[i][j] = this->data[i][j] + B.data[i][j];
            }
        }
        return Result;
    };

    //subtraction operator
    const Matrix<T> operator-(const Matrix<T> &B) {
        if ((this->n != B.n) || (this->m != B.m)) {
            throw SizeMismatchException("Action cannot be performed with matrices of different sizes");
        }
        Matrix<T> Result("0", this->n, this->m);
        for (size_t i = 0; i < this->n; ++i) {
            for (size_t j = 0; j < this->m; ++j) {
                Result.data[i][j] = this->data[i][j] - B.data[i][j];
            }
        }
        return Result;
    };

    // Matrix multiplication operator
    Matrix<T> operator*(const Matrix<T> &B) {
        if (this->m != B.n) {
            throw SizeMismatchException("Action cannot be performed with matrices of different sizes");

        }
        Matrix<T> Result("0", this->n, B.m);
        for (size_t i = 0; i < this->n; ++i) {
            for (size_t j = 0; j < B.m; ++j) {
                size_t k = 0;
                size_t l = 0;
                while ((k < this->m) && (l < B.n)) {
                    Result.data[i][j] += this->data[i][k] * B.data[l][j];
                    k += 1;
                    l += 1;
                }
            }
        }
        return Result;
    }

    // Creating an inverse matrix
    Matrix<T> operator!() {
        if (this->n != this->m) {
            throw NSquareMatrixException("Action cannot be performed with non square matrix");
        }
        if (this->det() == 0) {
            throw std::runtime_error("This matrix doesn't have inverse matrix");
        }
        T det = this->det();
        Matrix<T> Result("0", this->n, this->n);
        for (size_t i = 0; i < this->n; ++i) {
            for (size_t j = 0; j < this->n; ++j) {
                Result.data[i][j] = this->AlgComplement(i, j);
            }
        }
        Result.Transpose();
        for (size_t i = 0; i < this->n; ++i) {
            for (size_t j = 0; j < this->n; ++j) {
                Result.data[i][j] /= det;
            }
        }
        return Result;
    }

    // Matrix transposition
    Matrix<T> Transpose() {
        Matrix<T> trans("0", this->m, this->n);
        for (size_t i = 0; i < this->n; ++i) {
            for (size_t j = 0; j < this->m; ++j) {
                trans.data[i][j] = this->data[j][i];
            }
        }
        return trans;
    }

    //Counting the determinant
    T det() {
        if (this->n != this->m) {
            throw NSquareMatrixException("Action cannot be performed with non square matrix");
        }
        if (this->n == 1)
            return this->data[0][0];
        T sum = 0;
        for (size_t j = 0; j < this->n; ++j) {
            sum += this->data[0][j] * this->AlgComplement(0, j);
        }
        return sum;
    }

    //Creation of a minor
    T Minor(size_t pos_n, size_t pos_m) {
        if (this->n != this->m) {
            throw NSquareMatrixException("Action cannot be performed with non square matrix");
        }
        Matrix<T> matrix("0", this->n - 1, this->m - 1);
        size_t pos_n1 = 0;
        for (size_t i = 0; i < this->n; ++i) {
            size_t pos_m1 = 0;
            for (size_t j = 0; j < this->n; ++j) {
                if (i != pos_n && j != pos_m) {
                    matrix.data[pos_n1][pos_m1] = this->data[i][j];
                    pos_m1++;
                }
            }
            if (i != pos_n) {
                pos_n1++;
            }
        }
        return matrix.det();
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
            cout << "Matrix is empty" << endl;
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
        cout << "Determinate of matrix A" << endl;
        cout << A.det() << endl;

        cout << "Enter dimensions of matrix B" << endl;
        size_t n2, m2;
        cin >> n2 >> m2;
        Matrix<double> B("0", n1, m1);

        cout << "Inverse matrix for A" << endl;
        B = !A;
        B.Matrix_print();

        Matrix<double> C("0", n1, m1);
        C = A + B;
        cout << "A + inverse A" << endl;
        C.Matrix_print();

        Matrix<double> D("0", n1, m1);
        cout << "A * Inverse A" << endl;
        D = A * B;
        D.Matrix_print();
    }
    catch (const NSquareMatrixException &e) {
        std::cerr << "Exception NSquareMatrix: " << e.what() << std::endl;
    }
    catch (const SizeMismatchException &e) {
        std::cerr << "Exception SizeMismatch: " << e.what() << std::endl;
    }
    catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    return 0;
}