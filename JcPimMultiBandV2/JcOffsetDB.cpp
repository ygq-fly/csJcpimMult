#include "JcOffsetDB.h"
#include "MyUtil\JcCommonAPI.h"
#include "3rdParty\CppSQLite3.h"

//校准步进1M
#define OFFSET_STEP_TX 1
#define OFFSET_STEP_RX 1

JcOffsetDB::JcOffsetDB()
	: m_pSQLite(new CppSQLite3DB)
	, m_bConn(false)
	, m_band_info_table("JC_BAND2_INFO")
	, m_tx_offset_table("JC_TX_OFFSET_ALL")
	, m_rx_offset_table("JC_RX_OFFSET_ALL")
	, m_tx_dpx_offset_table("JC_TX_OFFSET_DPX")
	, m_rx_dpx_offset_table("JC_RX_OFFSET_DPX")
	, m_setting_table("JC_SETTING_INFO")
	, m_offset_mode(discontinuous_offset_mode)
	, m_tx_step(OFFSET_STEP_TX)
	, m_rx_step(OFFSET_STEP_RX)
{}

JcOffsetDB::~JcOffsetDB() {
	//if (m_pConn) {
	//	sqlite3_close(m_pConn);
	//}
	if (m_pSQLite != nullptr) {
		m_pSQLite->close();
		delete m_pSQLite;
	}
}

void JcOffsetDB::SetOffsetStep(double tx_step, double rx_step) {
	if (tx_step <= 0)
		m_tx_step = OFFSET_STEP_TX;
	else
		m_tx_step = tx_step;

	if (rx_step <= 0)
		m_rx_step = OFFSET_STEP_RX;
	else
		m_rx_step = rx_step;
}

bool JcOffsetDB::DbConnect(const char* addr) {
	//m_bConn = !sqlite3_open(addr, &m_pConn);
	try{
		m_pSQLite->open(addr);
		m_bConn = true;
	}
	catch (CppSQLite3Exception ex) {
		m_bConn = false;
	}
	return m_bConn;
}

