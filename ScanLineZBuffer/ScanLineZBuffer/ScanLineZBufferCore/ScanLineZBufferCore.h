#ifndef __SCANLINEZBUFFERCORE_H__
#define __SCANLINEZBUFFERCORE_H__

#include "../utility.h"
#include "ScanLineZBufferStructure.h"

class ScanLineZBufferCore 
{

public:
	ScanLineZBufferCore(PolyMesh &mesh);
	~ScanLineZBufferCore();
	void Run();
	void SetScene();
	void ProjectModelToScene();
	void BuildPolygonAndEdgeTable();
	void ScanLineZBufferAlgorithm();
	void DrawFrame();

//��������
	void addEdgeToActiveEdgeTable(int y, const PolygonTableElem *pte, int addType);

private:
	PolyMesh *pMesh;
	int m_viewport[4];		// �ӿھ���, [0, 0, width, height]
	double m_modelview[16];	// ģ�ͱ任����
	double m_projection[16];	// ͶӰ����
	std::vector<Point> m_projectionVertex;	// ͶӰ����

	double *zBuffer;// zBuffer����
	ColorType *colorBuffer; // ��ɫ����
	GLubyte *frame; // ��ǰͼ�� 
	std::vector<std::vector<PolygonTableElem>> m_polygonTable; // �������α�
	std::vector<std::vector<EdgeTableElem>> m_edgeTable; // ����߱�
	std::vector<PolygonTableElem> m_activePolygonTable; // �����α�

};


#endif // __SCANLINEZBUFFERCORE_H__