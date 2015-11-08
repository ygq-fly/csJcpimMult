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
//#define OFFSET_TX2 2
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

//Ƶ����Ϣ�� ������ʼ�����ݿ�
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
//huaweiƵ��
#define huawei_sql_body { \
	"'hw1', 'LTE700',     728,   746,   698,   716,  0, 0,   '111',   1,   2",  \
	"'hw2', 'DD800',      791,   821,   832,   862,  0, 0,   '111',   3,   4",  \
	"'hw3', 'EGSM900',    925,   960,   880,   915,  0, 0,   '111',   5,   6",  \
	"'hw4', 'DCS1800',    1805,  1880,  1710,  1785, 0, 0,   '111',   7,   8",  \
	"'hw5', 'PCS1900',    1930,  1990,  1850,  1910, 0, 0,   '111',   9,   10",  \
	"'hw6', 'WCDMA2100',  2110,  2170,  1920,  2060, 0, 0,   '111',   11,  12",  \
	"'hw7', 'LTE2600',    2620,  2690,  2500,  2570, 0, 0,   '111',   13,  14"  \
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

class JcOffsetDB
{
public:
    JcOffsetDB()
		: m_pConn(NULL)
		, m_bConn(false)
		, m_band_info_table("JC_BAND2_INFO")
		, m_tx_offset_table("JC_TX_OFFSET_ALL")
		, m_rx_offset_table("JC_RX_OFFSET_ALL")
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
	//�������ݿ�
	bool DbConnect(const char* addr);
	//��ʼ�����ݿ⣬������
	void DbInit(uint8_t mode);
	bool DbSetTxIncremental(const char* band, const char& dut, const char& coup, const char& real_or_dsp, double incremental);
	int GetBandCount(const char* band_mode);
	int GetBandInfo(const char* prefix, char* band_info);
	//�������������е�Ƶ��
	int FreqBand_continuous(const uint8_t& tx_or_rx, const char* band, double& f_start, double& f_stop);
	//���ҹ̶������е�У׼��ʽ
	int FreqBand_discontinuous(const uint8_t& tx_or_rx, const double& freq_mhz, const char* band,
								double &f1, double &f2, double &index1, double &index2);
	//��ȡУ׼Ƶ�ʵ㼯��
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

	//ʹ��ģʽ����ϵ��Ƶ�α�ĵ���
	uint8_t m_offset_mode;
};
#endif /* defined(__MyTest__JcOffsetDB__) */
