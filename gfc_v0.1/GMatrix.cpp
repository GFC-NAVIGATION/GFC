
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



#include "GMatrix.h"

namespace gfc
{
	
	
	GMatrix::GMatrix()
	{
		m_data = NULL;
		m_colno = 0;
		m_rowno = 0;
	}
	
	GMatrix::GMatrix( double* data,int nrow,int ncol )
	{
		if(  data != NULL && ncol != 0 && nrow != 0)
		{
			m_data = new double[ncol*nrow];
			memset(m_data,0,sizeof(double)*ncol*nrow);
		}
		memcpy( m_data,data, sizeof(double)*ncol*nrow);
		m_rowno = nrow;
		m_colno = ncol;
	}
	
	//copy construction
	GMatrix::GMatrix( const GMatrix& matrix )
	{
		m_colno = matrix.m_colno;
		m_rowno = matrix.m_rowno;		
		this->m_data = new double[m_colno*m_rowno]; 
		memcpy( this->m_data,matrix.m_data,sizeof(double)*m_colno*m_rowno );				
	}

	
	GMatrix& GMatrix::operator =( const GMatrix& right )
	{
		m_colno = right.m_colno;
		m_rowno = right.m_rowno;
		if(m_data != NULL )  { delete[] m_data; m_data = NULL; }
		m_data = new double[m_colno*m_rowno];
		memcpy(m_data,right.m_data,sizeof(double)*m_colno*m_rowno);
		return *this;
	}

    int GMatrix::ludcom(double *A, int n, int *indx, double& d)
    {
        double big,s,tmp;
        double *vv= new double[n]; //mat(n,1);
        memset(vv,0,sizeof(double)*n);
        int i=0,imax=0,j=0,k=0;
        d=1.0;
        for (i=0;i<n;i++ )
        {
            big=0.0;
            for (j=0;j<n;j++)
            {
                if ((tmp=fabs(A[i+j*n]))>big)
                    big=tmp;
            }
            
            if (big>0.0) vv[i]=1.0/big;
            else
            {
                if( vv != NULL )
                {
                    delete[] vv; vv = NULL;
                }
                return -1;
            }
        }
        for (j=0;j<n;j++)
        {
            for (i=0;i<j;i++)
            {
                s=A[i+j*n];
                for (k=0;k<i;k++)
                {
                    s-=A[i+k*n]*A[k+j*n];
                    A[i+j*n]=s;
                }
            }
            big=0.0;
            for (i=j;i<n;i++)
            {
                s=A[i+j*n];
                for (k=0;k<j;k++)
                {
                    s-=A[i+k*n]*A[k+j*n];
                    A[i+j*n]=s;
                }
                if ((tmp=vv[i]*fabs(s))>=big)
                {
                    big=tmp;
                    imax=i;
                }
            }
            if (j!=imax)
            {
                for (k=0;k<n;k++)
                {
                    tmp=A[imax+k*n];
                    A[imax+k*n]=A[j+k*n];
                    A[j+k*n]=tmp;
                }
                
                d=-(d);
                vv[imax]=vv[j];
            }
            indx[j]=imax;
            if (A[j+j*n]==0.0)
            {
                if(vv != NULL)
                {
                    delete[] vv; vv= NULL;
                }
                return -1;
            }
            if (j!=n-1)
            {
                tmp=1.0/A[j+j*n];
                for (i=j+1;i<n;i++)
                {
                A[i+j*n]*=tmp;
                }
            }
        }
        
        if(vv != NULL) {delete[] vv; vv = NULL;}
        return 0;
    }
    
    /* LU back-substitution ------------------------------------------------------*/
    void GMatrix::lubksb(const double *A, int n, const int *indx, double *b)
    {
        double s = 0.0;
        int i=0,ii=-1,ip=0,j=0;
        
        for ( i=0;i<n;i++ )
        {
            ip=indx[i]; s=b[ip]; b[ip]=b[i];
            if (ii>=0) for (j=ii;j<i;j++) s-=A[i+j*n]*b[j]; else if (s) ii=i;
            b[i]=s;
        }
        for (i=n-1;i>=0;i--)
        {
            s=b[i]; for (j=i+1;j<n;j++) s-=A[i+j*n]*b[j]; b[i]=s/A[i+i*n];
        }
    }
    
