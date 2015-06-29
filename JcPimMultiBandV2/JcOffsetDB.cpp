#include "JcOffsetDB.h"

//校准步进1M
#define OFFSET_STEP 1

bool JcOffsetDB::DbConnect(const char* addr) {
	m_bConn = !sqlite3_open(addr, &m_pConn);
	return m_bConn;
}

void JcOffsetDB::DbInit(uint8_t mode) {
	std::string hw_sql_param[7] = huawei_sql_body;
	std::string poi_sql_param[12] = poi_sql_body;

	m_hw_band_table_sql = sql_header + hw_sql_param[0];
	for (int i = 1; i < 7; i++){
		m_hw_band_table_sql += " union all select " + hw_sql_param[i];
	}

	m_poi_band_table_sql = sql_header + poi_sql_param[0];
	for (int i = 1; i < 12; i++){
		m_poi_band_table_sql += " union all select " + poi_sql_param[i];
	}

	if (mode == MODE_POI)
		m_offset_mode = continuous_offset_mode;
	else
		m_offset_mode = discontinuous_offset_mode;
}

int JcOffsetDB::FreqBand(const uint8_t& tx_or_rx, const char* band, double& f_start, double& f_stop) {
	if (m_offset_mode == continuous_offset_mode)
	{
		char sql[1024] = { 0 };
		if (tx_or_rx == OFFSET_TX)
			sprintf_s(sql, "select [tx_start],[tx_end] from [JC_BAND_INFO] where band = '%s'", band);
		else
			sprintf_s(sql, "select [rx_start],[rx_end] from [JC_BAND_INFO] where band = '%s'", band);

		if (GetSqlVal(sql, f_start, f_stop))
			return JCOFFSET_ERROR;
	}
	else
	{
		f_start = JCOFFSET_ERROR;
		f_stop = JCOFFSET_ERROR;
	}
	return 0;
}

int JcOffsetDB::FreqBand_Old(const uint8_t& tx_or_rx, const double& freq_mhz, const char* band,
							double &f1, double &f2, double &index1, double &index2) {
	std::stringstream ss;
	//选择校准频率表TX_EGSM900, 选择列 EGSM900
	//get(Freq) f1, f2!
	//查询freq_now的所在区间
	std::string stable = tx_or_rx == OFFSET_TX ? "TX_" + std::string(band) : "RX_" + std::string(band);
	std::string scolomn(band);
	//查找校准频率区间f1,f2的频率值
	ss.str("");
	ss << "select Max_val, Min_val from (select max(" + scolomn +
		") Max_val from [" + stable + "] where " + scolomn +
		" <= " << freq_mhz << "), (select min(" + scolomn +
		") Min_val from [" + stable + "] where " + scolomn +
		" >= " << freq_mhz << ")";
	if (GetSqlVal(ss.str().c_str(), f1, f2))
		return JCOFFSET_ERROR;
#ifdef JC_SQL_DEBUG
	std::cout << "freq Range: " << f1 << ", " << f2 << std::endl;
#endif
	//查找f1,f2对应的index
	ss.str("");
	ss << "select A,B from (select [ID] A from [" << stable << "] where " + scolomn +
		"=" << f1 << "),(select [ID] B from [" << stable << "] where " + scolomn +
		"=" << f2 << ")";
	if (GetSqlVal(ss.str().c_str(), index1, index2))
		return JCOFFSET_ERROR;
#ifdef JC_SQL_DEBUG
	std::cout << "freq_index Range: " << f1_index << ", " << f2_index << std::endl;
#endif
	return 0;
}

