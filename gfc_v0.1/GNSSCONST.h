//============================================================================
//
//  This file is part of GFC, the GNSS FOUNDATION CLASS.
//
//  The GFC is free software; you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published
//  by the Free Software Foundation; either version 3.0 of the License, or
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


#ifndef GFC_GNSSCONST_H
#define GFC_GNSSCONST_H


/*
 
 GNSS���ݴ�������еĳ����Ĵ洢
 
 ���еĳ�Ա����ȫ������Ϊstatic���������
 
 �����еĳ���ȫ���ڳ�ʼ��ʱ���ļ��ж�ȡ��
 δ���滻�������г����Ķ����ļ�constant.h 
 
*/

#include "GString.h"
#include <map>
#include <iomanip>
#include <math.h>

namespace gfc
{
    
    NEW_GEXCEPTION_CLASS( constantUnexist, gfc::GException );
    
    class GNSSCONST
    {
        
    public:
        
        /*
         ��̬���������ڶ�constantValue���г�ʼ��
         */
        static std::map< GString,  long double > Initializer();
        static void RegByName(GString variableName,long double variableValue);
        static long double GetByName(GString variableName);
        
        static void UnregByName(GString variableName);
        static void dump( std::ostream& s ) ;
        
    private:
        
        // private construction function means this class can not be hesitated!
       	GNSSCONST(void);
        virtual ~GNSSCONST(void);
        
        static long double GetByName_internal( GString variableName,std::map< GString,  long double >& myconstantValue);
        
        static std::map< GString,  long double > constantValue;  //���б�����ֵ�Լ����������洢������
        
        //GString constFileName;  //���������ļ�(���Ϊ�ı��ļ�)
        
        //std::map<Ellipsoid>   myEllipsoid;                        //���е�������Ϣ
        
    };
    
    
    
    
    
    
}  // end of namespace


/*
 
 �����CONSTANT���ڻ�ȡ������ֵ
 
 ���Է���LogStream����б�д
 
 ע�⣺��������Ϊ��д;
 
 ���ø�ʽ���£�
 CONSTANT("CLIHT")
 CONSTANT("PI")
 
 */

#define GCONST(constantName) \
GNSSCONST::GetByName(constantName)

#endif


