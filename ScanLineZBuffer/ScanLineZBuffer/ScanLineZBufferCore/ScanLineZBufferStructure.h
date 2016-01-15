#ifndef __SCANLINEZBUFFER_STRUCTURE_H__
#define __SCANLINEZBUFFER_STRUCTURE_H__


// �㶨��
struct Point {
	double x, y, z;
};


// ��ɫ����
struct ColorType {
	GLubyte r, g, b, a;
};

// ��߱� Ԫ��, ������¼һ���߱�Ԫ��
struct ActiveEdgeTableElem
{
	double xl; // �󽻵��x����
	double dxl; // (�󽻵����)������ɨ���߽����x����֮��
	int dyl; // �Ժ��󽻵����ڱ��ཻ��ɨ������Ϊ��ֵ���Ժ�����ÿ����һ��ɨ���߼�1
	// �ұߵĽ����������Ӧ��������������߽�����ͬ
	// double xr;
	// double dxr;
	// int dyr;
	double zl; // �󽻵㴦���������ƽ������ֵ
	double dzx; // ��ɨ���������߹�һ������ʱ�����������ƽ����������������ƽ�淽�̣�dzx = -a/c(c!=0)
	double dzy; // ��y���������ƹ�һ��ɨ����ʱ�����������ƽ����������������ƽ�淽�̣�dzy = b/c(c!=0)
	int id; // ��������ڵĶ���εı��
};

// �������α� || �����α� Ԫ��
struct PolygonTableElem
{
	double a, b, c, d; // ��������ڵ�ƽ�淽��ϵ��,ax+by+cz+d=0
	int id; // ����εı��
	int dy; // ����ο�Խ��ʣ��ɨ������Ŀ 
	std::vector<ActiveEdgeTableElem> activeEdgeTable; // ��Ի����α��еĻ�߱�
	ColorType color;
};

// ����߱� Ԫ��
struct EdgeTableElem
{
	double x; // �ߵ��϶˵��x����
	double dx; // ��������ɨ���߽����x�����dx (-1/k)
	int dy; // �߿�Խ��ɨ������Ŀ
	int id; // ����������εı��
};


#endif //  __SCANLINEZBUFFER_STRUCTURE_H__
