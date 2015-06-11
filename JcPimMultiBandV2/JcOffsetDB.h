//
//  JcOffsetDB.h
//  MyTest
//
//  Created by San on 14/12/4.
//  Copyright (c) 2014�� ___SAN___. All rights reserved.
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
	//У׼��������
	continuous_offset_mode = 0,
	//У׼����������
	discontinuous_offset_mode = 1
};

#define sql_header "insert into [JC_BAND_INFO] (band,tx_enable,coup1,coup2,tx_start,tx_end,rx_start,rx_end,vco_a,vco_b) select "
//poiƵ��
#define poi_sql_body { \
	"'1Cmgsm',    12,  1,   2,    925,   960,   880,   915,  0, 0",  \
	"'2Cucdma',   12,  1,   2,    865,   894,   820,   849,  0, 0",  \
	"'3Ctfd18',   12,  3,   4,    1805,  1880,  1710,  1785, 0, 0",  \
	"'4Cufd18',   12,  3,   4,    1805,  1880,  1710,  1785, 0, 0",  \
	"'5Ctfd21',   12,  5,   6,    2110,  2170,  1920,  2060, 0, 0",  \
	"'6Cuw21',    12,  5,   6,    2110,  2170,  1920,  2060, 0, 0",  \
	"'7Cmdcs',    12,  7,   8,    1805,  1880,  1710,  1785, 0, 0",  \
	"'8Cmtdf',    12,  7,   8,    1885,  1915,  1885,  1915, 0, 0",  \
	"'9Cmtda',    1,   -1,  9,    2010,  2025,  0,     0,    0, 0",  \
	"'10Cmtde',   12,  10,  11,   2300,  2390,  2300,  2390, 0, 0",  \
	"'11Cttde',   2,   10,  -1,   2300,  2390,  2300,  2390, 0, 0",  \
	"'12Cutde',   12,  -1,  11,   2300,  2390,  0,     0,    0, 0"  \
				}
//huaweiƵ��
#define huawei_sql_body { \
	"'LTE700',    12,   1,   2,   728,   746,   698,   716,  0, 0",  \
	"'DD800',     12,   3,   4,   791,   821,   832,   862,  0, 0",  \
	"'EGSM900',   12,   5,   6,   925,   960,   880,   915,  0, 0",  \
	"'DCS1800',   12,   7,   8,   1805,  1880,  1710,  1785, 0, 0",  \
	"'PCS1900',   12,   9,   10,  1930,  1990,  1850,  1910, 0, 0",  \
	"'WCDMA2100', 12,   11,  12,  2110,  2170,  1920,  2060, 0, 0",  \
	"'LTE2600',   12,   13,  14,  2620,  2690,  2500,  2570, 0, 0"  \
				}

class JcOffsetDB
{
public:
    JcOffsetDB()
		:_pConn(NULL)
		,_bConn(false)
		,_sTxFreqtable("TX_FREQ_LIST")
		, _offset_mode(discontinuous_offset_mode)
    {
        //char col_types[][10] = { "" ,"INTEGER", "FLOAT", "Text", "BLOB", "NULL"};
    }
    
    ~JcOffsetDB()
	{
        if (_pConn) {
            sqlite3_close(_pConn);
        }
    }
    
public:
	bool DbConnect(const char* addr);
	void DbInit(uint8_t mode);
	int FreqBand(const uint8_t& tx_or_rx, const char* band, double& f_start, double& f_stop);
	int FreqBand_Rx();
	int JcOffsetDB::FreqBand_Old(const uint8_t& tx_or_rx, const double& freq_mhz, const char* band,
								double &f1, double &f2, double &index1, double &index2);
    //��ȡУ׼ͷ��Ϣ
	int FreqHeader(const char& tx_or_rx, const char* band, double* freq, int maxnum);
    //��ȡTxУ׼����
	double OffsetTx(const char* band, const char& dut, const char& coup,
					const char& real_or_dsp,
					const double& freq_mhz, const double& tx_dbm);
	//��ȡRxУ׼����
	double OffsetRx(const char* band, const char& dut, const double& freq_now);
	//��ȡvco����
	double OffsetVco(const char* band, const char& dut);
	//�洢У׼����
	int Store_v2(const char& tx_or_rx,
				 const char* band, const char& dut, const char& coup,
				 const char& real_or_dsp,
				 const double tx,
				 const double* val, int num);
	//�洢vcoУ׼����
	int Store_vco_single(const char* band, const char& dut, const double val);

private:
    //����б��
	double SumSlope(double v, double x1, double y1, double x2, double y2);
    //sqlite���ִ��
	int GetSqlVal(const char* strsql, double& a1, double& a2);
    
private:
    sqlite3* _pConn;
    bool _bConn;
    std::string _sTxFreqtable;

	//ʹ��ģʽ����ϵ��Ƶ�α��ĵ���
	uint8_t _offset_mode;

	//Ƶ�α�sql���
	std::string _hw_band_table_sql;
	std::string _poi_band_table_sql;
};
#endif /* defined(__MyTest__JcOffsetDB__) */