    // transpos the matrix
	GMatrix& GMatrix::operator~ ()
	{
		int_t tmp = m_rowno;
		m_rowno = m_colno;
		m_colno = tmp;
        double* tmpMatrix = new double[m_rowno*m_colno];
        memcpy(tmpMatrix,m_data,sizeof(double)*m_rowno*m_colno);
        for( int i = 0 ; i< m_rowno; i++ )
        {
            for( int j = 0 ; j< m_colno; j++ )
            {
                tmpMatrix[i*m_colno+j] = m_data[j*m_rowno+i];
            }
        }
        
        memcpy(m_data,tmpMatrix,sizeof(double)*m_rowno*m_colno);
        if(tmpMatrix != NULL) {delete[] tmpMatrix; tmpMatrix = NULL;}
		return *this;
	}

 // inverse
	GMatrix&  GMatrix::operator! () 
	{
		if( m_colno != m_rowno )
		{
			printf("m_rowno is not equal to m_rolno\n");
		}
        
        double d=0.0;
        int i = 0,j = 0, n= m_colno;
        int *indx = new int[n];
        double* B = new double[n*n];
       // double* A = new double[n*n];
       // memcpy( A, m_data, sizeof(double)*n*n);
        memcpy( B,  m_data,sizeof(double)*n*n);
        if ( ludcom(B,n,indx,d) )
        {
            if(indx != NULL) {delete[] indx; indx = NULL;}
            if( B != NULL) {delete[] B; B = NULL;}
            //if( A != NULL) {delete[] A; A = NULL;}
            
            // failed£¡£¡£¡£¡
            return *this;
        }
        for ( j=0;j<n;j++ )
        {
            for ( i=0;i<n;i++)
            {
                m_data[i+j*n]=0.0;
                m_data[j+j*n]=1.0;
            }
            
            lubksb( B,n,indx,m_data+j*n);
        }
        
        if( indx != NULL) {delete[] indx; indx = NULL;}
        
        if( B != NULL) {delete[] B; B = NULL;}
        //if( A != NULL) {delete[] A; A = NULL;}
        
        return *this;
        
        
//		// try to call the lapack library
//		lapack_int m = m_rowno;
//		lapack_int n = m_colno;
//		lapack_int lda = m_rowno;
//		lapack_int * ipiv = new lapack_int[m];
//		double* tmp = new double[m_rowno*m_colno];
//		memcpy( tmp,m_data,sizeof(double)*m_rowno*m_colno );
//		lapack_int info1 = LAPACKE_dgetrf_work( LAPACK_ROW_MAJOR,m,n,tmp,lda,ipiv );
//		if( info1 != 0 )
//		{
//			printf("function£ºLAPACKE_degtrf_work failed!\n");
//		}
//		 
//		lapack_int info2 = LAPACKE_dgetri(LAPACK_ROW_MAJOR,n,tmp,lda,ipiv);	
//		if( info2 == 0 )
//		{
//			memcpy(m_data,tmp,sizeof(double)*m_colno*m_rowno);
//		}
//		
//		if(ipiv != NULL )  {delete[] ipiv; ipiv = NULL;}
//		if(tmp != NULL )   {delete[] tmp;  tmp =  NULL;}
        
	}
	