void JcOffsetDB::DbInit(uint8_t mode) {
	//if (mode == MODE_POI)
	m_offset_mode = continuous_offset_mode;
	//else
	//	m_offset_mode = discontinuous_offset_mode;
	m_band_info_table = "JC_BAND2_INFO";
	m_tx_offset_table = "JC_TX_OFFSET_ALL";
	m_rx_offset_table = "JC_RX_OFFSET_ALL";
	m_tx_dpx_offset_table = "JC_TX_OFFSET_DPX";
	m_rx_dpx_offset_table = "JC_RX_OFFSET_DPX";
	m_vco_offset_table = "JC_VCO_OFFSET_ALL";

	//if (!IsExist("JC_SETTING_INFO")) {
	//	std::string table = "CREATE TABLE \"JC_SETTING_INFO\" ("
	//		"\"key\" text NOT NULL,"
	//		"\"value\" text,"
	//		"PRIMARY KEY(\"key\"))";
	//	if (ExecSql(table.c_str())) {
	//		ExecSql("insert into [JC_SETTING_INFO] (key, value) values ('version', '1.1')");
	//		ExecSql("insert into [JC_SETTING_INFO] (key, value) values ('sn', '0000000000')");
	//		ExecSql("insert into [JC_SETTING_INFO] (key, value) values ('license', '20151231')");
	//		ExecSql("insert into [JC_SETTING_INFO] (key, value) values ('used_date', '20150101')");
	//		ExecSql("insert into [JC_SETTING_INFO] (key, value) values ('expire_date', '0')");
	//	}
	//}

	if (!IsExist(m_band_info_table.c_str()))
		ExecSql(sql_table);

	if (GetColCount(m_band_info_table.c_str()) == 11) {
		//扩充2个字段
		ExecSql("ALTER TABLE JC_BAND2_INFO ADD COLUMN vco_enable INTEGER NOT NULL  DEFAULT 1");
		ExecSql("ALTER TABLE JC_BAND2_INFO ADD COLUMN coup_enable INTEGER NOT NULL  DEFAULT 1");
	}

	//华为模式
	std::string hw_sql_param[8] = huawei_sql_body;
	std::string hw_band_table_sql = sql_header + hw_sql_param[0];
	for (int i = 1; i < 7; i++){
		hw_band_table_sql += " union all select " + hw_sql_param[i];
	}

	if (GetBandCount("hw") == 0)
		ExecSql(hw_band_table_sql.c_str());
	//华为扩频至8频
	if (GetBandInfo("hw8", NULL) == -1) {
		//printf("No find hw8!\n");
		std::string strSql("insert into [JC_BAND2_INFO] (prefix,band,tx_start,tx_end,rx_start,rx_end,vco_a,vco_b,tx_enable,coup1,coup2) values (");
		strSql += hw_sql_param[7];
		strSql += ")";
		ExecSql(strSql.c_str());
	}

	//poi 15频(12频扩容)
	std::string poi_sql_param[15] = poi_sql_body;
	int count = GetBandCount(poi_flag);
	if (count == 12 || count == 13) {
		ExecSql("delete from JC_BAND2_INFO  where prefix like 'poi%'");
		count = 0;
	}

	if (count == 0) {
		std::string poi_band_table_sql = sql_header + poi_sql_param[0];
		for (int i = 1; i < 15; i++){
			poi_band_table_sql += " union all select " + poi_sql_param[i];
		}
		ExecSql(poi_band_table_sql.c_str());
	}

	//poi新12频
	if (GetBandCount(NewPoi_flag) == 0) {
		std::string NewPoi_sql_param[12] = NewPoi_sql_body;
		std::string NewPoi_band_table_sql = sql_header + NewPoi_sql_param[0];
		for (int i = 1; i < 12; i++){
			NewPoi_band_table_sql += " union all select " + NewPoi_sql_param[i];
		}

		ExecSql(NewPoi_band_table_sql.c_str());
	}

	//华为新8频
	if (GetBandCount(NewHuawei_flag) == 0) {
		std::string nhw_sql_param[8] = NewHuawei_sql_body;
		std::string HuaweiA_band_table_sql = sql_header + nhw_sql_param[0];
		for (int i = 1; i < 8; i++){
			HuaweiA_band_table_sql += " union all select " + nhw_sql_param[i];
		}

		ExecSql(HuaweiA_band_table_sql.c_str());
	}

	//华为新8频
	if (GetBandCount(Dpx_flag) == 0) {
		std::string dpx_sql_param[12] = Dpx_sql_body;
		std::string dpx_band_table_sql = sql_header + dpx_sql_param[0];
		for (int i = 1; i < 12; i++){
			dpx_band_table_sql += " union all select " + dpx_sql_param[i];
		}

		ExecSql(dpx_band_table_sql.c_str());
	}

	//poi15频
	//if (GetBandCount(poi_15_flag) == 0) {
	//	std::string poi15_sql_param[15] = poi_15_sql_body;
	//	std::string poi15_band_table_sql = sql_header + poi15_sql_param[0];
	//	for (int i = 1; i < 8; i++){
	//		poi15_band_table_sql += " union all select " + poi15_sql_param[i];
	//	}

	//	ExecSql(poi15_band_table_sql.c_str());
	//}

	//{
	//	std::string version(VERSION_BAND);
	//	char sql[128] = { 0 };
	//	sprintf_s(sql, "update [JC_SETTING_INFO] set [value] = '%s' where [key] = 'version'", version.c_str());
	//	ExecSql(sql);
	//}

	if (!IsExist(m_tx_offset_table.c_str())) {
		std::string table = "CREATE TABLE \"JC_TX_OFFSET_ALL\" ("
			"\"Port\" text NOT NULL,"
			"\"Dsp\" integer NOT NULL,"
			"\"Power\" real NOT NULL DEFAULT(null),";
		for (int i = 1; i <= 150; i++) {
			char param[32] = { 0 };
			sprintf_s(param, "\"%d\" real,", i);
			table += std::string(param);
		}
		table += "PRIMARY KEY(\"Port\", \"Dsp\", \"Power\"))";
		ExecSql(table.c_str());
	}

	if (!IsExist(m_rx_offset_table.c_str())) {
		std::string table = "CREATE TABLE \"JC_RX_OFFSET_ALL\" ("
			"\"Port\" text NOT NULL, ";

		for (int i = 1; i <= 150; i++) {
			char param[32] = { 0 };
			sprintf_s(param, "\"%d\" real,", i);
			table += std::string(param);
		}
		table += "PRIMARY KEY(\"Port\"))";
		ExecSql(table.c_str());
	}

	if (!IsExist("JC_VCO_OFFSET_ALL")) {
		std::string table = "CREATE TABLE \"JC_VCO_OFFSET_ALL\" ("
			"\"port\" text NOT NULL,"
			"\"vco\" real,"
			"PRIMARY KEY(\"port\"))";
		ExecSql(table.c_str());
	}

	if (!IsExist("JC_CALIBATION_TIME")) {
		std::string table = "CREATE TABLE \"JC_CALIBATION_TIME\" ("
			"\"port\" text NOT NULL,"
			"\"time\" text,"
			"PRIMARY KEY(\"port\"))";
		ExecSql(table.c_str());
	}

	if (!IsExist("JC_TX_OFFSET_DPX")) {
		char table[] = "CREATE TABLE \"JC_TX_OFFSET_DPX\" ("
			"\"Port\" text NOT NULL,"
			"\"Dsp\" integer NOT NULL,"
			"\"Power\" real NOT NULL,"
			"\"Frequency\" real NOT NULL,"
			"\"Value\" real NOT NULL,"
			"PRIMARY KEY(\"Port\",\"Dsp\",\"Power\",\"Frequency\"))";
		ExecSql(table);
	}

	if (!IsExist("JC_RX_OFFSET_DPX")) {
		char table[] = "CREATE TABLE \"JC_RX_OFFSET_DPX\" ("
			"\"Port\" text NOT NULL,"
			"\"Frequency\" real NOT NULL,"
			"\"Value\" real NOT NULL,"
			"PRIMARY KEY(\"Port\",\"Frequency\"))";
		ExecSql(table);
	}
}

