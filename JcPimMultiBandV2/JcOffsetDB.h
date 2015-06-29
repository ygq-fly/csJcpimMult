//
//  JcOffsetDB.h
//  MyTest
//
//  Created by San on 14/12/4.
//  Copyright (c) 2014年 ___SAN___. All rights reserved.
//

#ifndef __SAN__JcOffsetDB__
#define __SAN__JcOffsetDB__

#include "stdafx.h"
#include "3rdParty/sqlite3.h"

#define JCOFFSET_ERROR -10000
#define OFFSET_TX 0
#define OFFSET_RX 1
#define OFFSET_REAL 0
#define OFFSET_DSP 1
//#define JC_SQL_DEBUG

enum 
{
	//校准点数连续
	continuous_offset_mode = 0,
	//校准点数非连续
	discontinuous_offset_mode = 1
};

#define sql_header "insert into [JC_BAND_INFO] (band,tx_enable,coup1,coup2,tx_start,tx_end,rx_start,rx_end,vco_a,vco_b) select "
//poi频段
#define poi_sql_body { \
	"'1Cmgsm',    111,  1,   2,    925,   960,   880,   915,  0, 0",  \
	"'2Cucdma',   111,  1,   2,    865,   894,   820,   849,  0, 0",  \
	"'3Ctfd18',   111,  3,   4,    1805,  1880,  1710,  1785, 0, 0",  \
	"'4Cufd18',   111,  3,   4,    1805,  1880,  1710,  1785, 0, 0",  \
	"'5Ctfd21',   111,  5,   6,    2110,  2170,  1920,  2060, 0, 0",  \
	"'6Cuw21',    111,  5,   6,    2110,  2170,  1920,  2060, 0, 0",  \
	"'7Cmdcs',    111,  7,   8,    1805,  1880,  1710,  1785, 0, 0",  \
	"'8Cmtdf',    111,  7,   8,    1885,  1915,  1885,  1915, 0, 0",  \
	"'9Cmtda',    010,  -1,  9,    2010,  2025,  0,     0,    0, 0",  \
	"'10Cmtde',   111,  10,  11,   2300,  2390,  2300,  2390, 0, 0",  \
	"'11Cttde',   101,  10,  -1,   2300,  2390,  2300,  2390, 0, 0",  \
	"'12Cutde',   010,  -1,  11,   2300,  2390,  0,     0,    0, 0"  \
				}
//huawei频段
#define huawei_sql_body { \
	"'LTE700',    111,   1,   2,   728,   746,   698,   716,  0, 0",  \
	"'DD800',     111,   3,   4,   791,   821,   832,   862,  0, 0",  \
	"'EGSM900',   111,   5,   6,   925,   960,   880,   915,  0, 0",  \
	"'DCS1800',   111,   7,   8,   1805,  1880,  1710,  1785, 0, 0",  \
	"'PCS1900',   111,   9,   10,  1930,  1990,  1850,  1910, 0, 0",  \
	"'WCDMA2100', 111,   11,  12,  2110,  2170,  1920,  2060, 0, 0",  \
	"'LTE2600',   111,   13,  14,  2620,  2690,  2500,  2570, 0, 0"  \
				}

class JcOffsetDB
{
public:
    JcOffsetDB()
		:m_pConn(NULL)
		,m_bConn(false)
		,m_sTxFreqtable("TX_FREQ_LIST")
		,m_offset_mode(discontinuous_offset_mode)
    {
        //char col_types[][10] = { "" ,"INTEGER", "FLOAT", "Text", "BLOB", "NULL"};
    }
    
    ~JcOffsetDB()
	{
        if (m_pConn) {
            sqlite3_close(m_pConn);
        }
    }
    
public:
	//连接数据库
	bool DbConnect(const char* addr);
	//初始化数据库，创建表
	void DbInit(uint8_t mode);
	//查找连续点序列的频段
	int FreqBand(const uint8_t& tx_or_rx, const char* band, double& f_start, double& f_stop);
	//查找固定点序列的校准方式
	int FreqBand_Old(const uint8_t& tx_or_rx, const double& freq_mhz, const char* band,
								double &f1, double &f2, double &index1, double &index2);
	//获取校准频率点集合
	int FreqHeader(const char& tx_or_rx, const char* band, double* freq, int maxnum);
    //获取Tx校准数据
	double OffsetTx(const char* band, const char& dut, const char& coup,
					const char& real_or_dsp,
					const double& freq_mhz, const double& tx_dbm);
	//获取Rx校准数据
	double OffsetRx(const char* band, const char& dut, const double& freq_now);
	//获取vco数据
	double OffsetVco(const char* band, const char& dut);
	//存储校准数据
	int Store_v2(const char& tx_or_rx,
				 const char* band, const char& dut, const char& coup,
				 const char& real_or_dsp,
				 const double tx,
				 const double* val, int num);
	//存储vco校准数据
	int Store_vco_single(const char* band, const char& dut, const double val);

private:
    //计算斜率
	double SumSlope(double v, double x1, double y1, double x2, double y2);
    //sqlite语句执行
	int GetSqlVal(const char* strsql, double& a1, double& a2);
    
private:
    sqlite3* m_pConn;
    bool m_bConn;
    std::string m_sTxFreqtable;

	//使用模式，关系到频段表的调用
	uint8_t m_offset_mode;

	//频段表sql语句
	std::string m_hw_band_table_sql;
	std::string m_poi_band_table_sql;
};
#endif /* defined(__MyTest__JcOffsetDB__) */
