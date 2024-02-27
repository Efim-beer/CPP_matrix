#include "s21_matrix_oop.h"

S21Matrix::S21Matrix() {
  rows_ = 3;
  cols_ = 3;
  create_matrix();
}

S21Matrix::S21Matrix(int rows, int cols) : rows_(rows), cols_(cols) {
  create_matrix();
}

S21Matrix::~S21Matrix() {
  if (matrix_) delete_matrix();
}

S21Matrix::S21Matrix(const S21Matrix &other)
    : rows_(other.rows_), cols_(other.cols_) {
  create_matrix();
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      matrix_[i][j] = other.matrix_[i][j];
    }
  }
}

S21Matrix::S21Matrix(S21Matrix &&other)
    : rows_(other.rows_), cols_(other.cols_), matrix_(other.matrix_) {
  other.cols_ = other.rows_ = 0;
  other.matrix_ = nullptr;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

bool S21Matrix::EqMatrix(const S21Matrix &other) {
  bool res = false;
  if (rows_ == other.rows_ && cols_ == other.cols_) {
    res = true;
    for (int i = 0; i < rows_; i++) {
      for (int j = 0; j < cols_; j++) {
        if (fabs(matrix_[i][j] - other.matrix_[i][j]) > 1e-7) {
          res = false;
          break;
        }
        if (!res) break;
      }
    }
  }
  return res;
}

void S21Matrix::SumMatrix(const S21Matrix &other) {
  this->SummMatrix(*this, other, 1);
}

void S21Matrix::SubMatrix(const S21Matrix &other) {
  this->SummMatrix(*this, other, 0);
}

void S21Matrix::MulNumber(const double num) {
  for (int i = 0; i < rows_; i++)
    for (int j = 0; j < cols_; j++) matrix_[i][j] *= num;
}

void S21Matrix::MulMatrix(const S21Matrix &other) {
  this->MultMatrix(*this, other);
}

S21Matrix S21Matrix::Transpose() {  // Создает новую транспонированную матрицу
                                    // из текущей и возвращает ее
  S21Matrix temp;
  if (rows_ > 0 && cols_ > 0) {
    temp = S21Matrix(cols_, rows_);
    for (int i = 0; i < rows_; i++)
      for (int j = 0; j < cols_; j++) temp.matrix_[j][i] = matrix_[i][j];
  }
  return temp;
}

S21Matrix S21Matrix::CalcComplements() {
  S21Matrix temp;
  if (rows_ > 0 && cols_ > 0) {
    temp = *this;
    if (rows_ == cols_) {
      int rows = temp.rows_;
      if (rows == 1) {
        temp.matrix_[0][0] = 1.0;
      } else if (rows == 2) {
        temp.matrix_[0][0] = matrix_[1][1];
        temp.matrix_[0][1] = -matrix_[1][0];
        temp.matrix_[1][0] = -matrix_[0][1];
        temp.matrix_[1][1] = matrix_[0][0];
      } else {
        for (int i = 0; i < rows; i++) {
          for (int j = 0; j < rows; j++) {
            temp.matrix_[i][j] = this->MinorMatrix(i, j) * pow(-1, (i + j));
          }
        }
      }
    } else {
      throw string("the matrix is not square");
    }
  } else {
    throw string("matrix colums and rows must above 0");
  }
  return temp;
}

double S21Matrix::MinorMatrix(int x, int y) {
  double res = 0.0;
  int rows = rows_ - 1;
  S21Matrix temp(rows, rows);
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < rows; j++) {
      int kx = 0, ky = 0;
      if (i >= x) kx = 1;
      if (j >= y) ky = 1;
      temp.matrix_[i][j] = matrix_[i + kx][j + ky];
    }
  }
  if (rows == 2) {
    res = temp.matrix_[0][0] * temp.matrix_[1][1] -
          temp.matrix_[0][1] * temp.matrix_[1][0];
  } else {
    res = temp.Triangle();
  }
  return res;
}

double S21Matrix::S21Sign(int n) { return (n % 2) ? -1.0 : 1.0; }

double S21Matrix::Determinant() {
  double result = NAN;
  if (rows_ > 0 && cols_ > 0) {
    result = 0;
    if (rows_ == cols_) {
      S21Matrix temp;
      temp = this->CalcComplements();
      for (int i = 0; i < rows_; i++)
        result += matrix_[0][i] * temp.matrix_[0][i];
    } else {
      throw string("the matrix is not square");
    }
  } else {
    throw string("matrix colums and rows must above 0");
  }
  return result;
}