bool JcOffsetDB::DbSetTxIncremental(const char* band, char dut, char coup, char real_or_dsp, double incremental) {
	bool ret = true;
	double freq[256] = { 0 };
	int num = FreqHeader(OFFSET_TX, band, freq, 256);
	
	std::string sSuffix = dut == 0 ? "_A" : "_B";
	sSuffix += (coup == 0 ? "_TX1" : "_TX2");

	for (int i = 1; i <= num; i++) {
		char sql[128] = { 0 };
		sprintf_s(sql, "update %s set [%d] = [%d] + (%lf) where Port = '%s%s' and Dsp = '%d'",
			m_tx_offset_table.c_str(), i, i, incremental, band, sSuffix.c_str(), (int)real_or_dsp);
		if (ExecSql(sql) == false) {
			ret = false;
			break;
		}

		//memset(sql, 0, 128);
		//sprintf_s(sql, "update %s set [%d] = [%d] - (%lf) where Port = '%s%s' and DSP = 1",
		//	m_tx_offset_table.c_str(), i, i, incremental, band, sSuffix.c_str());
		//if (ExecSql(sql) == false) {
		//	ret = false;
		//	break;
		//}

	}
	return ret;
}

int JcOffsetDB::GetBandCount(const char* band_mode) {
	char sql[128] = { 0 };
	sprintf_s(sql, "select count(*) as c from JC_BAND2_INFO  where prefix like '%s%%'", band_mode);
	int n = 0;

	//sqlite3_stmt* pstmt = NULL;
	//sqlite3_prepare(m_pConn, sql, -1, &pstmt, NULL);
	//if (sqlite3_step(pstmt) == SQLITE_ROW){
	//	n = sqlite3_column_int(pstmt, 0);
	//}
	//sqlite3_finalize(pstmt);

	try{
		n = m_pSQLite->execScalar(sql);
	}
	catch (CppSQLite3Exception ex) {
		n = -1;
	}
	return n;
}

int JcOffsetDB::GetBandInfo(const char* prefix, char* band_info) {

	char sql[1024] = { 0 };
	sprintf_s(sql, "select * from %s where prefix = '%s'", m_band_info_table.c_str(), prefix);

	//sqlite3_stmt* pstmt = NULL;
	//sqlite3_prepare(m_pConn, sql, -1, &pstmt, NULL);
	//std::string str_band_info = "";
	//while (sqlite3_step(pstmt) == SQLITE_ROW)
	//{
	//	std::string temp;
	//	int n = sqlite3_column_count(pstmt);
	//	for (int i = 0; i < n; i++) {
	//		const unsigned char * cell = sqlite3_column_text(pstmt, i);

	//		if (cell)
	//			temp.assign(reinterpret_cast<const char*>(cell));
	//		else
	//			temp.assign("");

	//		if (i == (n - 1))
	//			str_band_info += temp;
	//		else
	//			str_band_info += (temp + ",");
	//	}
	//} 
	//sqlite3_finalize(pstmt);

	std::string str_band_info = "";

	try{
		CppSQLite3Query query = m_pSQLite->execQuery(sql);
		if (query.eof() || query.numFields() == 0) {
			return JCOFFSET_ERROR;
		}

		int n = query.numFields();
		for (int i = 0; i < n; i++) {
			std::string temp;
			const char * cell = query.getStringField(i);
			if (cell)
				temp.assign(cell);
			else
				temp.assign("");

			if (i == (n - 1))
				str_band_info += temp;
			else
				str_band_info += (temp + ",");
		}
	}
	catch (CppSQLite3Exception ex) {
		return JCOFFSET_ERROR;
	}

	if (str_band_info == "")
		return -1;

	if (band_info != NULL)
		memcpy(band_info, str_band_info.c_str(), str_band_info.length());
	return 0;
}

