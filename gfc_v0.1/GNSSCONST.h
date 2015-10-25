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
 
 GNSS数据处理软件中的常量的存储
 
 其中的成员变量全部定义为static，方便调用
 
 该类中的常量全部在初始化时从文件中读取，
 未来替换掉代码中常量的定义文件constant.h 
 
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
         静态函数，用于对constantValue进行初始化
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
        
        static std::map< GString,  long double > constantValue;  //所有变量的值以及变量名均存储在这里
        
        //GString constFileName;  //常量配置文件(设计为文本文件)
        
        //std::map<Ellipsoid>   myEllipsoid;                        //所有的椭球信息
        
    };
    
    
    
    
    
    
}  // end of namespace


/*
 
 定义宏CONSTANT用于获取常量的值
 
 可以仿造LogStream类进行编写
 
 注意：常量名均为大写;
 
 调用格式如下：
 CONSTANT("CLIHT")
 CONSTANT("PI")
 
 */

#define GCONST(constantName) \
GNSSCONST::GetByName(constantName)

#endif


