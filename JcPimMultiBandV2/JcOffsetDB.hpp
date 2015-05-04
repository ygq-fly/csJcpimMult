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

#define JCOFFSET_ERROR -10000

#define OFFSET_TX 0
#define OFFSET_RX 1
#define OFFSET_REAL 0
#define OFFSET_DSP 1

//#define JC_SQL_DEBUG

class JcOffsetDB
{
public:
    JcOffsetDB()
    :_pConn(NULL),
    _bConn(false),
    _sTxFreqtable("TX_FREQ_LIST")
    {
        char col_types[][10] = { "" ,"INTEGER", "FLOAT", "Text", "BLOB", "NULL"};
    }
    
    ~JcOffsetDB(){
        if (_pConn) {
            sqlite3_close(_pConn);
        }
    }
    
public:
    bool Dbconnect(const char* addr) {
        _bConn = !sqlite3_open(addr, &_pConn);
        return _bConn;
    }
    
    void DbInit() {
    }
    
    //获取校准头信息
    int FreqHeader(const char& tx_or_rx, const char* band, double* freq, int maxnum) {
        int i;
		std::string shead = tx_or_rx == OFFSET_TX ? "TX_" : "RX_";

        std::string sband(band);
		std::string stable = shead + sband;
        std::string sql = "select ["+ sband + "] from [" + stable + "]";
        
        sqlite3_stmt* pstmt = NULL;
        sqlite3_prepare(_pConn, sql.c_str(), -1, &pstmt, NULL);

        for (i =0 ; i < maxnum; ++i) {
            if (sqlite3_step(pstmt) == SQLITE_ROW) {
                
                double val = sqlite3_column_double(pstmt, 0);
                if(val !=0)
                    *(freq + i) = val;
                else
                    break;
#ifdef JC_SQL_DEBUG
				std::cout << val << "/";	
#endif
            }
            else {
                break;
            }
        }
#ifdef JC_SQL_DEBUG
		std::cout << "\n";
		std::cout << "Num:" << i << std::endl;
#endif

        sqlite3_finalize(pstmt);
        return i;
    }
    
    //存储校准数据
	int Store_v2(const char& tx_or_rx,
				 const char* band, const char& dut, const char& coup, 
				 const char& real_or_dsp,
				 const double tx, 
				 const double* val, int num) {

		std::string sSuffix = dut == 0 ? "_A" : "_B";
		if (tx_or_rx == OFFSET_TX)
			sSuffix += (coup == 0 ? "_TX1" : "_TX2");

		std::string stable = tx_or_rx == OFFSET_TX ? "JC_TX_OFFSET_ALL" : "JC_RX_OFFSET_ALL";
		//设置复合主键Port
		std::string sport = std::string(band) + sSuffix;
		//设置复合主键Dsp
		int idsp = real_or_dsp;
        
        std::stringstream ss_freq;
        std::stringstream ss_val;
        
		if (tx_or_rx == OFFSET_TX) {
			//TX列序号数组（从1开始计数）
			ss_freq << "(Port,Dsp,Power,'" << 1 << "'";
			for (int i = 2; i <= num; ++i) {
				ss_freq << ",'" << i << "'";
			}
			ss_freq << ")";

			//存储值TX数组
			ss_val << "('" << sport << "'," << idsp << "," << tx << "," << val[0];
			for (int i = 1; i < num; ++i) {
				ss_val << "," << val[i];
			}
			ss_val << ")";
		}
		else {
			//RX列序号数组（从1开始计数）
			ss_freq << "(Port,'" << 1 << "'";
			for (int i = 2; i <= num; ++i) {
				ss_freq << ",'" << i << "'";
			}
			ss_freq << ")";

			//存储值RX数组
			ss_val << "('" << sport << "'," << val[0];
			for (int i = 1; i < num; ++i) {
				ss_val << "," << val[i];
			}
			ss_val << ")";
		}

        
        //开始存储，按列名写入，注：未列入的列名默认值为0
		std::string sql = "insert or replace into [" + stable + "] " + std::string(ss_freq.str()) + " values " + ss_val.str();
        sqlite3_stmt* pstmt;
        sqlite3_prepare(_pConn, sql.c_str(), -1, &pstmt, NULL);
        int resulte = sqlite3_step(pstmt);
        sqlite3_finalize(pstmt);
        
        if (resulte == SQLITE_DONE)
            return 0;
        else
            return JCOFFSET_ERROR;
    }
    
    //存储（已废除）
    int store() {
        std::string sql = "insert or replace into test (Power,'930') values (41,4)";
        sqlite3_stmt* pstmt;
        sqlite3_prepare(_pConn, sql.c_str(), -1, &pstmt, NULL);
        int resulte = sqlite3_step(pstmt);
        sqlite3_finalize(pstmt);
        
        if (resulte == SQLITE_DONE)
			return 0;
        else
			return JCOFFSET_ERROR;
    }
    