int JcOffsetDB::FreqHeader(char tx_or_rx, const char* band, double* freq, int maxnum) {
	int i;

	//新增POI获取频率区间
	if (m_offset_mode == continuous_offset_mode) {
		double step = tx_or_rx == OFFSET_TX ? m_tx_step : m_rx_step;
		double f_start, f_stop;
		int s = FreqBand_continuous(tx_or_rx, band, f_start, f_stop);
		if (s == JCOFFSET_ERROR)
			return s;

		int num = ceil((f_stop - f_start) / step) + 1;
		num = num < maxnum ? num : maxnum;
		for (int j = 0; j < num; ++j) {
			*(freq + j) = f_start + step*j;
		}

		//最后一点不在步进点上时，修正最后一点
		if ((f_start + (num - 1)  * step) > f_stop)
			*(freq + num - 1) = f_stop;
		i = num;
	}
	else {
		std::string shead = tx_or_rx == OFFSET_TX ? "TX_" : "RX_";
		std::string sband(band);
		std::string stable = shead + sband;
		std::string sql = "select [" + sband + "] from [" + stable + "]";

		//sqlite3_stmt* pstmt = NULL;
		//sqlite3_prepare(m_pConn, sql.c_str(), -1, &pstmt, NULL);
		//for (i = 0; i < maxnum; ++i) {
		//	if (sqlite3_step(pstmt) == SQLITE_ROW) 
		//	{
		//		double val = sqlite3_column_double(pstmt, 0);
		//		if (val <= 0)
		//			break;
		//		*(freq + i) = val;
		//	}
		//	else
		//		break;
		//}
		//sqlite3_finalize(pstmt);

		try {
			CppSQLite3Query query =  m_pSQLite->execQuery(sql.c_str());	
			for (i = 0; i < maxnum; ++i) {
				if (query.eof() || query.numFields() == 0) {
					break;
				}

				double val = query.getFloatField(0);
				if (val <= 0)
					break;
				*(freq + i) = val;

				query.nextRow();
			}
		}
		catch (CppSQLite3Exception ex) {
			return JCOFFSET_ERROR;
		}
	}

	return i;
}

int JcOffsetDB::FreqHeader(uint8_t tx_or_rx, const char* band, double** freqs) {
	//if (freqs != NULL)
	//	return JCOFFSET_ERROR;

	double step = tx_or_rx == OFFSET_TX ? m_tx_step : m_rx_step;
	double f_start, f_stop;
	if (FreqBand_continuous(tx_or_rx, band, f_start, f_stop) == JCOFFSET_ERROR)
		return JCOFFSET_ERROR;

	int num = ceil((f_stop - f_start) / step) + 1;
	*freqs = new double[num];
	for (int j = 0; j < num; ++j) {
		if ((f_start + j * step) > f_stop)
			*(*freqs + j) = f_stop;
		else
			*(*freqs + j) = f_start + step*j;
	}

	return num;
}

