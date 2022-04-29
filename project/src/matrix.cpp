#include <cmath>
#include <limits>
#include <iomanip>

#include "matrix.h"
#include "exceptions.h"

namespace prep {


void Matrix::init_data(){
    data =  new double[row_count * col_count];
}

Matrix::Matrix(size_t rows, size_t cols) {
    row_count = rows;
    col_count = cols;
    init_data();
    for (size_t i = 0; i < row_count * col_count; i++) {
        data[i] = 0.0;
    }
}

Matrix::Matrix(std::istream& is) {
    if (!is) {
        throw InvalidMatrixStream();
    }
    row_count = 0;
    col_count = 0;
    is >> row_count >> col_count;
    if (!row_count || !col_count) {
        throw InvalidMatrixStream();
    }
    init_data();
    size_t counter = 0;
    for (size_t i = 0; i < (row_count * col_count); i++) {
        is >> data[i];
        if (data[i]) {
            ++counter;
        }
    }
    if (counter != row_count * col_count) {
        throw InvalidMatrixStream();
    }
}

size_t Matrix::getRows() const {
    return row_count;
}

size_t Matrix::getCols() const {
    return col_count;
}

double Matrix::operator()(size_t i, size_t j) const {
    if (i > this->getRows() - 1 || j > this->getCols() - 1) {
        throw OutOfRange(i, j, *this);
    }
    return data[i * col_count + j];
}

double& Matrix::operator()(size_t i, size_t j) {
    if (i > this->getRows() -1  || j > this->getCols() - 1) {
        throw OutOfRange(i, j, *this);
    }
    return data[i * col_count + j];
}




bool Matrix::operator==(const Matrix& rhs) const {
    std::cout << "called == func: ";
    if (row_count != rhs.getRows() || col_count != rhs.getCols()) {
        std::cout << "matrixes not equal: different dimensions" << std::endl;
        return false;
    }
    for (size_t i = 0; i < row_count; i++) {
        for (size_t j = 0; j < col_count; j++) {                /* s etim padaet eshe i store test for whatever reason */
            if ( std::fabs( (*this)(i, j) - rhs(i, j))  >= 1e-7 /* std::numeric_limits<double>::epsilon() * 10e-7*/ ) {
                std::cout << "matrixes not equal: " << operator()(i, j) << " != " << rhs(i, j) << std::endl;
                return false;
            }
        }
    }
    std::cout << "success!! matrixes equal!" << std::endl;
    return true;
}

bool Matrix::operator!=(const Matrix& rhs) const {
    std::cout << "called =! func: calling ==... ";
    return !(*this == rhs);
}

Matrix Matrix::operator+(const Matrix& rhs) const {
    if (row_count != rhs.getRows() || col_count != rhs.getCols()) {
        throw DimensionMismatch(*this, rhs);
    }
    Matrix new_matrix = Matrix(row_count, col_count);
    for (size_t i = 0; i < row_count; i++) {
        for (size_t j = 0; j < col_count; j++) {
            new_matrix(i, j) = (*this)(i, j) + rhs(i, j);
        }
    }
    return new_matrix;
}

Matrix Matrix::operator-(const Matrix& rhs) const {
    if (row_count != rhs.getRows() || col_count != rhs.getCols()) {
        throw DimensionMismatch(*this, rhs);
    }
    Matrix new_matrix = Matrix(row_count, col_count);
    for (size_t i = 0; i < row_count; i++) {
        for (size_t j = 0; j < col_count; j++) {
            new_matrix(i, j) = (*this)(i, j) - rhs(i, j);
        }
    }
    return new_matrix;
}

Matrix Matrix::operator*(const Matrix& rhs) const {
    if (col_count !=  rhs.getRows()) {
        throw DimensionMismatch(*this, rhs);
    }
    Matrix new_matrix = Matrix(row_count, rhs.getCols());
    for (size_t i = 0; i < row_count; i++) {
        for (size_t j = 0; j < rhs.getCols(); j++) {
            double total = 0.0;
            for (size_t k = 0; k < col_count; k++) {
                total = total + (*this)(i, k) * rhs(k, j);
            }
            new_matrix(i, j) = total;
        }
    }
    return new_matrix;
}

Matrix Matrix::operator*(double val) const {
    Matrix new_matrix = Matrix(row_count, col_count);
    for (size_t i = 0; i < row_count; i++) {
        for (size_t j = 0; j < col_count; j++) {
            new_matrix(i, j) = (*this)(i, j) * val;
        }
    }
    return new_matrix;
}

Matrix operator*(double val, const Matrix& matrix) {
    return matrix * val;
}

std::ostream& operator<<(std::ostream& os, const Matrix& matrix) {
    os << matrix.getRows() << " " << matrix.getCols()  << std::endl;
    for (size_t i = 0; i < matrix.getRows(); i++) {
        for (size_t j = 0; j < matrix.getCols(); j++) {
            os << std::setprecision(10) << matrix(i, j) << " ";
        }
        os << std::endl;
    }
    return os;
}


Matrix Matrix::transp() const {
    Matrix new_matrix = Matrix(col_count, row_count);
    for (size_t i = 0; i < row_count; i++) {
        for (size_t j = 0; j < col_count; j++) {
            new_matrix(j, i) = (*this)(i, j);
        }
    }
    return new_matrix;
}

/***************** EXTRA OPERATIONS *****************/

double* Matrix::get_data_ptr() {
    return data;
}

Matrix Matrix::get_minor(size_t row, size_t col) const {
    if (row_count < 2 or col_count < 2) {
        throw DimensionMismatch((*this));
    }
    Matrix new_matrix = Matrix(row_count - 1, col_count - 1);
    double *ptr = new_matrix.get_data_ptr();
    for (size_t i = 0; i < row_count; i++) {
        for (size_t j = 0; j < col_count; j++) {
            if (i != row && j != col) {
                *ptr = (*this)(i, j);
                ptr++;
            }
        }
    }
    return new_matrix;
}

double Matrix::det() const {
    if (row_count != col_count) {
        throw DimensionMismatch((*this));
    }
    if (row_count == 1) {
        return (*this)(0, 0);
    } else if (row_count == 2) {
        return (*this)(0, 0) * (*this)(1, 1) -
               (*this)(0, 1) * (*this)(1, 0);
    } else {
        double sign = 1.0;
        double answ = 0.0;
        for (size_t j = 0; j < col_count; j++) {
            Matrix minor = get_minor(0, j);
            answ += (sign * (*this)(0, j) * minor.det());
            sign *= -1.0;
        }
        return answ;
    }
}

Matrix Matrix::adj() const {
    if (row_count != col_count) {
        throw DimensionMismatch((*this));
    } else if (row_count < 2) {
        Matrix new_matrix = (*this);
        return new_matrix;
    } else {
        Matrix new_matrix = Matrix(row_count, col_count);
        for (size_t i = 0; i < row_count; i++) {
            for (size_t j = 0; j < col_count; j++) {
                double sign = (i + j) % 2 == 0 ? 1.0 : -1.0;
                Matrix minor = get_minor(i, j);
                new_matrix(j, i) = sign * minor.det();
            }
        }
        return new_matrix;
    }
}

Matrix Matrix::inv() const {
    if (row_count != col_count) {
        throw DimensionMismatch((*this));
    } else if (row_count == 1) {
        Matrix inversed = Matrix(1, 1);
        inversed(0, 0) = 1 / (*this)(0, 0);  // det always equals 1
        return inversed;
    } else {
        Matrix inversed = (*this).adj() * (1 / (*this).det());
        return inversed;
    }
}

}  // namespace prep
