#pragma once
#include<fstream>
#include<vector>
#include<fstream>
#include<random>
#include<time.h>
#include<thread>
#include<iomanip>
using namespace std;


#define RANDMAX 200.

int svd_hestenes(int m_m, int n_n, vector<double>& a, vector<double>& u, vector<double>& v, vector<double>& sigma);
float MHJ(int kk, vector<vector<double>>A, vector<double> B, vector<double> &x, int& calc);


double mist(vector<vector<double>>& A, vector<double>& B, vector<double>& x);


vector<double>GetRandomMatrix(int n);
vector<vector<double>>TransformLinearMatrix(vector<double> data, int n);
vector<double>GetRandomB(int n);
void PrintEquation(vector<vector<double>>A, vector<double>B, ofstream&log);


vector<double>calcSigma(vector<double>data, int n);
void PrintSigma(vector<double>& sigma, ofstream& log);
void PrintCausuality(double data, ofstream& log);


void PrintX(vector<double>& x, ofstream& log);
void PrintIter(int i, ofstream& log);