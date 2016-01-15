#include "../utility.h"
#include "ScanLineZBufferCore.h"
#include <utility>

// �Ի����α�����ıȽϺ���,
bool cmp(const ActiveEdgeTableElem &lhs, const ActiveEdgeTableElem &rhs)
{
	if (convertToInt(lhs.xl) < convertToInt(rhs.xl)) return true;
	else if (convertToInt(lhs.xl) == convertToInt(rhs.xl)) {
		if (convertToInt(lhs.dxl) < convertToInt(rhs.dxl))
			return true;
	}
	return false;
}

ScanLineZBufferCore::ScanLineZBufferCore(PolyMesh &mesh)
{
	srand(time(NULL));
	this->pMesh = &mesh;
	this->zBuffer = NULL;
	this->colorBuffer = NULL;
	this->frame = NULL;
}

ScanLineZBufferCore::~ScanLineZBufferCore()
{
	delete[] zBuffer;
	delete[] colorBuffer;
	delete[] frame;

}

// ���ó�������
void ScanLineZBufferCore::SetScene()
{
	// �����ɫ����
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// �õ��ӿ�
	glGetIntegerv(GL_VIEWPORT, m_viewport);
	int width = m_viewport[2];
	int height = m_viewport[3];
	
	zBuffer= new double[width];
	colorBuffer = new ColorType[width];

	frame = new GLubyte[width*height*3];
	for (auto i = 0; i < width*height*3; i++)
		frame[i] = 0; // ��ɫ	
	
	m_projectionVertex.resize(pMesh->n_vertices());
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	GLfloat aspectRatio = (GLfloat)width / (GLfloat)height;
	// ͸��ͶӰ
	//gluPerspective(50.0, aspectRatio, 1e-2, 1e6);
	// ����ͶӰ
	GLfloat nRange = 4;
	//glOrtho(-nRange*aspectRatio, nRange*aspectRatio, -nRange, nRange, -200, 200);
	if (width >= height) glOrtho(-nRange*aspectRatio, nRange*aspectRatio, -nRange, nRange, -nRange, nRange);
	else glOrtho(-nRange, nRange, -nRange/aspectRatio, nRange/aspectRatio, -nRange, nRange);
	
	// �õ�ģ�ͱ任����
	glGetDoublev(GL_MODELVIEW_MATRIX, m_modelview);
	// �õ�ͶӰ����
	glGetDoublev(GL_PROJECTION_MATRIX, m_projection);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glEnable(GL_POLYGON_SMOOTH);
}
void ScanLineZBufferCore::DrawFrame()

{
	int width = m_viewport[2];
	int height = m_viewport[3];
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, width, 0.0, height);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glRasterPos2i(0, 0);
	glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, frame);
	/*
	glPointSize(3.f);
	glColor3f(1., 0., 0.);
	glBegin(GL_POINTS);
	for (int i = 0; i < m_projectionVertex.size(); i++) 
	{		
		//std::cout << m_projectionVertex[i].x << " " << m_projectionVertex[i].y << " " << m_projectionVertex[i].z << std::endl;
		glVertex3d(m_projectionVertex[i].x, m_projectionVertex[i].y, m_projectionVertex[i].z);
	}
	glEnd();
	*/
	glPopMatrix();
}

void ScanLineZBufferCore::Run()
{
	// ���ó���
	SetScene();
	// ͶӰģ�͵���Ļ
	ProjectModelToScene();
	// �����������α�ͷ���߱�
	BuildPolygonAndEdgeTable();
	// ScanLineZbuffer�����㷨
	ScanLineZBufferAlgorithm();
	// ����
	DrawFrame();
}

// ͶӰmeshģ�͵����굽��Ļ��
void ScanLineZBufferCore::ProjectModelToScene()
{
	int height = m_viewport[3];
	int width = m_viewport[4];
	for (auto vertex_it = pMesh->vertices_begin(); vertex_it != pMesh->vertices_end(); ++vertex_it)
	{	
		int idx = vertex_it.handle().idx();
		auto point = pMesh->point(vertex_it.handle());
		auto x = point.data()[0], y = point.data()[1], z = point.data()[2];
		//std::cout << "���ε�����: " << x << " " << y << " " << z << std::endl; 
		gluProject(x, y, z, m_modelview, m_projection, m_viewport, &m_projectionVertex[idx].x, &m_projectionVertex[idx].y, &m_projectionVertex[idx].z);
		m_projectionVertex[idx].x = convertToInt(m_projectionVertex[idx].x);
		m_projectionVertex[idx].y = convertToInt(m_projectionVertex[idx].y);
		m_projectionVertex[idx].z = m_projectionVertex[idx].z * 1000;
		//std::cout << "ͶӰ����: " << m_projectionVertex[idx].x << " " << m_projectionVertex[idx].y << " " << m_projectionVertex[idx].z << std::endl; 
	}
	//std::cout << pMesh->n_vertices() << std::endl;
}