    //获取Tx校准数据
	double OffsetTx(const char* band, const char& dut, const char& coup, 
					const char& real_or_dsp,
					const double& freq_mhz, const double& tx_dbm) {
		std::string sSuffix = dut == 0 ? "_A" : "_B";
		sSuffix += (coup == 0 ? "_TX1" : "_TX2");

        double f1 = 0;
        double f2 = 0;
        double tx1 = 0;
        double tx2 = 0;
        std::stringstream ss;
        
        //选择校准频率表TX_EGSM900, 选择列 EGSM900
        //get(Freq) f1, f2!
        //查询freq_now的所在区间
		std::string stable = "TX_" + std::string(band);
        std::string scolomn(band);
        ss.str("");
        ss << "select Max_val, Min_val from (select max(" + scolomn + 
			") Max_val from [" + stable + "] where " + scolomn + 
			" <= " << freq_mhz << "), (select min(" + scolomn + 
			") Min_val from [" + stable + "] where " + scolomn + 
			" >= " << freq_mhz << ")";
        if(GetSqlVal(ss.str().c_str(), f1, f2))
            return JCOFFSET_ERROR;
#ifdef JC_SQL_DEBUG
        std::cout << "freq Range: " << f1 << ", " << f2 << std::endl;
#endif
		//查找f1,f2对应的index
		double f1_index, f2_index;
		double freq1, freq2;
		ss.str("");
		ss << "select A,B from (select [ID] A from [" << stable << "] where " + scolomn +
			"=" << f1 << "),(select [ID] B from [" << stable << "] where " + scolomn +
			"=" << f2 << ")";
		if (GetSqlVal(ss.str().c_str(), f1_index, f2_index))
			return JCOFFSET_ERROR;
#ifdef JC_SQL_DEBUG
		std::cout << "freq_index Range: " << f1_index << ", " << f2_index << std::endl;
#endif
		freq1 = f1;
		freq2 = f2;
		f1 = f1_index;
		f2 = f2_index;
        
        //选择校准数据表 EGSM900, 选择列 Power
        //get(TX) tx1, tx2!
        //查询tx_now的所在区间
		stable = "JC_TX_OFFSET_ALL";
		//设置复合主键Port
		std::string sport = std::string(band) + sSuffix;
		//设置复合主键Dsp
		int idsp = real_or_dsp;

        scolomn = "Power";
        ss.str("");
        ss << "select Max_val, Min_val from (select max(" + scolomn + 
			") Max_val from [" + stable + "] where " + scolomn + 
			" <= " << tx_dbm << " and Port = '" + sport + "' and Dsp = " << idsp << "), (select min(" + scolomn +
			") Min_val from [" + stable + "] where " + scolomn + 
			" >= " << tx_dbm << " and Port = '" + sport + "' and Dsp = " << idsp << ")";
        if(GetSqlVal(ss.str().c_str(), tx1, tx2))
            return JCOFFSET_ERROR;
#ifdef JC_SQL_DEBUG
        std::cout << "tx Range: " << tx1 << ", " << tx2<< std::endl;
#endif
        
        double y1 = 0;
        double y2 = 0;
        double z1 = 0;
        double z2 = 0;
        
        //get y1, y2!
        //查询表中2个点的值(tx1,f1),(tx1,f2)
        ss.str("");
        ss << "select Val1, Val2 from (select [" << f1 << "] Val1 from [" + stable + 
			"] where Power = " << tx1 << " and Port = '" + sport + "' and Dsp = " << idsp << "), (select [" << f2 << "] Val2 from [" + stable +
			"] where Power = " << tx1 << " and Port = '" + sport + "' and Dsp = " << idsp << ")";
        if(GetSqlVal(ss.str().c_str(), y1, y2))
            return JCOFFSET_ERROR;
#ifdef JC_SQL_DEBUG
        std::cout << "y1 = " << y1 << ", y2 = " << y2 << std::endl;
#endif
        
        //get y3, y4!
        //查询表中2个点的值(tx2,f1),(tx2,f2)
        ss.str("");
        ss << "select Val1, Val2 from (select [" << f1 << "] Val1 from [" + stable + 
			"] where Power = " << tx2 << " and Port = '" + sport + "' and Dsp = " << idsp << "), (select [" << f2 << "] Val2 from [" + stable +
			"] where Power = " << tx2 << " and Port = '" + sport + "' and Dsp = " << idsp << ")";
        if(GetSqlVal(ss.str().c_str(), z1, z2))
            return JCOFFSET_ERROR;
#ifdef JC_SQL_DEBUG
        std::cout << "z1 = " << z1 << ", z2 = " << z2 << std::endl;
#endif
        
        //开始计算offset
        double offset1 = SumSlope(freq_mhz, freq1, y1, freq2, y2);
        double offset2 = SumSlope(freq_mhz, freq1, z1, freq2, z2);
#ifdef JC_SQL_DEBUG
        std::cout << "offset1 = " << offset1 << std::endl;
        std::cout << "offset2 = " << offset2 << std::endl;
#endif
        
        return SumSlope(tx_dbm, tx1, offset1, tx2, offset2);
    }

