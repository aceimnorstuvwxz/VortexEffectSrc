#ifndef VORTEXLAYER_H
#define VORTEXLAYER_H

#include "cocos2d.h"
#include "3d/CCMesh.h"
NS_CC_BEGIN

//旋涡显示类
class VortexLayer : public  cocos2d::Layer
{
	//顶点结构
	struct VortexVertexStruct
	{
		Vec3 Position;
		Vec2 TexUV;
	}
	;
public:
	VortexLayer();
	~VortexLayer();
	//初始化当前层
    virtual bool init();  
    CREATE_FUNC(VortexLayer);
	//重载渲染
	virtual void draw(Renderer* renderer, const Mat4 &transform, uint32_t flags);
    // a selector callback
    void menuCloseCallback(Ref* sender);
public:
	//设置渲染目标层
	void setTargetLayer(CCLayer*	pLayer)
	{
		m_TargetLayer = pLayer;
		if(m_pTarget)
		{
			m_pTarget->addChild(m_TargetLayer);
		}
	}
	//设置旋转的角度
	void setAngle( float f )
	{
		m_angle = f;
	}
	float getAngel() const
	{
		return m_angle;
	}
	//设置旋转的半径
	void setRadius( float f )
	{
		m_radius = f;
	}
	float getRadius()
	{
		return m_radius;
	}

	float						m_angleSpeed;	//角度速度
	float						m_radiusSpeed;	//半径速度
	float						m_Time;			//当前时间
	float						m_maxTime;		//最大时间

private:

	float						m_angle;		//角度
	float						m_radius;		//半径
	Mesh*						m_RenderMesh;	//渲染模型
	MeshCommand*				m_pMeshCommand;	//渲染队列
	BlendFunc					m_BlendFunc;	//渲染状态
    VortexVertexStruct*			m_VertexArray;	//所用的模型顶点
	GLushort*					m_IndiceArray;	//索引
	GLProgramState*			    m_ProgramState;	//Shader管理器
	CCLayer*					m_TargetLayer;	//目标
    CCRenderTexture*			m_pTarget;		//渲染目标
	timeval						m_lasttime;		//记录上次时间

}
;
NS_CC_END
#endif