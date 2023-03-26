#include"pch.h"
#include"func.h"


int svd_hestenes(int m_m, int n_n, vector<double>& a, vector<double>& u, vector<double>& v, vector<double>& sigma)
{
	float thr = 0.000001f;
	int n, m, i, j, l, k, lort, iter, in, ll, kk;
	float alfa, betta, hamma, eta, t, cos0, sin0, buf, s, r;
	n = n_n;
	m = m_m;
	for (i = 0; i < n; i++)
	{
		in = i * n;
		for (j = 0; j < n; j++)
			if (i == j) v[in + j] = 1.;
			else v[in + j] = 0.;
	}
	for (i = 0; i < m; i++)
	{
		in = i * n;
		for (j = 0; j < n; j++)
		{
			u[in + j] = a[in + j];
		}
	}

	iter = 0;
	while (1)
	{
		lort = 0;
		iter++;
		for (l = 0; l < n - 1; l++)
			for (k = l + 1; k < n; k++)
			{
				alfa = 0.; betta = 0.; hamma = 0.;
				for (i = 0; i < m; i++)
				{
					in = i * n;
					ll = in + l;
					kk = in + k;
					alfa += u[ll] * u[ll];
					betta += u[kk] * u[kk];
					hamma += u[ll] * u[kk];
				}

				if (sqrt(alfa * betta) < 1.e-10)	continue;
				if (fabs(hamma) / sqrt(alfa * betta) < thr)
					continue;

				lort = 1;
				eta = (betta - alfa) / (2.f * hamma);
				t = (eta / fabs(eta)) /
					(fabs(eta) + (float)sqrt(1.f + eta * eta));
				cos0 = 1.f / (float)sqrt(1.f + t * t);
				sin0 = t * cos0;

				for (i = 0; i < m; i++)
				{
					in = i * n;
					buf = u[in + l] * cos0 - u[in + k] * sin0;
					u[in + k] = u[in + l] * sin0 + u[in + k] * cos0;
					u[in + l] = buf;

					if (i >= n) continue;
					buf = v[in + l] * cos0 - v[in + k] * sin0;
					v[in + k] = v[in + l] * sin0 + v[in + k] * cos0;
					v[in + l] = buf;
				}
			}

		if (!lort) break;
	}

	for (i = 0; i < n; i++)
	{
		s = 0.;
		for (j = 0; j < m; j++)	s += u[j * n + i] * u[j * n + i];
		s = (float)sqrt(s);
		sigma[i] = s;
		if (s < 1.e-10)	continue;
		for (j = 0; j < m; j++)	u[j * n + i] = u[j * n + i] / s;
	}
	for (i = 0; i < n - 1; i++)
		for (j = i; j < n; j++)
			if (sigma[i] < sigma[j])
			{
				r = sigma[i]; sigma[i] = sigma[j]; sigma[j] = r;
				for (k = 0; k < m; k++)
				{
					r = u[i + k * n]; u[i + k * n] = u[j + k * n]; u[j + k * n] = r;
				}
				for (k = 0; k < n; k++)
				{
					r = v[i + k * n]; v[i + k * n] = v[j + k * n]; v[j + k * n] = r;
				}
			}

	return iter;
}
float MHJ(int kk, vector<vector<double>>A, vector<double>B,vector<double>& x, int& calc)
{
	calc = 0;
	// kk - количество параметров; x - массив параметров
	float  TAU = 1.e-6f; // Точность вычислений
	int i, j, bs, ps;
	float z, h, k, fi, fb;
	float* b = new float[kk];
	float* y = new float[kk];
	float* p = new float[kk];

	h = 1.;
	x[0] = 1.;
	for (i = 1; i < kk; i++)  x[i] = (float)rand() / RAND_MAX; // Задается начальное приближение

	k = h;
	for (i = 0; i < kk; i++)	y[i] = p[i] = b[i] = x[i];
	fi = mist(A, B , x);
	ps = 0;   bs = 1;  fb = fi;

	j = 0;
	while (1)
	{
		calc++; // Счетчик итераций. Можно игнорировать

		x[j] = y[j] + k;
		z = mist(A, B , x);
		if (z >= fi) {
			x[j] = y[j] - k;
			z = mist(A, B , x);
			if (z < fi)   y[j] = x[j];
			else  x[j] = y[j];
		}
		else  y[j] = x[j];
		fi = mist(A, B , x);

		if (j < kk - 1) { j++;  continue; }
		if (fi + 1e-8 >= fb) {
			if (ps == 1 && bs == 0) {
				for (i = 0; i < kk; i++) {
					p[i] = y[i] = x[i] = b[i];
				}
				z = mist(A, B , x);
				bs = 1;   ps = 0;   fi = z;   fb = z;   j = 0;
				continue;
			}
			k /= 10.;
			if (k < TAU) break;
			j = 0;
			continue;
		}

		for (i = 0; i < kk; i++) {
			p[i] = 2 * y[i] - b[i];
			b[i] = y[i];
			x[i] = p[i];

			y[i] = x[i];
		}
		z = mist(A, B , x);
		fb = fi;   ps = 1;   bs = 0;   fi = z;   j = 0;
	} //  end of while(1)

	for (i = 0; i < kk; i++)  x[i] = p[i];

	delete b;
	delete y;
	delete p;
	return fb;
}
float LinearApprMHJ(int kk, vector<double> val, vector<double> key, vector<double>& x, int& calc)
{
	calc = 0;
	// kk - количество параметров; x - массив параметров
	float  TAU = 1.e-6f; // Точность вычислений
	int i, j, bs, ps;
	float z, h, k, fi, fb;
	float* b = new float[kk];
	float* y = new float[kk];
	float* p = new float[kk];

	h = 1.;
	x[0] = 1.;
	for (i = 1; i < kk; i++)  x[i] = (float)rand() / RAND_MAX; // Задается начальное приближение

	k = h;
	for (i = 0; i < kk; i++)	y[i] = p[i] = b[i] = x[i];
	fi = Apprfunc(val, key, x);
	ps = 0;   bs = 1;  fb = fi;

	j = 0;
	while (1)
	{
		calc++; // Счетчик итераций. Можно игнорировать

		x[j] = y[j] + k;
		z = Apprfunc(val, key, x);
		if (z >= fi) {
			x[j] = y[j] - k;
			z = Apprfunc(val, key, x);
			if (z < fi)   y[j] = x[j];
			else  x[j] = y[j];
		}
		else  y[j] = x[j];
		fi = Apprfunc(val, key, x);

		if (j < kk - 1) { j++;  continue; }
		if (fi + 1e-8 >= fb) {
			if (ps == 1 && bs == 0) {
				for (i = 0; i < kk; i++) {
					p[i] = y[i] = x[i] = b[i];
				}
				z = Apprfunc(val, key, x);
				bs = 1;   ps = 0;   fi = z;   fb = z;   j = 0;
				continue;
			}
			k /= 10.;
			if (k < TAU) break;
			j = 0;
			continue;
		}

		for (i = 0; i < kk; i++) {
			p[i] = 2 * y[i] - b[i];
			b[i] = y[i];
			x[i] = p[i];

			y[i] = x[i];
		}
		z = Apprfunc(val, key, x);
		fb = fi;   ps = 1;   bs = 0;   fi = z;   j = 0;
	} //  end of while(1)

	for (i = 0; i < kk; i++)  x[i] = p[i];

	delete b;
	delete y;
	delete p;
	return fb;
}
double Apprfunc(vector<double>& val, vector<double>& key, vector<double>& x)
{
	double res = 0;
	for (int i = 0; i < val.size(); i++)
	{
		double temp = val[i] - x[0] * key[i] - x[1];
		res += temp * temp;
	}
	return res;
}

