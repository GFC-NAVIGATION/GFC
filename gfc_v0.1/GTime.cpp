
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


#include "GTime.h"

namespace gfc
{
    
    //��̬�����Ķ��弰��ʼ��
    //Ŀǰ���ڵ������ǣ���α�֤��ľ�̬�������̰߳�ȫ�ԣ��������ԣ���������
    //
    
    //��TimeSystem��صĳ�ʼ��
    std::list<GString> gfc::TimeSystem::timesystemTab  = gfc::TimeSystem::Initializer();
    std::list<GString> TimeSystem::Initializer()
    {
        std::list<GString>  tmpTab;
        tmpTab.push_back("tsUKN");
        tmpTab.push_back("tsGPS");
        tmpTab.push_back("tsBDS");
        tmpTab.push_back("tsGLO");
        tmpTab.push_back("tsUTC");
        tmpTab.push_back("tsTAI");
        tmpTab.push_back("tsTT");
        tmpTab.push_back("tsTTB");
        
        return tmpTab;
    }
    
    // register user defined timesystem
    void TimeSystem::RegByName( GString variableName )
    {
        // if variableName does not exist, then register , otherwise throw out exception
        int NumberOfTS(0) ;
        NumberOfTS = static_cast<int>(count(timesystemTab.begin(), timesystemTab.end(), variableName ));
        
        if( NumberOfTS == 0 )
        {
            timesystemTab.push_back(variableName);
        }
        else
        {
            printf("WARNING: TimeSystem : timesystem %s has already existed!\n",variableName.c_str());
        }
    }
    
    void TimeSystem::UnregByName( GString variableName )
    {
        std::list<GString>::iterator myIterator;
        myIterator = find(timesystemTab.begin(),timesystemTab.end(),variableName);
        if( myIterator == timesystemTab.end() )
        {
            printf("WARNING: TimeSystem : timesystem %s hasn't  existed!\n",variableName.c_str());
        }
        else
        {
            timesystemTab.erase(myIterator);
        }
    }
    
    
    
    TimeSystem TimeSystem::GetByName( GString variableName )
    {
        std::list<GString>::iterator myIterator;
        myIterator = find(timesystemTab.begin(),timesystemTab.end(),variableName);
        if( myIterator == timesystemTab.end() )
        {
            // throw out exception, timesystem does not exist
            //�Ƿ�����
            TimeSystemUnexist ir("TimeSystem unexist!");
            ir.addLocation(FILE_LOCATION);
            GFC_THROW( ir );
            
        }
        else
        {
            TimeSystem  myts(*myIterator);
            return myts;
        }
    }
    
    
    void TimeSystem::dump( std::ostream& s )
    {
        s<<"ClassName:    "<<"TimeSystem"<<std::endl;
        
        //�������еı���
        std::list<GString>::iterator myit = timesystemTab.begin();
        for(;myit!= timesystemTab.end(); ++myit)
        {
            s<<"TimeSystemName:  "<<*myit<<std::endl;
        }
    }
    
    
    
    
    //��GTime��صĳ�ʼ��
    std::numeric_limits<double> e;
    double gfc::GTime::eps = e.epsilon();
    
    std::list< GTime::LeapType  > gfc::GTime::LeapTable = gfc::GTime::InitializeLeapSecTable();
    