	//获取Rx校准数据
	double OffsetRx(const char* band, const char& dut,
		const double& freq_now) {
		std::stringstream ss;
		std::string sSuffix = dut == 0 ? "_A" : "_B";

		double f1 = 0;
		double f2 = 0;
		//选择校准频率表RX_EGSM900, 选择列 EGSM900
		//get(Freq) f1, f2!
		std::string stable = "RX_" + std::string(band);
		std::string scolomn(band);
		ss.str("");
		ss << "select Max_val, Min_val from (select max(" + scolomn +
			") Max_val from [" + stable + "] where " + scolomn +
			" <= " << freq_now << "), (select min(" + scolomn +
			") Min_val from [" + stable + "] where " + scolomn +
			" >= " << freq_now << ")";
		if (GetSqlVal(ss.str().c_str(), f1, f2))
			return JCOFFSET_ERROR;
#ifdef JC_SQL_DEBUG
		std::cout << "Rxfreq Range: " << f1 << ", " << f2 << std::endl;
#endif
		//查找f1,f2对应的index
		double f1_index, f2_index;
		double freq1, freq2;
		ss.str("");
		ss << "select A,B from (select [ID] A from [" << stable << "] where " + scolomn +
			"=" << f1 << "),(select [ID] B from [" << stable << "] where " + scolomn +
			"=" << f2 << ")";
		if (GetSqlVal(ss.str().c_str(), f1_index, f2_index))
			return JCOFFSET_ERROR;
#ifdef JC_SQL_DEBUG
		std::cout << "Rxfreq_index Range: " << f1_index << ", " << f2_index << std::endl;
#endif
		freq1 = f1;
		freq2 = f2;
		f1 = f1_index;
		f2 = f2_index;

		double y1 = 0;
		double y2 = 0;
		//设置复合主键Port
		std::string sport = std::string(band) + sSuffix;
		stable = "JC_RX_OFFSET_ALL";
		//get y1, y2!
		//查询表中2个点的值(tx1,f1),(tx1,f2)
		ss.str("");
		ss << "select Val1, Val2 from (select [" << f1 << "] Val1 from [" + stable +
			"] where Port = '" + sport + "'), (select [" << f2 << "] Val2 from [" + stable +
			"] where Port = '" + sport + "')";
		if (GetSqlVal(ss.str().c_str(), y1, y2))
			return JCOFFSET_ERROR;
#ifdef JC_SQL_DEBUG
		std::cout << "y1 = " << y1 << ", y2 = " << y2 << std::endl;
#endif
		//开始计算offset
		return SumSlope(freq_now, freq1, y1, freq2, y2);
	}

	//获取vco数据
	double OffsetVco(const char* band, const char& dut) {
		double val = -10000;
		std::string sSuffix = dut == 0 ? "_A" : "_B";

		std::string sTable = "JC_VCO_OFFSET_ALL";
		std::string sColomn(band);
		sColomn += sSuffix;
		std::string sql = "select vco from " + sTable + " where port = '" + sColomn + "'";

		sqlite3_stmt* pStmt;
		int s = sqlite3_prepare(_pConn, sql.c_str(), -1, &pStmt, NULL);
		if (sqlite3_step(pStmt) == SQLITE_ROW) {
			val = sqlite3_column_double(pStmt, 0);
		}
		else
			val = JCOFFSET_ERROR;
		sqlite3_finalize(pStmt);
		return val;
	}

	int Store_vco_single(const char* band, const char& dut, const double val) {
		std::string sSuffix = dut == 0 ? "_A" : "_B";
		std::string sColomn(band);
		sColomn += sSuffix;

		std::string sql = "insert or replace into JC_VCO_OFFSET_ALL (port,vco) values ('" + sColomn + "'," + std::to_string(val) + ")";
		sqlite3_stmt* pstmt;
		sqlite3_prepare(_pConn, sql.c_str(), -1, &pstmt, NULL);
		int resulte = sqlite3_step(pstmt);
		sqlite3_finalize(pstmt);

		if (resulte == SQLITE_DONE)
			return 0;
		else
			return JCOFFSET_ERROR;
	}

private:
    //计算斜率
    double SumSlope(double v, double x1, double y1, double x2, double y2) {
        if (x1 == x2)
            return y1;
        else
            return (y2 - y1) / (x2 - x1) * v + (y1 * x2 - y2 * x1) / (x2 - x1);
    }
    
    //sqlite语句执行
    int GetSqlVal(const char* strsql, double& a1, double& a2) {
        sqlite3_stmt* pStmt;
		int r = 0;
        sqlite3_prepare(_pConn, strsql, -1, &pStmt, NULL);
        
        if( sqlite3_step(pStmt) == SQLITE_ROW) {
            if(sqlite3_column_count(pStmt) == 2) {
                a1 = sqlite3_column_double(pStmt, 0);
                a2 = sqlite3_column_double(pStmt, 1);
            }
            else
                r =  JCOFFSET_ERROR;
        }
        else
            r = JCOFFSET_ERROR;
        sqlite3_finalize(pStmt);
        return r;
    }
    
private:
    sqlite3* _pConn;
    bool _bConn;
    std::string _sTxFreqtable;
};
#endif /* defined(__MyTest__JcOffsetDB__) */