double mist(vector<vector<double>>& A, vector<double>& B, vector<double>& x)
{
	double res = 0;
	for (int i = 0; i < A.size(); i++)
	{
		double xsum = 0;
		for (int g = 0; g < A[i].size(); g++)
		{
			xsum += A[i][g] * x[g];
		}
		xsum = B[i] - xsum;
		res += xsum * xsum;
	}
	return res;
}


vector<double>GetRandomMatrix(int n)
{
	int size = n * n;
	vector<double>res;
	for (int i = 0; i < size; i++)res.push_back(RANDMIN + RANDMAX * double(rand()) / double(RAND_MAX));
	return res;
}
vector<vector<double>>TransformLinearMatrix(vector<double> data, int n)
{
	vector<vector<double>>res(n, vector<double>());

	for (int i = 0; i < data.size(); i++)
	{
		res[i / n].push_back(data[i]);
	}

	return res;
}
vector<double>GetRandomB(int n)
{
	vector<double>res;
	for (int i = 0; i < n; i++)
	{
		res.push_back(RANDMIN + RANDMAX * double(rand()) / double(RAND_MAX));
	}
	return res;
}
void PrintEquation(vector<vector<double>>A, vector<double>B, ofstream& log)
{
	log << "\n\n\nУравнение: \n\n";
	for (int i = 0; i < A.size(); i++)
	{
		log << "|| ";
		for (int g = 0; g < A[i].size(); g++)
		{
			log << setprecision(4) << setw(10) << setfill(' ') << A[i][g];
			if (g != A[i].size() - 1)log << '\t';
		}

		log << " ||";
		log << '\t';

		log << "|| x" << i << " ||";
		if (i == (A.size() / 2))log << " = ";
		log << '\t';
		log << "|| " << setprecision(4) << setw(10) << setfill(' ') << B[i] << " ||" << endl;
	}
}


vector<double>calcSigma(vector<double>data, int n)
{
	int len = n * n;
	vector<double>sigma(n, 0);
	vector<double>v(len, 0);
	vector<double>u(len, 0);
	svd_hestenes(n, n, data, u, v, sigma);
	return sigma;
}
void PrintSigma(vector<double>& sigma, ofstream& log)
{
	log << "\n\n\nСигма:\n\n";
	for (int i = 0; i < sigma.size(); i++)
	{
		log << "Сигма " << i << " = " << setprecision(4) << setw(7) << setfill(' ') << sigma[i] << endl;
	}
}
void PrintCausuality(double data, ofstream& log)
{
	log<< "\n\n\nОбусловленность матрицы: " << setprecision(4) << setw(7) << setfill(' ') << data << endl;
}


void PrintX(vector<double>& x, ofstream& log)
{
	log << "\n\n\nКорни уравнения:\n\n";
	for (int i = 0; i < x.size(); i++)
	{
		log << "x" << i << " = " << setprecision(4) << setw(7) << setfill(' ') << x[i] << endl;
	}
}
void PrintIter(int i, ofstream& log)
{
	log << "\n\n\nЧисло Итераций: " << i;

}