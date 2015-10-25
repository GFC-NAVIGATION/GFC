
//============================================================================
//
//  This file is part of GFC, the GNSS FOUNDATION CLASS.
//
//  The GFC is free software; you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published
//  by the Free Software Foundation; either version 2.1 of the License, or
//  any later version.
//
//  The GFC is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with GFC; if not, write to the Free Software Foundation,
//  Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
//
//  Copyright 2015, lizhen
//
//============================================================================


#ifndef  GFCMATRIX_H
#define  GFCMATRIX_H

#include "Platform.h"
#include <iostream>
#include <iomanip>
#include <complex>

//#include "../lapacklib/lapacke.h"
//#ifdef _WIN32_
//动态链接
//#pragma comment(lib,"../lapacklib/libblas.lib")
//#pragma comment(lib,"../lapacklib/liblapacke.lib")
//#endif

using namespace std;

namespace gfc
{
	class GMatrix
	{
	  	
	public:
		GMatrix();  //构造函数
		GMatrix( double* data,int nrow,int ncol);
		GMatrix( const GMatrix& matrix);   //拷贝构造函数
		GMatrix& operator= (const GMatrix& right);  //赋值重载
		GMatrix  operator/ (const GMatrix& right);  //矩阵右除运算,对于右除式A/B，相当于A*inv(B)
		//gfcMatrix  operator\ (const gfcMatrix& right);  //矩阵左除运算
		
		GMatrix&  operator~ (); //transpose
		GMatrix&  operator! (); //inverse
		double det();             //det  
		
		
		friend inline ostream& operator>> (ostream& os, GMatrix& m)
		{
			
			return os;
		}
	
			
		friend inline ostream &operator<< (ostream& os,GMatrix& m)
		{
			os<<"matrix object"<<std::endl;
			//os.setf(ios::fixed);//
			//os<<setprecision(8);
			for( int i = 0 ; i< m.m_rowno ; i++ )
			{
				for( int j = 0 ; j < m.m_colno ; j++ )
				{
					os<<m.m_data[i*m.m_colno+j]<<"    ";
				}
				os<<std::endl;
			}
			return os;
		}
		
		void dump();  //output some data
	
		void testLapack();
        
		//void print_matrix( char* desc, lapack_int m, lapack_int n, double* a, lapack_int lda );
		//void print_int_vector( char* desc, lapack_int n, lapack_int* a );
		
		~GMatrix(); 
	  
      //LU分解相关的函数
      static  int  ludcom( double *A, int n, int *indx, double& d);
      static  void lubksb( const double *A, int n, const int *indx, double *b);
        
    private:
		double*  m_data; 
		int      m_rowno; 
		int      m_colno;
			
	};

}

#endif