int JcOffsetDB::FreqHeader(const char& tx_or_rx, const char* band, double* freq, int maxnum) {
	int i;

	//新增POI获取频率区间
	if (m_offset_mode == continuous_offset_mode)
	{
		double f_start, f_stop;
		int s = FreqBand(tx_or_rx, band, f_start, f_stop);
		if (s == JCOFFSET_ERROR)
			return s;

		int num = ceil((f_stop - f_start) / OFFSET_STEP) + 1;
		num = num < maxnum ? num : maxnum;
		for (int j = 0; j < num; ++j)
		{
			*(freq + j) = f_start + OFFSET_STEP*j;
		}

		//最后一点不在步进点上时，修正最后一点
		if ((f_start + (num - 1)  * OFFSET_STEP) > f_stop)
			*(freq + num - 1) = f_stop;
		i = num;
	}
	else
	{
		std::string shead = tx_or_rx == OFFSET_TX ? "TX_" : "RX_";
		std::string sband(band);
		std::string stable = shead + sband;
		std::string sql = "select [" + sband + "] from [" + stable + "]";

		sqlite3_stmt* pstmt = NULL;
		sqlite3_prepare(m_pConn, sql.c_str(), -1, &pstmt, NULL);

		for (i = 0; i < maxnum; ++i) {
			if (sqlite3_step(pstmt) == SQLITE_ROW) 
			{
				double val = sqlite3_column_double(pstmt, 0);
				if (val != 0)
					*(freq + i) = val;
				else
					break;
#ifdef JC_SQL_DEBUG
				std::cout << val << "/";
#endif
			}
			else
				break;
		}
#ifdef JC_SQL_DEBUG
		std::cout << "\n";
		std::cout << "Num:" << i << std::endl;
#endif
		sqlite3_finalize(pstmt);
	}

	return i;
}