    std::list< GTime::LeapType >   GTime::InitializeLeapSecTable()
    {
        const  GTime::LeapType myleap[]=
        {
            { 1971,  12,	31,	10 },
            { 1972,  06,	30, 11 },
            { 1972,  12,	31,	12 },
            { 1973,  12,	31,	13 },
            { 1974,  12,	31,	14 },
            { 1975,  12,	31, 15 },
            { 1976,  12,	31,	16 },
            { 1977,  12,	31,	17 },
            { 1978,  12,	31,	18 },
            { 1979,  12,	31,	19 },
            { 1981,  06,	30,	20 },
            { 1982,  06,	30,	21 },
            { 1983,  06,	30,	22 },
            { 1985,  06,	30,	23 },
            { 1987,  12,	31,	24 },
            { 1989,  12,	31,	25 },
            { 1990,  12,	31,	26 },
            { 1992,  06,	30,	27 },
            { 1993,  06,	30,	28 },
            { 1994,  06,	30,	29 },
            { 1995,  12,	31,	30 },
            { 1997,  06,	30,	31 },
            { 1998,  12,	31,	32 },
            { 2005,  12,	31,	33 },
            { 2008,  12,	31,	34 },
            { 2012,  06,	30,	35 },
            { 2015,  06,	30,	36 }
            
        };
        
        int_t  leaptypesize  = sizeof(myleap)/sizeof(LeapType);
        
        std::list<  GTime::LeapType >  tmpLeap(myleap,myleap+leaptypesize);
        
        return tmpLeap;
    }
    
    
    void  GTime::AddLeapSecond( int year, int month , int day ,int leaps )
    {
        GTime::LeapType  mytype;
        mytype.year = year;
        mytype.month = month;
        mytype.day   = day;
        mytype.nleap = leaps;
        
        //��push_back֮ǰ��Ҫ����¸������¼�Ƿ����ظ�
        
        LeapTable.push_back(mytype);
    }
    
    
    /*��ȡ����ĺ���Ҫ�Ǿ�̬��*/
    double GTime::getLeapSecond( int year, int month, int day)
    {
        double leapsec = 0.0;
        if( year < 1972 )  //1972����ǰ�����벻����
        {
            InvalidTime e("Class:GTime, year should be larger than 1972.");
            e.addLocation(FILE_LOCATION);
            std::cout<<e<<std::endl;
            GFC_THROW(e);
        }
        
        
        std::list<LeapType>::iterator  myit ;
        for( myit = LeapTable.begin(); myit != LeapTable.end(); ++myit )
        {
            
            if( myit->year > year || ( myit->year == year && myit->month > month)
               || ( myit->year == year && myit->month == month && myit->day > day )
              )
                
            {
                continue;
            }
            
            leapsec = double( myit->nleap);
            break;
        }
        
        return leapsec;
    }
    
    //���캯��
    GTime::GTime()
    {
        m_mjd = 0.0; m_sod = 0.0; m_fsod = 0.0;
    }
    
    /*�������캯��
     ��̬��Ա������Ҫ���������𣿣���
     */
    GTime::GTime( const GTime& time )
    {
        this->m_ts = time.m_ts;
        this->m_mjd = time.m_mjd;
        this->m_sod = time.m_sod;
        this->m_fsod = time.m_fsod;
    }
    
    
    GTime::~GTime()  {}
    
    TimeSystem GTime::getTimeSystem() const
    { return m_ts; }
    
    
    /*�㷨�ο��� GPS Theory and Practice , B.Hofmann-Wellenhof,H.Lichtenegger, J.Collins
     fifth, revised edition
     */
    JDTime GTime::CivilTime2JDTime(CivilTime ct)
    {
        long_t MINPHOR = GCONST("MINPHOR");
        long_t SECPHOR = GCONST("SECPHOR");
        long_t HORPDAY = GCONST("HORPDAY");
        long_t SECPMIN = GCONST("SECPMIN");
        
        JDTime jdt;
        jdt.m_ts = ct.m_ts;
        
        int y = 0 , m = 0;
        y = ct.m_year;m = ct.m_month;
        if( ct.m_month <= 2 )
        {
            y = y-1; m = m+12;
        }
        
        double	ut = ct.m_hour + ct.m_minute/MINPHOR+ ct.m_second/SECPHOR;
        long double tmpjd = static_cast<long>(365.25*y) + static_cast<long>(30.6001*(m+1))+ ct.m_day + ut/HORPDAY + 1720981.5;
        
        jdt.m_jd = static_cast<long>(tmpjd); 
        
        jdt.m_sod = ( (ct.m_hour + 12)%HORPDAY )*SECPHOR + ct.m_minute*SECPMIN	+ static_cast<long>(ct.m_second);
        
        jdt.m_fsod = ct.m_second - static_cast<long>(ct.m_second) ;
        
        return jdt;
    }
    
    void GTime::SetData(TimeSystem tsp, long mjdp,long sodp, double fsodp)
    {
        //Ϊ�˱�֤���ݵ���ȷ�ԣ����뱣֤sodp<86400;��fsodp < 1.0E9
        if( sodp > GCONST("SECPDAY") && fsodp > GCONST("NANO") )
        {
            //�Ƿ�����
            InvalidRequest ir("Time Parameters out of range!");
            GFC_THROW( ir );
        }
        
        m_ts  = tsp;
        m_mjd = mjdp;
        m_sod = sodp;
        m_fsod = fsodp;
    }
    
    void GTime::GetData(TimeSystem& ts, long& mjdp,long& sodp, double& fsodp)
    {
        ts = m_ts;
        mjdp = m_mjd;
        sodp = m_sod;
        fsodp = m_fsod;
    }
    
    
    /*������ ת��Ϊ  GTime*/
    GTime GTime::CivilTime2GTime( CivilTime ct )
    {
        JDTime jd = CivilTime2JDTime(ct);
        GTime  gt = JDTime2GTime(jd);
        return gt;
    }
    
