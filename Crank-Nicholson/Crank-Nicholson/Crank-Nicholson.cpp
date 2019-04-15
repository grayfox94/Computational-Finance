// Crank-Nicholson.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <vector>
#include <math.h>
using namespace std;

int main()
{
	for (int timestep = 100; timestep < 200; timestep = timestep * 2) {
		//build payoff function
		double strike = 1;
		vector<double> uT;
		double dt = 1.0 / double(timestep);
		double dx = 2.0*dt;
		double n = 1 / (2 * dt); //time mesh
		double y = 2 / dx; //space mesh
		double q = 0.3; //sigma term
		double L = (-1.0 / 4.0)*(q*q)*dt / (dx*dx); //lambda term
		for (int i = 0; i < y; i++) {
			//compute expiratio boundary conditions
			if (strike - dx * i < 0) {
				uT.push_back(0);
			}
			else {
				uT.push_back(strike - dx * i);
			}

		}
		//calculate AX of AX=Bb
		for (int j = 0; j < n; j++) {
			vector<double>u_star;
			for (int i = 0; i < y; i++) {
				double a = L * (dx*dx)*(i)*(i);
				double b = 1 + 2 * L *(dx*dx)*(i)*(i);
				double c = L * (dx*dx)*(i)*(i);
				if (i == 0) {
					double temp = uT[i];
					u_star.push_back(temp);
				}
				else if (i == 1) {
					double temp = b * uT[i] - c * uT[i + 1] - 2 * a*uT[i - 1];
					u_star.push_back(temp);
				}
				else if (i >= y - 1) {
					double temp = 0;
					u_star.push_back(temp);
				}
				else {
					double temp = b * uT[i] - c * uT[i + 1] - a * uT[i - 1];
					u_star.push_back(temp);
				}
			}
			/*solve right hand side now*/
			vector<double> v; /*v*/
			v.push_back(1);
			vector<double> p; /*l*/
			p.push_back(1);
			vector<double> h; /*c*/
			h.push_back(0);
			vector<double> f;
			f.push_back(u_star[0]);
			for (int i = 1; i < y; i++) {
				double a = L * (dx*dx)*(i)*(i);
				double b = 1 - 2 * L *(dx*dx)*(i)*(i);
				double c = L * (dx*dx)*(i)*(i);
				h.push_back(c);
				if (i == 1) {
					v.push_back(b);
				}
				else {
					p.push_back(a / v[i - 1]);
					v.push_back(b - p[i - 1] * h[i - 1]);
				}
				f.push_back(u_star[i] - p[i - 1] * f[i - 1]);
			}
			for (int i = y; i > 0; i--) {
				if (i == y) {
					uT[i - 1] = 0;
				}
				else if (i == 1) {
					uT[i - 1] = 1;
				}
				else {
					uT[i - 1] = (f[i - 1] - h[i - 1] * uT[i]) / v[i - 1];
				}
				if (j == n - 1) {
					cout << "stock price: " << dx * (i - 1) << " option price: " << uT[i - 1] << endl;
				}
			}

		}
	}
    
}