	//
	double GMatrix::det()
	{
		double mydet = 1.0;
		if( m_colno != m_rowno )
		{
			printf("m_rowno is not equal to m_rolno\n");
		}
		
//		lapack_int m = m_rowno;
//		lapack_int n = m_colno;
//		lapack_int lda = m_rowno;
//		lapack_int * ipiv = new lapack_int[m];
//		double* tmp = new double[m_rowno*m_colno];
//		memcpy( tmp,m_data,sizeof(double)*m_rowno*m_colno );
//		
//		int info = LAPACKE_dgetrf_work( LAPACK_ROW_MAJOR,m,n,tmp,lda,ipiv );
//		if( info == 0 )
//		{
//			for(int i = 0 ; i< m_rowno ; i++)
//			mydet = mydet * tmp[i*m_colno+i];
//		}
//			
//		if(ipiv != NULL )  {delete[] ipiv; ipiv = NULL;}
//		if(tmp != NULL )   {delete[] tmp;  tmp =  NULL;}
//		
	

		return mydet;
	}

	void GMatrix::dump()
	{
		int_t i = 0 , j = 0;
		for( i = 0; i < m_colno; i++ ) 
		{
			for( j = 0; j < m_rowno; j++ )
				printf( " %6.2f", m_data[i*m_colno+j] );
			printf( "\n" );
		}
	}
	
	void  GMatrix::testLapack()
	{
		
//		/* Locals */
//		lapack_int n = 5, nrhs = 3, lda = 5, ldb = 3, info;
//		/* Local arrays */
//		lapack_int ipiv[5];
//		double a[] = 
//		{
//			6.80, -6.05, -0.45,  8.32, -9.67,
//			-2.11, -3.30,  2.58,  2.71, -5.14,
//			5.66, 5.36, -2.70,  4.35, -7.26,
//			5.97, -4.44,  0.27, -7.17, 6.08,
//			8.23, 1.08,  9.04,  2.14, -6.87
//		};
//		double b[] = 
//		{
//			4.02, -1.56, 9.81,
//			6.19,  4.00, -4.09,
//			-8.22, -8.67, -4.57,
//			-7.57,  1.75, -8.61,
//			-3.03,  2.86, 8.99
//		};
//		
//		/* Print Entry Matrix */
//		print_matrix( "Entry Matrix A", n, n, a, lda );
//		/* Print Right Rand Side */
//		print_matrix( "Right Rand Side", n, nrhs, b, ldb );
//		printf( "\n" );
//		/* Executable statements */
//		printf( "LAPACKE_dgesv (row-major, high-level) Example Program Results\n" );
//		/* Solve the equations A*X = B */
//		info = LAPACKE_dgesv( LAPACK_ROW_MAJOR, n, nrhs, a, lda, ipiv,b, ldb );
//		
//		/* Check for the exact singularity */
//		if( info > 0 ) 
//		{
//			printf( "The diagonal element of the triangular factor of A,\n" );
//			printf( "U(%i,%i) is zero, so that A is singular;\n", info, info );
//			printf( "the solution could not be computed.\n" );
//			exit( 1 );
//		}
//		/* Print solution */
//		print_matrix( "Solution", n, nrhs, b, ldb );
//		/* Print details of LU factorization */
//		print_matrix( "Details of LU factorization", n, n, a, lda );
//		/* Print pivot indices */
//		print_int_vector( "Pivot indices", n, ipiv );
	
    }

//	void gfcMatrix::print_matrix(char* desc, lapack_int m, lapack_int n, double* a, lapack_int lda)
//	{
//		lapack_int i, j;
//		printf( "\n %s\n", desc );
//		for( i = 0; i < m; i++ ) {
//			for( j = 0; j < n; j++ ) printf( " %6.2f", a[i*lda+j] );
//			printf( "\n" );
//		}	
//	}

	/* Auxiliary routine: printing a vector of integers */
//	void gfcMatrix::print_int_vector( char* desc, lapack_int n, lapack_int* a ) {
//		lapack_int j;
//		printf( "\n %s\n", desc );
//		for( j = 0; j < n; j++ ) printf( " %6i", a[j] );
//		printf( "\n" );
//	}
	

	GMatrix::~GMatrix()
	{
		if( m_data != NULL )
		{
			delete[] m_data;
			m_data = NULL;
		}
		m_rowno = 0;
		m_colno = 0;
	}

}