    /*������ ת��Ϊ DOYT */
    DOYTime GTime::CivilTime2DOYTime( CivilTime ct )
    {
        DOYTime dt;
        dt.m_ts = ct.m_ts;
        CivilTime ct1; ct1.m_ts = ct.m_ts; ct1.m_year = ct.m_year ; ct1.m_month = 1; ct1.m_day = 1;
        GTime gt1 = CivilTime2GTime(ct);
        GTime gt2 = CivilTime2GTime(ct1);  //��ǰ��ĵ�һ��
        GTime gt = gt1 - gt2 ;             //��Ҫ���ؼ�������
        //��Ҫ��GTime gtת��ΪDOYTime
        
        
        return dt;
    }
    
    
    /*��������ת��Ϊ��������*/
    GTime GTime::JDTime2GTime( JDTime jdt )
    {
        GTime  gt;
        
        double mjd0 = GCONST("MJD0");
        long_t SECPDAY = GCONST("SECPDAY");
        long_t NANO = GCONST("NANO");
        double tmpjd = jdt.m_jd;
        double tmpsod = jdt.m_sod;
        if( jdt.m_sod+jdt.m_fsod - 0.5*SECPDAY < 0   )
        {
            tmpsod  =  jdt.m_sod + SECPDAY ; //������ı仯
            tmpjd   =  jdt.m_jd - 1 ;  //��ʱ jd�����С��
        }
        //
        tmpsod = tmpsod - 0.5*SECPDAY;
        tmpjd =  tmpjd - mjd0+0.5;
        
        double tmpfsod = jdt.m_fsod*NANO;
        
        gt.SetData(jdt.m_ts,tmpjd,tmpsod,tmpfsod);
        
        return gt;
        
    }
    
    //MJD2JD
    JDTime GTime::GTime2JDTime( GTime gt )
    {
        long_t SECPDAY =GCONST("SECPDAY");
        
        JDTime  jdt;
        long mjdp = 0, sodp = 0;
        double fsodp = 0.0, mjd0 = 0.0;
        mjd0 = GCONST("MJD0");
        gt.GetData(jdt.m_ts,mjdp,sodp,fsodp);
        double nano = GCONST("NANO");
        double tmpsec = sodp + fsodp/nano;  //����Ϊ��λ��������
        if( tmpsec >= 0.5*SECPDAY )  //����ڰ���
        {
            mjdp = mjdp + mjd0 + 0.5;  //��Ϊ����
            sodp = sodp - 0.5*SECPDAY;
        }
        else
        {
            mjdp = mjdp + mjd0-0.5;
            sodp = sodp + 0.5*SECPDAY;
        }
        
        jdt.m_jd = mjdp;
        jdt.m_sod = sodp;
        jdt.m_fsod = fsodp/nano;
        
        return jdt;
    }
    
    /*
     ��̬���� JDT2CivilTime
     �㷨�ο��� GPS Theory and Practice , B.Hofmann-Wellenhof,H.Lichtenegger, J.Collins
     fifth, revised edition
     */
    CivilTime GTime::JDTime2CivilTime( JDTime jdt )
    {
        
        long_t  SECPDAY = GCONST("SECPDAY");
        long_t  SECPHOR = GCONST("SECPHOR");
        long_t  SECPMIN = GCONST("SECPMIN");
        
        CivilTime ct;
        ct.m_ts = jdt.m_ts;  //ʱ��ϵͳ��ֵ
        long a = 0, b = 0 , c = 0 , d = 0 , e = 0;
        
        double fracJd = jdt.m_jd  + (jdt.m_sod + jdt.m_fsod)/SECPDAY;
        
        a = static_cast<long>( fracJd + 0.5 );
        b = a+ 1537;
        c = static_cast<long>( ( b - 122.1 )/365.25 );
        d = static_cast<long>( 365.25 * c );
        e = static_cast<long>( ( b-d )/30.6001 );
        
        
        ct.m_month = static_cast<int_t>(e -1 -12 * static_cast<long>( e/14 )) ;
        ct.m_year  = static_cast<int_t>(c - 4715 - static_cast<long>( ( 7 + ct.m_month )/10 ));
        ct.m_day  = static_cast<int_t>( b - d -static_cast<long>( 30.6001 * e ));
        
        //��jdt����0.5��
        if( jdt.m_sod >= static_cast<long>(0.5*SECPDAY) )
        {
            jdt.m_jd = jdt.m_jd  + 1;
            jdt.m_sod = jdt.m_sod - 0.5*SECPDAY;
        }
        else
        {
            jdt.m_sod = jdt.m_sod + static_cast<long>(0.5*SECPDAY);
        }
        
        ct.m_hour  =  static_cast<int_t>( (jdt.m_sod + jdt.m_fsod)/SECPDAY );
        
        ct.m_minute = static_cast<int_t>( ( jdt.m_sod + jdt.m_fsod - ct.m_hour*SECPHOR )/SECPMIN );
        
        ct.m_second = jdt.m_sod - ct.m_hour*SECPHOR - ct.m_minute*SECPMIN + jdt.m_fsod;
        
        return ct;
        
    }
    
