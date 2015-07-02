#ifndef VORTEXLAYER_H
#define VORTEXLAYER_H

#include "cocos2d.h"
#include "3d/CCMesh.h"
NS_CC_BEGIN

//������ʾ��
class VortexLayer : public  cocos2d::Layer
{
	//����ṹ
	struct VortexVertexStruct
	{
		Vec3 Position;
		Vec2 TexUV;
	}
	;
public:
	VortexLayer();
	~VortexLayer();
	//��ʼ����ǰ��
    virtual bool init();  
    CREATE_FUNC(VortexLayer);
	//������Ⱦ
	virtual void draw(Renderer* renderer, const Mat4 &transform, uint32_t flags);
    // a selector callback
    void menuCloseCallback(Ref* sender);
public:
	//������ȾĿ���
	void setTargetLayer(CCLayer*	pLayer)
	{
		m_TargetLayer = pLayer;
		if(m_pTarget)
		{
			m_pTarget->addChild(m_TargetLayer);
		}
	}
	//������ת�ĽǶ�
	void setAngle( float f )
	{
		m_angle = f;
	}
	float getAngel() const
	{
		return m_angle;
	}
	//������ת�İ뾶
	void setRadius( float f )
	{
		m_radius = f;
	}
	float getRadius()
	{
		return m_radius;
	}

	float						m_angleSpeed;	//�Ƕ��ٶ�
	float						m_radiusSpeed;	//�뾶�ٶ�
	float						m_Time;			//��ǰʱ��
	float						m_maxTime;		//���ʱ��

private:

	float						m_angle;		//�Ƕ�
	float						m_radius;		//�뾶
	Mesh*						m_RenderMesh;	//��Ⱦģ��
	MeshCommand*				m_pMeshCommand;	//��Ⱦ����
	BlendFunc					m_BlendFunc;	//��Ⱦ״̬
    VortexVertexStruct*			m_VertexArray;	//���õ�ģ�Ͷ���
	GLushort*					m_IndiceArray;	//����
	GLProgramState*			    m_ProgramState;	//Shader������
	CCLayer*					m_TargetLayer;	//Ŀ��
    CCRenderTexture*			m_pTarget;		//��ȾĿ��
	timeval						m_lasttime;		//��¼�ϴ�ʱ��

}
;
NS_CC_END
#endif