double JcOffsetDB::OffsetTx(const char* band, const char& dut, const char& coup,
							const char& real_or_dsp,
							const double& freq_mhz, const double& tx_dbm) {
	std::stringstream ss;
	std::string sSuffix = dut == 0 ? "_A" : "_B";
	sSuffix += (coup == 0 ? "_TX1" : "_TX2");

	//double f1_index, f2_index, f1_temp, f2_temp;
	double freq1, freq2, f1, f2;
	//新增POI获取频率区间
	if (m_offset_mode == continuous_offset_mode) {
		double f_start, f_stop;
		int s = FreqBand(OFFSET_TX, band, f_start, f_stop);
		if (s == JCOFFSET_ERROR) 
			return s;

		if (freq_mhz<f_start || freq_mhz>f_stop)
			return JCOFFSET_ERROR;
		//查找序号
		f1 = floor((freq_mhz - f_start) / OFFSET_STEP) + 1;
		f2 = ceil ((freq_mhz - f_start) / OFFSET_STEP) + 1;
		//查找序号对应的值
		freq1 = f_start + OFFSET_STEP * (f1 - 1);
		freq2 = f_start + OFFSET_STEP * (f2 - 1);
	}
	else 
	{
		FreqBand_Old(OFFSET_TX, freq_mhz, band, freq1, freq2, f1, f2);
	}

	//double freq1 = f1_temp;
	//double freq2 = f2_temp;
	//double f1 = f1_index;
	//double f2 = f2_index;

	double tx1 = 0;
	double tx2 = 0;
	//选择校准数据表 EGSM900, 选择列 Power
	//get(TX) tx1, tx2!
	//查询tx_now的所在区间
	std::string stable = "JC_TX_OFFSET_ALL";
	//设置复合主键Port
	std::string sport = std::string(band) + sSuffix;
	//设置复合主键Dsp
	int idsp = real_or_dsp;
	std::string scolomn = "Power";
	ss.str("");
	ss << "select Max_val, Min_val from (select max(" + scolomn +
		") Max_val from [" + stable + "] where " + scolomn +
		" <= " << tx_dbm << " and Port = '" + sport + "' and Dsp = " << idsp << "), (select min(" + scolomn +
		") Min_val from [" + stable + "] where " + scolomn +
		" >= " << tx_dbm << " and Port = '" + sport + "' and Dsp = " << idsp << ")";
	if (GetSqlVal(ss.str().c_str(), tx1, tx2))
		return JCOFFSET_ERROR;
#ifdef JC_SQL_DEBUG
	std::cout << "tx Range: " << tx1 << ", " << tx2 << std::endl;
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
	if (GetSqlVal(ss.str().c_str(), y1, y2))
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
	if (GetSqlVal(ss.str().c_str(), z1, z2))
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
double JcOffsetDB::OffsetRx(const char* band, const char& dut, const double& freq_now) {
	std::stringstream ss;
	std::string sSuffix = dut == 0 ? "_A" : "_B";

	double freq1, freq2, f1, f2;
	//新增POI获取频率区间
	if (m_offset_mode == continuous_offset_mode) {
		double f_start, f_stop;
		int s = FreqBand(OFFSET_RX, band, f_start, f_stop);
		if (s == JCOFFSET_ERROR) 
			return s;

		if (freq_now<f_start || freq_now>f_stop)
			return JCOFFSET_ERROR;

		f1 = floor((freq_now - f_start) / OFFSET_STEP) + 1;
		f2 = ceil((freq_now - f_start) / OFFSET_STEP) + 1;
		freq1 = f_start + OFFSET_STEP * (f1 - 1);
		freq2 = f_start + OFFSET_STEP * (f2 - 1);
	}
	else
	{
		FreqBand_Old(OFFSET_RX, freq_now, band, freq1, freq2, f1, f2);
	}

	//double freq1 = f1_temp;
	//double freq2 = f2_temp;
	//double f1 = f1_index;
	//double f2 = f2_index;

	double y1 = 0;
	double y2 = 0;
	//设置复合主键Port
	std::string sport = std::string(band) + sSuffix;
	std::string stable = "JC_RX_OFFSET_ALL";
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
double JcOffsetDB::OffsetVco(const char* band, const char& dut) {
	double val = -10000;
	std::string sSuffix = dut == 0 ? "_A" : "_B";

	std::string sTable = "JC_VCO_OFFSET_ALL";
	std::string sColomn(band);
	sColomn += sSuffix;
	std::string sql = "select vco from " + sTable + " where port = '" + sColomn + "'";

	sqlite3_stmt* pStmt;
	int s = sqlite3_prepare(m_pConn, sql.c_str(), -1, &pStmt, NULL);
	if (sqlite3_step(pStmt) == SQLITE_ROW) {
		val = sqlite3_column_double(pStmt, 0);
	}
	else
		val = JCOFFSET_ERROR;
	sqlite3_finalize(pStmt);
	return val;
}

//存储校准数据
int JcOffsetDB::Store_v2(const char& tx_or_rx,
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
	std::string sql = "insert or replace into [" + stable + "] " + std::string(ss_freq.str()) + " values " + std::string(ss_val.str());
	sqlite3_stmt* pstmt;
	sqlite3_prepare(m_pConn, sql.c_str(), -1, &pstmt, NULL);
	int resulte = sqlite3_step(pstmt);
	sqlite3_finalize(pstmt);

	if (resulte == SQLITE_DONE)
		return 0;
	else
		return JCOFFSET_ERROR;
}

//存储vco校准数据
int JcOffsetDB::Store_vco_single(const char* band, const char& dut, const double val) {
	std::string sSuffix = dut == 0 ? "_A" : "_B";
	std::string sColomn(band);
	sColomn += sSuffix;

	std::string sql = "insert or replace into JC_VCO_OFFSET_ALL (port,vco) values ('" + sColomn + "'," + std::to_string(val) + ")";
	sqlite3_stmt* pstmt;
	sqlite3_prepare(m_pConn, sql.c_str(), -1, &pstmt, NULL);
	int resulte = sqlite3_step(pstmt);
	sqlite3_finalize(pstmt);

	if (resulte == SQLITE_DONE)
		return 0;
	else
		return JCOFFSET_ERROR;
}

//计算斜率
double JcOffsetDB::SumSlope(double v, double x1, double y1, double x2, double y2) {
	if (x1 == x2)
		return y1;
	else
		return (y2 - y1) / (x2 - x1) * v + (y1 * x2 - y2 * x1) / (x2 - x1);
}

//sqlite语句执行
int JcOffsetDB::GetSqlVal(const char* strsql, double& a1, double& a2){
	sqlite3_stmt* pStmt;
	int r = 0;
	sqlite3_prepare(m_pConn, strsql, -1, &pStmt, NULL);

	if (sqlite3_step(pStmt) == SQLITE_ROW) {
		if (sqlite3_column_count(pStmt) == 2) {
			a1 = sqlite3_column_double(pStmt, 0);
			a2 = sqlite3_column_double(pStmt, 1);
		}
		else
			r = JCOFFSET_ERROR;
	}
	else
		r = JCOFFSET_ERROR;
	sqlite3_finalize(pStmt);
	return r;
}