    /*��ֵ���غ���*/
    GTime& GTime::operator =( const GTime& right )
    {
        this->m_ts = right.m_ts;
        this->m_mjd = right.m_mjd;
        this->m_sod = right.m_sod;
        this->m_fsod = right.m_fsod;
        return *this;
    }
    
    /*�ӷ�����*/
    GTime GTime::operator+ (  GTime& right )
    {
        long_t NANO = GCONST("NANO");
        long_t SECPDAY = GCONST("SECPDAY");
        GTime  gt;
        if( ( m_ts != GTimeSystem("tsUKN") &&
             right.m_ts != GTimeSystem("tsUKN")) &&
           m_ts != right.m_ts )
        {
            //�Ƿ�����
            InvalidRequest ir("GTime objects not in same time system, cannot be differenced");
            GFC_THROW( ir );
        }
        
        gt.m_fsod = this->m_fsod + right.m_fsod;
        if( gt.m_fsod >= NANO )
        {
            gt.m_fsod = gt.m_fsod - NANO;
            gt.m_sod = gt.m_sod + 1;
        }
        
        gt.m_sod = gt.m_sod + ( this->m_sod + right.m_sod );
        if(gt.m_sod >= SECPDAY )
        {
            gt.m_sod = gt.m_sod - SECPDAY;
            gt.m_mjd = gt.m_mjd + 1;
        }
        
        gt.m_mjd = gt.m_mjd + ( this->m_mjd + right.m_mjd );
        
        gt.m_ts = m_ts;
        return gt;
    }
    
    GTime GTime::operator+ ( const double& nanosecond )
    {
        long_t NANO = GCONST("NANO");
        long_t SECPDAY = GCONST("SECPDAY");
        
        GTime gt;
        long tmjd = m_mjd;
        long tsod = m_sod;
        double tfsod = m_fsod;
        long temp1 = static_cast<long> (nanosecond) % static_cast<long>(NANO);
        tfsod = nanosecond - temp1*NANO;
        long temp2 = static_cast<long>(temp1) % static_cast<long>(SECPDAY);
        tsod = temp1 - temp2*static_cast<long>(SECPDAY);
        tmjd = tmjd + temp2;
        gt.SetData(m_ts,tmjd,tsod,tfsod);
        return gt;
        
    }
    
    /*��������*/
    GTime GTime::operator -(  GTime& right )
    {
        long_t NANO = GCONST("NANO");
        long_t SECPDAY = GCONST("SECPDAY");
        
        GTime  gt;
        if( ( m_ts!= GTimeSystem("tsUKN") &&
             right.m_ts != GTimeSystem("tsUKN")) &&
           m_ts != right.m_ts )
        {
            //�Ƿ�����
            InvalidRequest ir("GTime objects not in same time system, cannot be differenced");
            GFC_THROW( ir );
        }
        
        //ģ���ֶ���λ������̣���һλ���Ա�֤�������ȷ��
        if( this->m_fsod < right.m_fsod )
        {
            this->m_sod = this->m_sod -1;
            this->m_fsod = this->m_fsod + NANO;
        }
        
        gt.m_fsod = this->m_fsod - right.m_fsod;
        //fsod�ļ������
        if( this->m_sod < right.m_sod )
        {
            this->m_mjd = this->m_mjd - 1;
            this->m_sod = this->m_sod + SECPDAY;
        }
        
        gt.m_sod = this->m_sod - right.m_sod;
        //��ļ������
        //��ʼ������
        gt.m_mjd = this->m_mjd - right.m_mjd ;
        gt.m_ts = m_ts;
        
        return gt;
        
    }
    