S21Matrix
S21Matrix::InverseMatrix() {  // Вычисляет и возвращает обратную матрицу |
                              // определитель матрицы равен 0
  S21Matrix temp;
  if (rows_ == cols_) {
    double det = Determinant();
    if (det) {
      S21Matrix calccomp = CalcComplements();
      temp = calccomp.Transpose();
      for (int i = 0; i < temp.rows_; i++) {
        for (int j = 0; j < temp.cols_; j++) {
          temp.matrix_[i][j] = temp.matrix_[i][j] / det;
        }
      }
    } else {
      throw std::out_of_range("Matrix Determinant is 0");
    }
  } else {
    throw std::out_of_range("Incorrect size of matrixes");
  }
  return temp;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

S21Matrix S21Matrix::operator+(const S21Matrix &M_) {
  S21Matrix temp(*this);
  temp.SummMatrix(temp, M_, 1);
  return temp;
}

S21Matrix S21Matrix::operator-(const S21Matrix &M_) {
  S21Matrix temp(*this);
  temp.SummMatrix(temp, M_, 0);
  return temp;
}

S21Matrix S21Matrix::operator*(const S21Matrix &M_) {
  S21Matrix temp(*this);
  temp.MulMatrix(M_);
  return temp;
}

S21Matrix S21Matrix::operator*(const double num) {
  S21Matrix temp(*this);
  for (int i = 0; i < rows_; i++)
    for (int j = 0; j < cols_; j++) temp.matrix_[i][j] *= num;
  return temp;
}

bool S21Matrix::operator==(const S21Matrix &M_) {
  bool res = this->EqMatrix(M_);
  return res;
}

S21Matrix S21Matrix::operator=(const S21Matrix &M_) {
  if (cols_ > 0) {
    for (int i = 0; i < rows_; i++) delete[] matrix_[i];
  }
  if (rows_ > 0) delete[] matrix_;
  rows_ = M_.rows_;  // Копирование данных M <= M_
  cols_ = M_.cols_;
  matrix_ = (double **)new double *[rows_];  // Выделить память для M опять
  for (int i = 0; i < rows_; i++) matrix_[i] = (double *)new double[cols_];
  for (int i = 0; i < rows_; i++)  // заполнить значениями
    for (int j = 0; j < cols_; j++) matrix_[i][j] = M_.matrix_[i][j];
  return *this;
}

void S21Matrix::operator+=(const S21Matrix &M_) {
  this->SummMatrix(*this, M_, 1);
}

void S21Matrix::operator-=(const S21Matrix &M_) {
  this->SummMatrix(*this, M_, 0);
}

void S21Matrix::operator*=(const S21Matrix &M_) { this->MulMatrix(M_); }

void S21Matrix::operator*=(const double num) {
  for (int i = 0; i < rows_; i++)
    for (int j = 0; j < cols_; j++) matrix_[i][j] *= num;
}

double &S21Matrix::operator()(int i, int j) {
  // double res = NAN;
  if (i < 1 || j < 1) {
    throw string("matrix colums and rows must above 0");
  } else if (rows_ < i || cols_ < j) {
    throw string("index is outside the matrix");
  }
  return matrix_[i - 1][j - 1];
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void S21Matrix::SetMij(int i, int j, double value) {
  if ((i > 0) || (i < rows_) || (j > 0) || (j < cols_)) {
    matrix_[i][j] = value;
  }
}

int S21Matrix::GetRows() { return rows_; }

int S21Matrix::GetCols() { return cols_; }

void S21Matrix::SetRows(int num) {
  if (num > 0) {
    S21Matrix tmp(num, cols_);
    for (int i = 0; i < tmp.rows_; i++) {
      for (int j = 0; j < tmp.cols_; j++) {
        if (i < rows_) {
          tmp.matrix_[i][j] = matrix_[i][j];
        } else {
          tmp.matrix_[i][j] = 0.0;
        }
      }
    }
    delete_matrix();
    rows_ = num;
    create_matrix();
    *this = tmp;
  } else {
    throw std::out_of_range("Incorrect input, index is out of range");
  }
}

void S21Matrix::SetCols(int num) {
  if (num > 0) {
    S21Matrix tmp(rows_, num);
    for (int i = 0; i < tmp.rows_; i++) {
      for (int j = 0; j < tmp.cols_; j++) {
        if (j < cols_) {
          tmp.matrix_[i][j] = matrix_[i][j];
        } else {
          tmp.matrix_[i][j] = 0.0;
        }
      }
    }
    delete_matrix();
    cols_ = num;
    create_matrix();
    *this = tmp;
  } else {
    throw std::out_of_range("Incorrect input, index is out of range");
  }
}

void S21Matrix::FillMatrix() {
  for (auto i = 0; i < rows_; i++) {
    for (auto j = 0; j < cols_; j++) {
      matrix_[i][j] = i + j;
    }
  }
}

void S21Matrix::SummMatrix(const S21Matrix &other_1, const S21Matrix &other_2,
                           bool plus) {
  if (other_1.rows_ > 0 && other_2.rows_ > 0 && other_1.cols_ > 0 &&
      other_2.cols_ > 0) {
    if (other_1.rows_ == other_2.rows_ && other_1.cols_ == other_2.cols_) {
      for (int i = 0; i < rows_; i++)
        for (int j = 0; j < cols_; j++)
          if (plus)
            matrix_[i][j] = other_1.matrix_[i][j] + other_2.matrix_[i][j];
          else
            matrix_[i][j] = other_1.matrix_[i][j] - other_2.matrix_[i][j];
    } else {
      throw string("different matrix dimensions");
    }
  } else {
    throw string("matrix colums and rows must above 0");
  }
}

void S21Matrix::MultMatrix(const S21Matrix &other_1, const S21Matrix &other_2) {
  if (other_1.rows_ > 0 && other_2.rows_ > 0 && other_1.cols_ > 0 &&
      other_2.cols_ > 0) {
    if (other_2.rows_ == other_1.cols_) {
      S21Matrix temp(other_1.rows_, other_2.cols_);
      for (int i = 0; i < other_1.rows_; i++) {
        for (int j = 0; j < other_2.cols_; j++) {
          for (int k = 0; k < other_1.cols_ && k < other_2.rows_; k++) {
            double x = other_1.matrix_[i][k] * other_2.matrix_[k][j];
            temp.matrix_[i][j] += x;
          }
        }
      }
      temp.Round();
      *this = temp;
    } else {
      throw string(
          "the number of columns of the first matrix is not equal to the "
          "number of rows of the second matrix");
    }
  } else {
    throw string("matrix colums and rows must above 0");
  }
}

void S21Matrix::Round() {
  for (int i = 0; i < rows_; i++)
    for (int j = 0; j < cols_; j++)
      if (matrix_[i][j] < 1e-7) matrix_[i][j] = 0.0;
}

double S21Matrix::Triangle() {
  int point = 1;
  double res = 1.0;
  for (int i = 0; i < rows_ - 1; i++) {
    if (!matrix_[i][i]) {
      point = this->Change(i);
    }
    if (!point) {
      res = 0.0;
      break;
    } else {
      for (int j = i + 1; j < rows_; j++) {
        double value = matrix_[j][i];
        if (value) {
          for (int k = i; k < rows_; k++) {
            matrix_[j][k] =
                matrix_[j][k] - matrix_[i][k] * value / matrix_[i][i];
          }
        }
      }
    }
  }
  if (res) {
    for (int i = 0; i < rows_; i++) {
      res *= matrix_[i][i];
    }
  }
  return res;
}

int S21Matrix::Change(int a) {
  int res = 0, i = a + 1;
  double num;
  while (!matrix_[a][a] && i < rows_) {
    for (int j = a; j < cols_; j++) {
      num = matrix_[a][j];
      matrix_[a][j] = matrix_[i][j];
      matrix_[i][j] = -num;
      if (matrix_[a][a]) res = 1;
    }
    if (res) break;
    i++;
  }
  return res;
}

void S21Matrix::create_matrix() {
  if (rows_ < 1 || cols_ < 1) {
    throw std::invalid_argument("matrix colums and rows must above 0");
  }
  matrix_ = new double *[rows_];
  for (auto i = 0; i < rows_; i++) {
    matrix_[i] = new double[cols_]{0};
  }
}

void S21Matrix::delete_matrix() {
  for (auto i = 0; i < rows_; i++) {
    delete[] matrix_[i];
  }
  delete[] matrix_;
}

// void S21Matrix::Print() {
//   for (int i = 0; i < rows_; i++) {
//     for (int j = 0; j < cols_; j++)
//       cout << matrix_[i][j] << "\t";
//     cout << endl;
//   }
//   cout << "---------------------" << endl;
// }
