#include "utility.h"

/**
* unsigned short ת��Ϊ string
* @todo implement caching for better performance
* @param WORD(unsigned short) value, ����Ҫ����ת����ֵ.
* @return string, ����ת�����ֵ.
*/
std::string uInt2String(unsigned short value) {
	std::stringstream ss;
	ss << value;
	std::string ret;
	ss >> ret;
	return ret;
}

/**
* ���ص�ǰʱ��
* @todo
* @param, empty.
* @return string, ���ظ�ʽ����ǰʱ��.
*/
std::string logTime() {
	SYSTEMTIME systime;
	GetLocalTime(&systime);
	std::string ret = "";
	ret += uInt2String(systime.wYear) + "-";
	ret += uInt2String(systime.wMonth) + "-";
	ret += uInt2String(systime.wDay) + " ";
	ret += uInt2String(systime.wHour) + ":";
	ret += uInt2String(systime.wMinute) + ":";
	ret += uInt2String(systime.wSecond) + ".";
	ret += uInt2String(systime.wMilliseconds) + " ";
	return ret;
}

/**
* �Ը�����val��������
* @todo
* @param double, Ҫ�������������ֵ
* @return int, ���������������
*/
int convertToInt(double val)
{
	return (int)(val + 0.5);
}
