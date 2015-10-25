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

#ifndef GFC_GTIME_H
#define GFC_GTIME_H

#include "GException.h"
#include "GNSSCONST.h"
#include <algorithm>

namespace gfc
{
    NEW_GEXCEPTION_CLASS(InvalidTime,gfc::GException);
    
    NEW_GEXCEPTION_CLASS(TimeSystemUnexist, gfc::GException );
    
    /*ʱ��ϵͳ�Ķ���*/
    class TimeSystem  //ά��ʱ��ϵͳ�Ķ��弰ʱ��ϵͳ����
    {
        
    public:
        
        TimeSystem(void) { m_tsString="tsUKN";}
        TimeSystem(GString timesysString)  { m_tsString = timesysString;}
        virtual ~TimeSystem(void) {};
        
        static std::list<GString > Initializer();
        static void RegByName( GString variableName );
        
        static void UnregByName( GString variableName );
        static void dump( std::ostream& s ) ;
        
        static TimeSystem GetByName( GString variableName );
        
        TimeSystem& operator= (const TimeSystem& right)  //��ֵ����
        {
            this->m_tsString = right.m_tsString;
            return *this;
        }
        
        TimeSystem( const TimeSystem& right )   //�������캯��
        {
            this->m_tsString = right.m_tsString;
        }
        
        bool operator== (const TimeSystem& right)  //�Ⱥ�����
        {
            return this->m_tsString == right.m_tsString;
        }
        
        bool operator!= (const TimeSystem& right)  //���Ⱥ�����
        {
            return !(*this == right);
        }
        
        // return the name(GString) of certain timesystem
        GString getTimeSystemName() { return m_tsString;}
        
    private:
        
        static std::list<GString>  timesystemTab;
        
        GString m_tsString;
    };
    
    
#define GTimeSystem(TimeSystemName) \
TimeSystem::GetByName(TimeSystemName)
    
    
    
    class CivilTime
    {
        
    public:
        CivilTime() :m_year(0),m_month(0),m_day(0),m_hour(0),m_minute(0),m_second(0.0) {}
        ~CivilTime() {}
        
        int m_year;
        int m_month;
        int m_day;
        int m_hour;
        int m_minute;
        double m_second; //��λ����
        TimeSystem m_ts;
    };
    
    
    /*�����ʱ����ʽ*/
    class DOYTime
    {
        
    public:
        DOYTime() :m_year(0),m_doy(0),m_sod(0.0) {}
        ~DOYTime() {}
        
        int m_year;
        long m_doy;  //day of year
        double m_sod; //������,��λ����
        TimeSystem  m_ts;
    };
    
    
    /*����ʱ�䣬һ�����ܺ��������ʾ*/
    class NavTime
    {
        
    public:
        NavTime(): m_week(0),m_sow(0.0) {}
        ~NavTime() {}
        
        int					 m_week;     //�ܼ���
        double				 m_sow;   //������
        TimeSystem  m_ts;   //ʱ��ϵͳ��Ŀǰֻ��GPST��BDT��GALT���ܺ��������ʾ
    };
    
    
    class JDTime
    {
        
    public:
        JDTime(): m_jd(0),m_sod(0.0),m_fsod(0.0) {}
        ~JDTime() {}
        
        long m_jd;
        long m_sod;
        double m_fsod;
        TimeSystem m_ts;
    };
    
    
    
				
    class GTime
    {
        
    public:
        
        
        struct LeapType
        {
        public:
            int year;
            int month;
            int day;
            int nleap;
        };
        
        static std::list<  LeapType >  InitializeLeapSecTable();
        //���������¼���������
        static	void  AddLeapSecond( int year, int month ,int day, int leaps );
        /*��ȡ����ĺ���Ҫ�Ǿ�̬��*/
        static double getLeapSecond(  int year, int month,int day);
        
        static DOYTime CivilTime2DOYTime(CivilTime ct);
        static JDTime CivilTime2JDTime(CivilTime ct);
        static CivilTime JDTime2CivilTime(JDTime jdt);
        static GTime  JDTime2GTime(JDTime jdt);
        static JDTime GTime2JDTime(GTime gt);
        static GTime  CivilTime2GTime(CivilTime ct);
        
        void SetData(TimeSystem ts, long mjdp,long sodp, double fsodp);
        void GetData(TimeSystem& ts, long& mjdp,long& sodp, double& fsodp);
        
        void SetFromCivilTime(CivilTime ct);
        void SetFromDoyTime(DOYTime dt);
        void SetFromNavTime(NavTime nt);
        
        //���������
        GTime  operator- ( GTime& right);
        GTime  operator- (const double& nanosecond);
        GTime  operator+ ( GTime& right);
        GTime  operator+ (const double& nanosecond);
        GTime& operator= (const GTime& right);  //��ֵ����
        bool   operator== (const GTime& right);
        bool   operator!= (const GTime& right);
        bool   operator>  ( GTime& right);
        bool   operator<  ( GTime& right);
        bool   operator>= ( GTime& right);
        bool   operator<= ( GTime& right);
        
        
        TimeSystem getTimeSystem() const	;
        
        virtual ~GTime() ;
        GTime() ;
        GTime(const GTime& time); 
        
    private:	
        static  double eps;   
        static std::list< struct LeapType > LeapTable;
        
        TimeSystem    m_ts;  //ʱ��ϵͳ�Ķ���
        long 		m_mjd;   //Modified Julian Day
        long		    m_sod;  //�����������(��������) 0 -> 86399
        double		m_fsod;  //���С������(��λ����) 0->10^9;double�ľ���ֻ��С�����6λ
        
    };
    
}  //end of namespace gfc


#endif