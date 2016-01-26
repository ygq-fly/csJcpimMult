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
#define VERSION_BAND "2.1"
//#define JC_SQL_DEBUG

enum 
{
	//校准点数连续
	continuous_offset_mode = 0,
	//校准点数非连续
	discontinuous_offset_mode = 1
};

//频段信息， 构建初始化数据库
#define sql_table "CREATE  TABLE [JC_BAND2_INFO] (prefix TEXT PRIMARY KEY  NOT NULL , band TEXT, tx_start REAL, tx_end REAL, rx_start REAL, rx_end REAL, vco_a REAL, vco_b REAL, tx_enable TEXT, coup1 INTEGER, coup2 INTEGER)"
#define sql_header "insert into [JC_BAND2_INFO] (prefix,band,tx_start,tx_end,rx_start,rx_end,vco_a,vco_b,tx_enable,coup1,coup2) select "
#define poi_sql_body { \
	"'poi1', '1Cmgsm',     925,   960,   880,   915,  0, 0,   '111',  1,   2",  \
	"'poi2', '2Cucdma',    865,   894,   820,   849,  0, 0,   '111',  1,   2",  \
	"'poi3', '3Ctfd18',    1805,  1880,  1710,  1785, 0, 0,   '111',  3,   4",  \
	"'poi4', '4Cufd18',    1805,  1880,  1710,  1785, 0, 0,   '111',  3,   4",  \
	"'poi5', '5Ctfd21',    2110,  2170,  1920,  2060, 0, 0,   '111',  5,   6",  \
	"'poi6', '6Cuw21',     2110,  2170,  1920,  2060, 0, 0,   '111',  5,   6",  \
	"'poi7', '7Cmdcs',     1805,  1880,  1710,  1785, 0, 0,   '111',  7,   8",  \
	"'poi8', '8Cmtdf',     1885,  1915,  1885,  1915, 0, 0,   '111',  7,   8",  \
	"'poi9', '9Cmtda',     2010,  2025,  0,     0,    0, 0,   '010',  -1,  9",  \
	"'poi10', '10Cmtde',   2300,  2390,  2300,  2390, 0, 0,   '111',  10,  11",  \
	"'poi11', '11Cttde',   2300,  2390,  2300,  2390, 0, 0,   '101',  10,  -1",  \
	"'poi12', '12Cutde',   2300,  2390,  0,     0,    0, 0,   '010',  -1,  11"  \
				}
//huawei频段
#define huawei_sql_body { \
	"'hw1', 'LTE700',     728,   746,   698,   716,    1334,    1336,   '111',   1,   2",  \
	"'hw2', 'DD800',      791,   821,   832,   862,    1338,    1340,   '111',   3,   4",  \
	"'hw3', 'EGSM900',    925,   960,   880,   915,    1342,    1344,   '111',   5,   6",  \
	"'hw4', 'DCS1800',    1805,  1880,  1710,  1785,   1346,    1348,   '111',   7,   8",  \
	"'hw5', 'PCS1900',    1930,  1990,  1850,  1910,   1350,    1352,   '111',   9,   10",  \
	"'hw6', 'WCDMA2100',  2110,  2170,  1920,  2060,   1354,    1356,   '111',   11,  12",  \
	"'hw7', 'LTE2600',    2620,  2690,  2500,  2570,   1358,    1360,   '111',   13,  14",  \
	"'hw8', 'PDC1500',    1475,  1512,  1405,  1464,   1366,    1368,   '111',   15,  16"  \
					}

#define NewPoi_sql_body { \
	"'np1', '450f1',      460,     465.75,  450,    455.75,  0, 0,   '111',   1,   2",  \
	"'np2', '450f2',      462.5,   467.5,   452.5,  457.5,   0, 0,   '111',   3,   4",  \
	"'np3', '700l',       728,     746,     698,    712,     0, 0,   '111',   5,   6",  \
	"'np4', '700h',       746,     768,     777,    798,     0, 0,   '111',   7,   8",  \
	"'np5', '700apt',     758,     803,     703,    748,     0, 0,   '111',   9,   10",  \
	"'np6', 'dd800',      791,     821,     832,    862,     0, 0,   '111',   11,  12",  \
	"'np7', 'cdma800',    865,     894,     820,    849,     0, 0,   '111',   13,  14",  \
	"'np8', 'gsm900',     925,     960,     880,    915,     0, 0,   '111',   15,   16",  \
	"'np9', 'dcs1800',    1805,    1880,    1710,   1785,    0, 0,   '111',   17,   18",  \
	"'np10', 'pcs1900',   1930,    1990,    1850,   1910,    0, 0,   '111',   19,   20",  \
	"'np11', 'wcdma2100', 2110,    2170,    1920,   2060,    0, 0,   '111',   21,   22",  \
	"'np12', 'wcdma2600', 2620,    2690,    2500,   2570,    0, 0,   '111',   23,   24"  \
					}

#define NewHuawei_sql_body { \
	"'nhw1', '800a',      862,      869,      848,     857,    1362,  1364,   '111',   1,   2",  \
	"'nhw2', '800b',      862,      869,      848,     857,    1362,  1364,   '111',   3,   4",  \
	"'nhw3', '1900a',     1930,     1990,     1850,    1910,   1350,  1352,   '111',   5,   6",  \
	"'nhw4', '1900b',     1930,     1990,     1850,    1990,   1350,  1352,   '111',   7,   8",  \
	"'nhw5', '2600a',     2620,     2690,     2500,    2580,   1358,  1360,   '111',   9,   10",  \
	"'nhw6', '2600b',     2620,     2690,     2500,    2580,   1358,  1360,   '111',   11,  12",  \
	"'nhw7', '2600c',     2620,     2690,     2500,    2580,   1358,  1360,   '111',   13,  14",  \
	"'nhw8', '2600d',     2620,     2690,     2500,    2580,   1358,  1360,   '111',   15,   16"  \
					}

class JcOffsetDB
{
public:
	JcOffsetDB()
		: m_pConn(NULL)
		, m_bConn(false)
		, m_band_info_table("JC_BAND2_INFO")
		, m_tx_offset_table("JC_TX_OFFSET_ALL")
		, m_rx_offset_table("JC_RX_OFFSET_ALL")
		, m_setting_table("JC_SETTING_INFO")
		, m_offset_mode(discontinuous_offset_mode)
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
	bool DbSetTxIncremental(const char* band, const char& dut, const char& coup, const char& real_or_dsp, double incremental);
	int GetBandCount(const char* band_mode);
	int GetBandInfo(const char* prefix, char* band_info);
	//查找连续点序列的频段
	int FreqBand_continuous(const uint8_t& tx_or_rx, const char* band, double& f_start, double& f_stop);
	//查找固定点序列的校准方式
	int FreqBand_discontinuous(const uint8_t& tx_or_rx, const double& freq_mhz, const char* band,
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
	bool IsExist(const char* table);
	bool ExecSql(const char* sql);
private:
    sqlite3* m_pConn;
    bool m_bConn;
	std::string m_band_info_table;
	std::string m_tx_offset_table;
	std::string m_rx_offset_table;
	std::string m_vco_offset_table;
	std::string m_setting_table;

	//使用模式，关系到频段表的调用
	uint8_t m_offset_mode;
};
#endif /* defined(__MyTest__JcOffsetDB__) */