// �����������αߺͷ���߱�
void ScanLineZBufferCore::BuildPolygonAndEdgeTable()
{
	int width = m_viewport[2];
	int height = m_viewport[3];
	m_polygonTable.resize(height, std::vector<PolygonTableElem>());
	m_edgeTable.resize(height, std::vector<EdgeTableElem>());
	for (auto i = 0; i < height; i++) 
	{
		m_polygonTable[i].clear();
		m_edgeTable[i].clear();
	}
	PolygonTableElem cpte;
	// ����ÿ������Σ����뵽����α�ͱ߱�
	for (auto f_it = pMesh->faces_begin(); f_it != pMesh->faces_end(); ++f_it)
	{
		//std::cout << "��ǰ����� " << f_it->idx() << std::endl;
		// ��������ε�ÿ���ߣ��������߱�
		for (auto h_it = pMesh->fh_begin(f_it.handle()); h_it != pMesh->fh_end(f_it.handle()); ++h_it)
		{
			EdgeTableElem cete; 
			cete.id = f_it->idx();
			auto point_a_idx = (pMesh->from_vertex_handle(h_it.handle())).idx(); 
			auto point_b_idx = (pMesh->to_vertex_handle(h_it.handle())).idx();

			// �жϱ���������˭��yֵ����
			int max_idx, min_idx;
			if (m_projectionVertex[point_a_idx].y > m_projectionVertex[point_b_idx].y)
			{
				max_idx = point_a_idx;
				min_idx = point_b_idx;
			}
			else
			{
				max_idx = point_b_idx;
				min_idx = point_a_idx;
			}
			// ����Ϊ�ߵ��϶˵�����
			cete.x = m_projectionVertex[max_idx].x;
			// ���ñ߿�Խ��ɨ������Ŀ �� ����x���� ʵ��Ϊ -cet.dy/cet.dx 
			cete.dy = m_projectionVertex[max_idx].y - m_projectionVertex[min_idx].y;
			// �Թ�ˮƽ�߲�����
			if (abs(cete.dy) < 1e-8) continue;
			cete.dx = -(m_projectionVertex[min_idx].x - m_projectionVertex[max_idx].x) / (m_projectionVertex[min_idx].y - m_projectionVertex[max_idx].y);

			// ��ӵ�����߱�
			// //std::cout << logTime() << " " << convertToInt(m_projectionVertex[max_idx].y) << std::endl;
			m_edgeTable[convertToInt(m_projectionVertex[max_idx].y)].push_back(cete);
			//std::cout << ">>>> �����" << h_it->idx() << std::endl;
		}

		// ��ǰ����ε�Y�������Сֵ
		double min_y, max_y;
		// ��ȡ������ϵ�һ�����㣬��ʼ�������Y�������Сֵ
		min_y = max_y = m_projectionVertex[(pMesh->fv_begin(f_it.handle()))->idx()].y;
		// ȡ����ǰ������������㣬��ƽ��a,b,c,d
		std::vector<Point> poly_point;
		// �����ǰ����ε�Y�������Сֵ
		for (auto v_it = pMesh->fv_begin(f_it.handle()); v_it != pMesh->fv_end(f_it.handle()); ++v_it)
		{
			auto point = m_projectionVertex[v_it->idx()];
			// //std::cout << logTime() << " ������" << point.y << std::endl;
			poly_point.push_back(point);
			min_y = mmin(min_y, point.y);
			max_y = mmax(max_y, point.y);
		}
		// if (poly_point.size() < 3) //std::cout << logTime() << " ������ϵ����������3" << std::endl;
		
		// ��ax+by+cz+d=0�����÷������β���
		cpte.a = (poly_point[0].y - poly_point[1].y)*(poly_point[0].z - poly_point[2].z)
			- (poly_point[0].y - poly_point[2].y)*(poly_point[0].z - poly_point[1].z);
		cpte.b = (poly_point[0].x - poly_point[2].x)*(poly_point[0].z - poly_point[1].z)
			- (poly_point[0].z - poly_point[2].z)*(poly_point[0].x - poly_point[1].x);
		cpte.c = (poly_point[0].x - poly_point[1].x)*(poly_point[0].y - poly_point[2].y)
			- (poly_point[0].x - poly_point[2].x)*(poly_point[0].y - poly_point[1].y);
		cpte.d = -cpte.a*poly_point[0].x - cpte.b*poly_point[0].y - cpte.c*poly_point[0].z;
		//std::cout << ">>>> ƽ�淽�̲���" << cpte.a << " " << cpte.b << " " << cpte.c << " " << cpte.d << std::endl;
		// ���ö���ε�id
		cpte.id = f_it->idx();
		// ���ö���ε���ɫ
		cpte.color.r = (rand() % 20) + 235;
		cpte.color.g = (rand() % 20) + 145;
		cpte.color.b = 0;

		// ���뵽�������α�
		//std::cout << std::endl;
		cpte.dy = convertToInt(max_y) - convertToInt(min_y);
		m_polygonTable[convertToInt(max_y)].push_back(cpte);
	}
}