    /*��������*/
    GTime GTime::operator -( const double& nanosecond )
    {
        long_t NANO = GCONST("NANO");
        long_t SECPDAY = GCONST("SECPDAY");
        
        GTime gt;
        gt.m_ts = m_ts;
        double tfsod = this->m_fsod;
        long tsod = this->m_sod;
        long tmjd = this->m_mjd;
        while(  tfsod - nanosecond < 0.0 )
        {
            tsod = tsod - 1;
            tfsod = tfsod + NANO;
            if( tsod < 0 )
            {
                tmjd = tmjd - 1;
                tsod = tsod + SECPDAY;
            }
        }
        
        tfsod = tfsod - nanosecond;  //����������
        
        //��ʼ�����ݽ�������(tmjd, tsod, tfsod)
        gt.SetData(m_ts,tmjd,tsod,tfsod);
        
        return gt;
    }
    
    /*�Ⱥ�����*/
    bool GTime::operator== ( const GTime& right )
    {
        if( this->m_ts == right.m_ts
           && this->m_mjd == right.m_mjd
           && this->m_sod == right.m_sod
           && fabs(this->m_fsod - right.m_fsod)<GTime::eps )
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    
    /*���Ⱥ�����*/
    bool GTime::operator!= ( const GTime& right )
    {
        bool t = (*this)==right;
        return !t;
    }
    
    /*
     ���ں�����
     ͬһ��ʱ��ϵͳ��ʱ����ܱȽϴ�С
     */
    bool GTime::operator> (  GTime& right )
    {
        if(  m_ts != GTimeSystem("tsUKN")
           &&right.m_ts != GTimeSystem("tsUKN")
           &&m_ts != right.m_ts
           )
        {
            //�Ƿ�����
            InvalidRequest ir("GTime objects not in same time system, cannot be differenced");
            GFC_THROW( ir );
        }	
        
        if(    this->m_mjd > right.m_mjd
           ||(this->m_mjd== right.m_mjd && this->m_sod > right.m_sod )
           ||(this->m_mjd== right.m_mjd && this->m_sod == right.m_sod && this->m_fsod-right.m_fsod > eps )
           )
        {
            return true;
        }
        else
        {
            return false;
        }		
    }
    
    bool GTime::operator< (  GTime& right )
    {
        bool t = (*this)>right;
        return !t;
    }
    
    /**/
    bool GTime::operator<= (  GTime& right )
    {
        bool t = ( (*this)<right || (*this)==right ); 
        return t;
    }
    
    bool GTime::operator>= ( GTime& right )
    {
        bool t = ( (*this)>right || (*this)==right ); 
        return t;
    }
    
    /*
     ��������ת������
     ��double[6]��TimeSystem����ʱ����ʽת��
     �ο���gpstk�к��� convertCalendarToJD
     */
    void GTime::SetFromCivilTime(  CivilTime ct )
    {
        JDTime  jdt = CivilTime2JDTime(ct);
        *this = JDTime2GTime(jdt);
    }
    
    /*����������ת��������ֻ����ʱ����ʽ��ת��*/	
    void GTime::SetFromDoyTime( DOYTime dt )
    {
        m_ts = dt.m_ts;
        //������
        
        
    }
    
    void GTime::SetFromNavTime(NavTime nt)
    {
        
        m_ts = nt.m_ts;   //ʱ��ϵͳ��ֵ
        int secpday = GCONST("SECPDAY");
        int daypwek = GCONST("DAYPWEK");
        long_t  NANO = GCONST("NANO");
        
        int  deltad = 0;
        int  dow = 0;
        while ( nt.m_sow >= secpday )
        {
            nt.m_sow-=secpday;
            dow++;
        }
        while ( nt.m_sow <  0 )
        {
            nt.m_sow+=secpday;
            dow--;
        }
        
        deltad = nt.m_week*daypwek + dow;
        
        double T0 = 0.0;
        if( nt.m_ts == GTimeSystem("tsGPS"))
        {
            T0 = 44244.0;
        }
        else if( nt.m_ts == GTimeSystem("tsBDS"))
        {
            //��ȷ��??
            T0 = 53736.0;
        }
        else if(nt.m_ts  == GTimeSystem("tsGAL"))
        {
            T0 = 51412.0;    /* 1999,8,22,0,0,0 */
        }
        
        
        m_mjd  = static_cast<long>( T0 + deltad );   //�����44244��GPST0ʱ������Ӧ��MJDֵ����ͬϵͳ��һ��
        m_sod  = static_cast<long>(nt.m_sow);
        m_fsod = static_cast<double>((nt.m_sow - m_sod)*NANO);   //���С�����ֵĵ�λ������
        
    } // end of function SetFromNavTime(NavTime nt)
    
    
    
    
    
    
    
    
    
}  // end of namespace