double JcOffsetDB::OffsetTx(const char* band, char dut, char coup, char real_or_dsp, double freq_mhz, double tx_dbm) {
	std::stringstream ss;
	std::string sSuffix = dut == 0 ? "_A" : "_B";
	sSuffix += (coup == 0 ? "_TX1" : "_TX2");

	//double f1_index, f2_index, f1_temp, f2_temp;
	double freq1, freq2, f1, f2;
	//新增POI获取频率区间
	if (m_offset_mode == continuous_offset_mode) {
		double f_start, f_stop;
		int s = FreqBand_continuous(OFFSET_TX, band, f_start, f_stop);
		if (s == JCOFFSET_ERROR) 
			return s;

		if (freq_mhz<f_start || freq_mhz>f_stop)
			return JCOFFSET_ERROR;
		//查找序号
		f1 = floor((freq_mhz - f_start) / m_tx_step) + 1;
		f2 = ceil((freq_mhz - f_start) / m_tx_step) + 1;
		//查找序号对应的值
		freq1 = f_start + m_tx_step * (f1 - 1);
		freq2 = f_start + m_tx_step * (f2 - 1);
		if (freq1 > f_stop)
			freq1 = f_stop;
		if (freq2 > f_stop)
			freq2 = f_stop;
	}
	else 
	{
		FreqBand_discontinuous(OFFSET_TX, freq_mhz, band, freq1, freq2, f1, f2);
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
	std::string stable = m_tx_offset_table;//"JC_TX_OFFSET_ALL";
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
double JcOffsetDB::OffsetRx(const char* band, char dut, double freq_now) {
	std::stringstream ss;
	std::string sSuffix = dut == 0 ? "_A" : "_B";

	double freq1, freq2, f1, f2;
	//新增POI获取频率区间
	if (m_offset_mode == continuous_offset_mode) {
		double f_start, f_stop;
		int s = FreqBand_continuous(OFFSET_RX, band, f_start, f_stop);
		if (s == JCOFFSET_ERROR) 
			return s;

		if (freq_now<f_start || freq_now>f_stop)
			return JCOFFSET_ERROR;

		f1 = floor((freq_now - f_start) / m_rx_step) + 1;
		f2 = ceil((freq_now - f_start) / m_rx_step) + 1;
		freq1 = f_start + m_rx_step * (f1 - 1);
		freq2 = f_start + m_rx_step * (f2 - 1);
		if (freq1 > f_stop)
			freq1 = f_stop;
		if (freq2 > f_stop)
			freq2 = f_stop;
	}
	else
	{
		FreqBand_discontinuous(OFFSET_RX, freq_now, band, freq1, freq2, f1, f2);
	}

	//double freq1 = f1_temp;
	//double freq2 = f2_temp;
	//double f1 = f1_index;
	//double f2 = f2_index;

	double y1 = 0;
	double y2 = 0;
	//设置复合主键Port
	std::string sport = std::string(band) + sSuffix;
	std::string stable = m_rx_offset_table;// "JC_RX_OFFSET_ALL";
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
double JcOffsetDB::OffsetVco(const char* band, char dut) {
	double val = -10000;
	std::string sSuffix = dut == 0 ? "_A" : "_B";

	std::string sTable = "JC_VCO_OFFSET_ALL";
	std::string sColomn(band);
	sColomn += sSuffix;
	std::string sql = "select vco from " + sTable + " where port = '" + sColomn + "'";

	//sqlite3_stmt* pStmt;
	//int s = sqlite3_prepare(m_pConn, sql.c_str(), -1, &pStmt, NULL);
	//if (sqlite3_step(pStmt) == SQLITE_ROW) {
	//	val = sqlite3_column_double(pStmt, 0);
	//}
	//sqlite3_finalize(pStmt);

	try{
		CppSQLite3Query query = m_pSQLite->execQuery(sql.c_str());
		if (query.eof() || query.numFields() < 1)
		{
			return JCOFFSET_ERROR;
		}
		val = query.getFloatField(0);
	}
	catch (CppSQLite3Exception ex) {
		val = JCOFFSET_ERROR;
	}

	return val;
}

int JcOffsetDB::OffsetTime(char* ctime, int len, const char* band, char dut) {
	std::string sSuffix = dut == 0 ? "_A" : "_B";

	std::string sTable = "JC_CALIBATION_TIME";
	std::string sColomn(band);
	sColomn += sSuffix;
	std::string sql = "select time from " + sTable + " where port = '" + sColomn + "'";

	//sqlite3_stmt* pStmt;
	//int s = sqlite3_prepare(m_pConn, sql.c_str(), -1, &pStmt, NULL);
	//const char* val;
	//if (sqlite3_step(pStmt) == SQLITE_ROW) {
	//	//val = sqlite3_column_double(pStmt, 0);
	//	val = (const char*)sqlite3_column_text(pStmt, 0);
	//	int val_len = strlen(val);
	//	len = len > val_len ? val_len : len;
	//	memcpy(ctime, val, len);
	//}
	//sqlite3_finalize(pStmt);

	try{
		CppSQLite3Query query = m_pSQLite->execQuery(sql.c_str());
		if (query.eof() || query.numFields() < 1)
		{
			return JCOFFSET_ERROR;
		}

		const char* val = query.getStringField(0);
		int val_len = strlen(val);
		len = len > val_len ? val_len : len;
		memcpy(ctime, val, len);

		return 0;
	}
	catch (CppSQLite3Exception ex) {
		return JCOFFSET_ERROR;
	}	
}

//存储校准数据
int JcOffsetDB::Store_v2(char tx_or_rx, const char* band, char dut, char coup,
						 char real_or_dsp, double tx, const double* val, int num) {
	std::string sSuffix = dut == 0 ? "_A" : "_B";
	if (tx_or_rx == OFFSET_TX)
		sSuffix += (coup == 0 ? "_TX1" : "_TX2");
	//"JC_TX_OFFSET_ALL" : "JC_RX_OFFSET_ALL";
	std::string stable = tx_or_rx == OFFSET_TX ? m_tx_offset_table : m_rx_offset_table;
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

	int index = tx_or_rx == OFFSET_TX ? 3 : 1;
	int col_count = GetColCount(stable.c_str());
	if (col_count > 0 && col_count < (num + index)) {
		for (int i = col_count - index + 1; i <= num; i++) {
			bool b = ExtendTable(stable.c_str(), i);
			if (!b) {
				Util::logged("Store_v2: ExtendTable Error");
				return JCOFFSET_ERROR;
			}
		}
	}
	//开始存储，按列名写入，注：未列入的列名默认值为0
	std::string sql = "insert or replace into [" + stable + "] " + ss_freq.str() + " values " + ss_val.str();

	//sqlite3_stmt* pstmt;
	//int result1 = sqlite3_prepare(m_pConn, sql.c_str(), -1, &pstmt, NULL);
	//int result2 = sqlite3_step(pstmt);
	//sqlite3_finalize(pstmt);
	//if (result2 == SQLITE_DONE)
	//	return 0;
	//else {
	//	Util::logging("==> Store_v2: error(%d - %d)\r\n%s\r\n", result1, result2, sql.c_str());
	//	Util::logged("Store_v2: error(%d - %d)", result1, result2);
	//	return JCOFFSET_ERROR;
	//}

	try {
		m_pSQLite->execDML(sql.c_str());
		return 0;
	}
	catch (CppSQLite3Exception ex) {
		Util::logging("==> Store_v2: error: %d\r\n%s\r\n", ex.errorCode(), sql);
		Util::logged("Store_v2: error: %s", ex.errorMessage());
		return JCOFFSET_ERROR;
	}
}

double JcOffsetDB::OffsetTx_dpx(const char* band, uint8_t dut, uint8_t coup, uint8_t real_or_dsp, double freq_mhz, double tx_dbm) {
	char table[] = "JC_TX_OFFSET_DPX";
	char port[64] = { 0 };
	sprintf_s(port, "%s_P%d_TX%d", band, (int)dut, (int)coup + 1);

	char sql[256] = { 0 };
	sprintf_s(sql,
		"select Max_val, Min_val from "
		"(select max([Power]) Max_val from [%s] where [Power] <= %lf and [Port] = '%s' and [Dsp] = %d), "
		"(select min([Power]) Min_val from [%s] where [Power] >= %lf and [Port] = '%s' and [Dsp] = %d)",
		table, tx_dbm, port, (int)real_or_dsp, table, tx_dbm, port, (int)real_or_dsp);
	double p1, p2;
	if (GetSqlVal(sql, p1, p2))
		return JCOFFSET_ERROR;

	memset(sql, 0, sizeof(sql));
	sprintf_s(sql,
		"select Max_val, Min_val from "
		"(select max([Frequency]) Max_val from [%s] where [Frequency] <= %lf and [Port] = '%s' and [Dsp] = %d), "
		"(select min([Frequency]) Min_val from [%s] where [Frequency] >= %lf and [Port] = '%s' and [Dsp] = %d)",
		table, freq_mhz, port, (int)real_or_dsp, table, freq_mhz, port, (int)real_or_dsp);
	double f1, f2;
	if (GetSqlVal(sql, f1, f2))
		return JCOFFSET_ERROR;

	memset(sql, 0, sizeof(sql));
	sprintf_s(sql, 
		"select Val1, Val2 from "
		"(select [Value] Val1 from [%s] where [Frequency] = %lf and [Power] = %lf and [Port] = '%s' and [Dsp] = %d), "
		"(select [Value] Val2 from [%s] where [Frequency] = %lf and [Power] = %lf and [Port] = '%s' and [Dsp] = %d)",
		table, f1, p1, port, (int)real_or_dsp, table, f2, p1, port, (int)real_or_dsp);
	double y1, y2;
	if (GetSqlVal(sql, y1, y2))
		return JCOFFSET_ERROR;

	memset(sql, 0, sizeof(sql));
	sprintf_s(sql,
		"select Val1, Val2 from "
		"(select [Value] Val1 from [%s] where [Frequency] = %lf and [Power] = %lf and [Port] = '%s' and [Dsp] = %d), "
		"(select [Value] Val2 from [%s] where [Frequency] = %lf and [Power] = %lf and [Port] = '%s' and [Dsp] = %d)",
		table, f1, p2, port, (int)real_or_dsp, table, f2, p2, port, (int)real_or_dsp);
	double z1, z2;
	if (GetSqlVal(sql, z1, z2))
		return JCOFFSET_ERROR;

	double offset1 = SumSlope(freq_mhz, f1, y1, f2, y2);
	double offset2 = SumSlope(freq_mhz, f1, z1, f2, z2);
	return SumSlope(tx_dbm, p1, offset1, p2, offset2);
}

double JcOffsetDB::OffsetRx_dpx(const char* band, uint8_t dut, double freq_now) {
	char table[] = "JC_RX_OFFSET_DPX";
	char port[64] = { 0 };
	sprintf_s(port, "%s_P%d", band, (int)dut);

	char sql[256] = { 0 };
	sprintf_s(sql,
		"select Max_val, Min_val from "
		"(select max([Frequency]) Max_val from [%s] where [Frequency] <= %lf and [Port] = '%s'), "
		"(select min([Frequency]) Min_val from [%s] where [Frequency] >= %lf and [Port] = '%s')",
		table, freq_now, port, table, freq_now, port);
	double f1, f2;
	if (GetSqlVal(sql, f1, f2))
		return JCOFFSET_ERROR;

	memset(sql, 0, sizeof(sql));
	sprintf_s(sql, "select Val1, Val2 from "
		"(select [Value] Val1 from [%s] where [Frequency] = %lf and Port = '%s'), "
		"(select [Value] Val2 from [%s] where [Frequency] = %lf and Port = '%s')",
		table, f1, port, table, f2, port);
	double y1 = 0, y2 = 0;
	if (GetSqlVal(sql, y1, y2))
		return JCOFFSET_ERROR;

	return SumSlope(freq_now, f1, y1, f2, y2);
}

int JcOffsetDB::Store_dpx(uint8_t tx_or_rx, const char* band, uint8_t dut, uint8_t coup,
						  uint8_t real_or_dsp, double tx, const double* freqs, const double* vals, int num) {
	std::string table = "";
	char header[64] = { 0 };
	char port[64] = { 0 };
	
	if (tx_or_rx == OFFSET_TX) {
		//sprintf_s(port, "dpx_P%d_TX%d", (int)dut, (int)coup + 1);
		sprintf_s(port, "'dpx_P%d_TX%d'", (int)dut, (int)coup + 1);
		sprintf_s(header, "Port,Dsp,Power,Frequency,Value");
		table = m_tx_dpx_offset_table;
	}
	else {
		//sprintf_s(port, "dpx_P%d", (int)dut);
		sprintf_s(port, "'dpx_P%d'", (int)dut);
		sprintf_s(header, "Port,Frequency,Value");
		table = m_rx_dpx_offset_table;
	}

	try {
		for (int i = 0; i < num; ++i) {
			char values[256] = { 0 };
			if (tx_or_rx == OFFSET_TX) {
				sprintf_s(values,"%s,%d,%lf,%lf,%lf", port, (int)real_or_dsp, tx, freqs[i], vals[i]);
			}
			else {
				sprintf_s(values, "%s,%lf,%lf", port, freqs[i], vals[i]);
			}
			char sql[256] = { 0 };
			sprintf_s(sql, "insert or replace into [%s] (%s) values (%s)", table.c_str(), header, values);
		
			m_pSQLite->execDML(sql);
		}		
		return 0;
	}
	catch (CppSQLite3Exception ex) {
		Util::logging("==> Store_dpx: error: %d\r\n", ex.errorCode());
		Util::logged("Store_dpx: error: %s", ex.errorMessage());
		return JCOFFSET_ERROR;
	}
}

//存储vco校准数据
int JcOffsetDB::Store_vco_single(const char* band, char dut, double val) {
	std::string sSuffix = dut == 0 ? "_A" : "_B";
	std::string sColomn(band);
	sColomn += sSuffix;

	//std::string sql = "insert or replace into JC_VCO_OFFSET_ALL (port,vco) values ('" + sColomn + "'," + std::to_string(val) + ")";
	//sqlite3_stmt* pstmt;
	//sqlite3_prepare(m_pConn, sql.c_str(), -1, &pstmt, NULL);
	//int result = sqlite3_step(pstmt);
	//sqlite3_finalize(pstmt);
	//if (result == SQLITE_DONE)
	//	return 0;
	//else
	//{
	//	Util::logging("==> Save Vco error: %d\r\n%s\r\n", result, sql.c_str());
	//	Util::logged("Save Vco error: %d", result);
	//	return JCOFFSET_ERROR;
	//}

	char sql[128] = { 0 };
	sprintf_s(sql, "insert or replace into JC_VCO_OFFSET_ALL (port,vco) values ('%s','%lf')", sColomn.c_str(), val);
	try{
		m_pSQLite->execScalar(sql);
		return 0;
	}
	catch (CppSQLite3Exception ex) {
		Util::logging("==> Save Vco error: %d\r\n%s\r\n", ex.errorCode(), sql);
		Util::logged("Save Vco error: %s", ex.errorMessage());
		return JCOFFSET_ERROR;
	}
}

int JcOffsetDB::Store_calibration_time(const char* band, char dut, const char* val) {
	std::string sSuffix = dut == 0 ? "_A" : "_B";
	std::string sColomn(band);
	sColomn += sSuffix;

	//std::string sql = "insert or replace into JC_CALIBATION_TIME (port,time) values ('" + sColomn + "','" + std::string(val) + "')";
	//sqlite3_stmt* pstmt;
	//sqlite3_prepare(m_pConn, sql.c_str(), -1, &pstmt, NULL);
	//int result = sqlite3_step(pstmt);
	//sqlite3_finalize(pstmt);
	//if (result == SQLITE_DONE)
	//	return 0;
	//else
	//{
	//	//Util::logging("==> Save time error: %d\r\n%s\r\n", result, sql.c_str());
	//	//Util::logged("Save time error: %d", result);
	//	return JCOFFSET_ERROR;
	//}

	char sql[128] = { 0 };
	sprintf_s(sql, "insert or replace into JC_CALIBATION_TIME (port,time) values ('%s','%s')", sColomn.c_str(), val);
	try{
		m_pSQLite->execScalar(sql);
		return 0;
	}
	catch (CppSQLite3Exception ex) {
		Util::logging("==> Save Vco error: %d\r\n%s\r\n", ex.errorCode(), sql);
		Util::logged("Save Vco error: %s", ex.errorMessage());
		return JCOFFSET_ERROR;
	}
}

int JcOffsetDB::FreqBand_continuous(uint8_t tx_or_rx, const char* band, double& f_start, double& f_stop) {
	if (m_offset_mode != continuous_offset_mode)
		return -1;

	char sql[1024] = { 0 };
	if (tx_or_rx == OFFSET_TX)
		sprintf_s(sql, "select [tx_start],[tx_end] from [%s] where band = '%s'", m_band_info_table.c_str(), band);
	else
		sprintf_s(sql, "select [rx_start],[rx_end] from [%s] where band = '%s'", m_band_info_table.c_str(), band);

	if (GetSqlVal(sql, f_start, f_stop))
		return JCOFFSET_ERROR;

	return 0;
}

int JcOffsetDB::FreqBand_discontinuous(uint8_t tx_or_rx, double freq_mhz, const char* band,
									   double &f1, double &f2, double &index1, double &index2) {
	if (m_offset_mode != discontinuous_offset_mode)
		return -1;

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

	//查找f1,f2对应的index
	ss.str("");
	ss << "select A,B from (select [ID] A from [" << stable << "] where " + scolomn +
		"=" << f1 << "),(select [ID] B from [" << stable << "] where " + scolomn +
		"=" << f2 << ")";
	if (GetSqlVal(ss.str().c_str(), index1, index2))
		return JCOFFSET_ERROR;

	return 0;
}

//计算斜率
double JcOffsetDB::SumSlope(double v, double x1, double y1, double x2, double y2) {
	if (x1 == x2)
		return y1;
	else
		return (y2 - y1) / (x2 - x1) * v + (y1 * x2 - y2 * x1) / (x2 - x1);
}

//sqlite语句执行
int JcOffsetDB::GetSqlVal(const char* sql, double& a1, double& a2){
	//sqlite3_stmt* pStmt;
	//int r = 0;
	//sqlite3_prepare(m_pConn, strsql, -1, &pStmt, NULL);
	//if (sqlite3_step(pStmt) == SQLITE_ROW) {
	//	if (sqlite3_column_count(pStmt) == 2) {
	//		a1 = sqlite3_column_double(pStmt, 0);
	//		a2 = sqlite3_column_double(pStmt, 1);
	//	}
	//	else
	//		r = JCOFFSET_ERROR;
	//}
	//else
	//	r = JCOFFSET_ERROR;
	//sqlite3_finalize(pStmt);
	//return r;

	try {
		CppSQLite3Query query = m_pSQLite->execQuery(sql);
		if (query.eof() || query.numFields() != 2) {
			return JCOFFSET_ERROR;
		}
		a1 = query.getFloatField(0);
		a2 = query.getFloatField(1);
		return 0;
	}
	catch (CppSQLite3Exception ex) {
		return JCOFFSET_ERROR;
	}	
}

bool JcOffsetDB::IsExist(const char* table) {
	//std::string sql_drop = "DROP TABLE JC_BAND2_INFO";
	//判断表JC_BAND2_INFO是否存在
	char sql[128] = { 0 };
	sprintf_s(sql, "select count(*) as c from Sqlite_master  where type = 'table' and name = '%s'", table);
	int n = 0;

	//sqlite3_stmt* pstmt = NULL;
	//sqlite3_prepare(m_pConn, sql, -1, &pstmt, NULL);
	//if (sqlite3_step(pstmt) == SQLITE_ROW){
	//	n = sqlite3_column_int(pstmt, 0);
	//}
	//sqlite3_finalize(pstmt);

	try{
		n = m_pSQLite->execScalar(sql);
	}
	catch (CppSQLite3Exception ex) {
		n = -1;
	}
	return (n > 0);
}

bool JcOffsetDB::ExecSql(const char* sql) {
	//sqlite3_stmt* pstmt;
	//sqlite3_prepare(m_pConn, sql, -1, &pstmt, NULL);
	//int resulte = sqlite3_step(pstmt);
	//sqlite3_finalize(pstmt);
	//return (resulte == SQLITE_DONE);

	try{
		m_pSQLite->execDML(sql);
		return true;
	}
	catch (CppSQLite3Exception ex) {
		return false;
	}
}

int JcOffsetDB::GetColCount(const char* table) {
	char sql[128] = { 0 };
	sprintf_s(sql, "select * from '%s' limit 1", table);
	int n = 0;

	//sqlite3_stmt* pstmt = NULL;
	//sqlite3_prepare(m_pConn, sql, -1, &pstmt, NULL);
	//n = sqlite3_column_count(pstmt);
	//sqlite3_finalize(pstmt);

	try{
		n = m_pSQLite->execScalar(sql);
	}
	catch (CppSQLite3Exception ex) {
		n = -1;
	}

	return n;
}

bool JcOffsetDB::ExtendTable(const char* table, int num) {
	char sql[128] = { 0 };
	sprintf_s(sql, "ALTER TABLE %s ADD COLUMN '%d' real", table, num);
	return ExecSql(sql);
}