void ScanLineZBufferCore::ScanLineZBufferAlgorithm()
{
	int width = m_viewport[2];
	int height = m_viewport[3];

	// �����αߺͻ�߱��ʼ��Ϊ��
	m_activePolygonTable.clear();

	// ------ɨ��˳����ϵ��£��ڴ���������һ��ɨ����֮ǰ�������α�ͱ߱��ǿյ�
	for (auto y = height - 1; y >= 0; --y)
	{
		//std::cout << "��ǰ����ɨ���� :" << scan_line_pos << " " << std::endl;
		// ֡��������ʼ�����óɱ���ɫ
		// z��������ʼ�����ó���Сֵ����ʾ���ӵ���Զ
		for (auto i = 0; i < width; i++)
		{
			colorBuffer[i].r = colorBuffer[i].g = colorBuffer[i].b = 0;
			zBuffer[i] = -100000;
		}
		for (auto cpt_it = m_polygonTable[y].begin(); cpt_it != m_polygonTable[y].end(); ++cpt_it)
			m_activePolygonTable.push_back(*cpt_it);
		//if (m_activePolygonTable.size() != 0)
		//	printf("��ǰɨ����%d  ��ǰ�����θ���%d\n", y, m_activePolygonTable.size());
		bool flag = false;
		for (auto apt_it = m_activePolygonTable.begin(); apt_it != m_activePolygonTable.end();) {
			if (apt_it->dy < 0) {
				apt_it = m_activePolygonTable.erase(apt_it);
			} else {
				// ������
				for (auto cet_it = m_edgeTable[y].begin(); cet_it != m_edgeTable[y].end();) {
					if (cet_it->id == apt_it->id) { // �˱����ڴ˶����
						ActiveEdgeTableElem activeEdgeElem;
						activeEdgeElem.xl = cet_it->x;
						if (abs(cet_it->dy) <= 0) {
							activeEdgeElem.dxl = 0;
						} else {
							activeEdgeElem.dxl = cet_it->dx;
						}
						activeEdgeElem.dyl = cet_it->dy;
						if (abs(apt_it->c) < 1e-8) { // XOYƽ��
							activeEdgeElem.zl = 0;
							activeEdgeElem.dzx = 0;
							activeEdgeElem.dzy = 0;
						} else {
							activeEdgeElem.zl = -(apt_it->d + apt_it->a*cet_it->x + apt_it->b*y) / apt_it->c;
							activeEdgeElem.dzx = -(apt_it->a / apt_it->c);
							activeEdgeElem.dzy = apt_it->b / apt_it->c;
						}
						apt_it->activeEdgeTable.push_back(activeEdgeElem);
						cet_it = m_edgeTable[y].erase(cet_it);
					} else cet_it++;
				}
				// ɾ���Ѿ��������
				for (auto aet_it = apt_it->activeEdgeTable.begin(); aet_it != apt_it->activeEdgeTable.end();) {
					//printf("��ǰ�����%d����ߵ�dyl: %d\n", apt_it->id, aet_it->dyl);
					if (aet_it->dyl <= 0) aet_it = apt_it->activeEdgeTable.erase(aet_it);
					else ++aet_it;
				}
				//printf("��ǰ�����%d �Ļ�߱���Ŀ%d\n",apt_it->id, apt_it->activeEdgeTable.size());
				// �Ե�ǰ�����εĻ�߱���x����
				std::sort(apt_it->activeEdgeTable.begin(), apt_it->activeEdgeTable.end(), cmp);
				for (auto lo = apt_it->activeEdgeTable.begin(); lo != apt_it->activeEdgeTable.end(); lo += 2) {
					auto hi = lo + 1;
					double zx = lo->zl;
					for (int x = convertToInt(lo->xl); x < convertToInt(hi->xl); x++) {
						if (zx >= zBuffer[x]) {
							zBuffer[x] = zx;
							colorBuffer[x].r = apt_it->color.r;
							colorBuffer[x].g = apt_it->color.g;
							colorBuffer[x].b = apt_it->color.b;
						}
						zx += lo->dzx;
					}
					lo->dyl--;
					hi->dyl--;
					lo->xl += lo->dxl;
					hi->xl += hi->dxl;

					lo->zl += lo->dzx * lo->dxl + lo->dzy;
					hi->zl += hi->dzx * hi->dxl + hi->dzy;
					flag = true;
				}
				apt_it->dy--;
				++apt_it;
			}
		}
		if (flag) {
			for (int x = 0; x < width; x++) {
				// ע�����3
				frame[y*width*3 + x*3] = colorBuffer[x].r;
				frame[y*width*3 + x*3 + 1] = colorBuffer[x].g;
				frame[y*width*3 + x*3 + 2] = colorBuffer[x].b;
			}
		}
	}
	// �������
	m_projectionVertex.clear();
	m_activePolygonTable.clear();
}
