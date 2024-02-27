#ifndef _SRC_S21M_ATRIX_OOP_H_
#define _SRC_S21M_ATRIX_OOP_H_

#include <cmath>
#include <iostream>

using namespace std;

class S21Matrix {
 private:
  int rows_, cols_;
  double **matrix_;

 public:
  S21Matrix();
  S21Matrix(int rows, int cols);
  S21Matrix(const S21Matrix &other);
  S21Matrix(S21Matrix &&other);
  ~S21Matrix();
  void create_matrix();
  void delete_matrix();

  S21Matrix FillMatrixExample();
  bool EqMatrix(const S21Matrix &other);
  void SumMatrix(const S21Matrix &other);
  void SubMatrix(const S21Matrix &other);
  void MulNumber(const double num);
  void MulMatrix(const S21Matrix &other);
  S21Matrix Transpose();
  S21Matrix CalcComplements();
  double MinorMatrix(int r, int c);
  double S21Sign(int n);
  double Determinant();
  S21Matrix InverseMatrix();

  S21Matrix operator+(const S21Matrix &M_);
  S21Matrix operator-(const S21Matrix &M_);
  S21Matrix operator*(const S21Matrix &M_);
  S21Matrix operator*(const double num);
  bool operator==(const S21Matrix &M_);
  S21Matrix operator=(const S21Matrix &M_);
  void operator+=(const S21Matrix &M_);
  void operator-=(const S21Matrix &M_);
  void operator*=(const S21Matrix &M_);
  void operator*=(const double num);
  double &operator()(int i, int j);
  void SetMij(int i, int j, double value);

  int GetRows();
  void SetRows(int i);
  int GetCols();
  void SetCols(int i);

  double Triangle();
  int Change(int a);
  void Round();
  void FillMatrix();
  void SummMatrix(const S21Matrix &other_1, const S21Matrix &other_2,
                  bool plus);
  void MultMatrix(const S21Matrix &other_1, const S21Matrix &other_2);
};
#endif  // SRC_S21M_ATRIX_OOP_H_
