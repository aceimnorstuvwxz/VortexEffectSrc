#include "VortexLayer.h"
#include "FishLayer.h"
#include <vector>
using namespace std;

USING_NS_CC;
VortexLayer::VortexLayer():
	m_angle( 0.f )
	, m_radius( 0.f )
	, m_TargetLayer(NULL)
	, m_angleSpeed(1.5)
	, m_radiusSpeed(0.5)
	, m_Time( 0.0 )
	, m_maxTime( 2.0 )
	, m_pMeshCommand( NULL )
{

}
VortexLayer::~VortexLayer()
{
	CC_SAFE_DELETE(m_pMeshCommand);
	if(m_pTarget)
	{
		if(m_TargetLayer)
		{
			m_pTarget->removeChild(m_TargetLayer);
		}
		removeChild(m_pTarget);
	}
}

bool VortexLayer::init()
{
	gettimeofday( &m_lasttime, nullptr);


	m_BlendFunc = BlendFunc::ALPHA_NON_PREMULTIPLIED;

	// 取得文件管理器
	auto fileUtiles = FileUtils::getInstance();
	// 加载相应的Shader文件
    // 加载Vortex.vsh并取得文件内容字符串
    auto vertexFilePath = fileUtiles->fullPathForFilename("Vortex.vsh");
    auto vertSource = fileUtiles->getStringFromFile(vertexFilePath);

    // 加载Vortex.fsh并取得文件内容字符串
    auto fragmentFilePath = fileUtiles->fullPathForFilename("Vortex.fsh");
    auto fragSource = fileUtiles->getStringFromFile(fragmentFilePath);

	// 将vsh与fsh装配成一个完整的Shader文件。
    auto glprogram = GLProgram::createWithByteArrays(vertSource.c_str(), fragSource.c_str());
	glprogram->addAttribute( "a_position", GLProgram::VERTEX_ATTRIB_POSITION);
    glprogram->addAttribute( "a_texCoord", GLProgram::VERTEX_ATTRIB_TEX_COORD);
	glprogram->link();
    glprogram->updateUniforms();

	// 由Shader文件创建这个Shader
    m_ProgramState = GLProgramState::getOrCreateWithGLProgram(glprogram);
	// 给精灵设置所用的Shader
    setGLProgramState(m_ProgramState);

	// 创建渲染目标纹理
	auto  WinSize  = Director::getInstance()->getWinSize();
	m_pTarget = CCRenderTexture::create(WinSize.width, WinSize.height, kCCTexture2DPixelFormat_RGBA8888);
	m_pTarget->setClearColor(Color4F(0, 0, 0, 0));
	m_pTarget->setVisible(false);
	addChild(m_pTarget);

	//行，列数
	int		cows = 100;
	int		rows = 100;
	//每一行，列的顶点位置偏移
	float	vertexStepX = WinSize.width/cows;
	float	vertexStepY = WinSize.height/rows;
	//第一行，列的纹理坐标偏移
	float	uStep = 1.0/rows;
	float	vStep = 1.0/cows;
	//顶点，法线，色，纹理的容器
	vector<float>	positions;
	vector<float>   normals;
	vector<float>   colors;
	vector<float>   texs;

	for ( unsigned int x = 0; x <= cows; ++x )
	{
		for ( unsigned int  y = 0; y <= rows; ++y )
		{
			unsigned int offset = x + y * ( cows + 1 );

			positions.push_back(x*vertexStepX);
			positions.push_back(y*vertexStepY);
			positions.push_back(10);

			texs.push_back(x*uStep);
			texs.push_back(y*vStep);
		}
	}
	std::vector<unsigned short> tTriangle;

	for ( unsigned int x = 0; x < cows; ++x )
	{
		for ( unsigned int y = 0; y < rows; ++y )
		{
			unsigned short* ptr = &(m_IndiceArray[(x+y*cows)*6]);

			tTriangle.push_back((x+0) + (y+0)*(cows+1));
			tTriangle.push_back((x+0) + (y+1)*(cows+1));
			tTriangle.push_back((x+1) + (y+0)*(cows+1));


			tTriangle.push_back((x+0) + (y+1)*(cows+1));
			tTriangle.push_back((x+1) + (y+1)*(cows+1));
			tTriangle.push_back((x+1) + (y+0)*(cows+1));
		}
	}
	//创建模型
	m_RenderMesh = Mesh::create(positions,normals,texs,tTriangle);
	m_RenderMesh->retain();

	//设置顶点格式
    long offset = 0;
    auto attributeCount = m_RenderMesh->getMeshVertexAttribCount();
    for (auto k = 0; k < attributeCount; k++) {
        auto meshattribute = m_RenderMesh->getMeshVertexAttribute(k);
        m_ProgramState->setVertexAttribPointer(s_attributeNames[meshattribute.vertexAttrib],
                                             meshattribute.size,
                                             meshattribute.type,
                                             GL_FALSE,
											 m_RenderMesh->getVertexSizeInBytes(),
                                             (GLvoid*)offset);
        offset += meshattribute.attribSizeBytes;
    }



	//关闭按钮
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    auto closeItem = MenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        CC_CALLBACK_1(VortexLayer::menuCloseCallback,this));
    
    closeItem->setPosition(origin + Vec2(visibleSize) - Vec2(closeItem->getContentSize() / 2));
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

	return true;
}

void VortexLayer::draw(Renderer* renderer, const Mat4 &transform, uint32_t flags)
{
	//如果有渲染目标纹理，将指定的结点渲染到目标纹理

	if( m_pTarget && m_TargetLayer)
	{
		m_pTarget->clear(0,0,0,0);
		m_pTarget->begin();
		m_TargetLayer->visit();
		m_pTarget->end();
	}

	//计算时间间隔
	timeval		currtime;
	gettimeofday( &currtime, nullptr);
	float dt = (currtime.tv_sec - m_lasttime.tv_sec) + (currtime.tv_usec - m_lasttime.tv_usec) / 1000000.0f;

	if ( m_Time < m_maxTime )
	{
		setAngle( getAngel()+m_angleSpeed*dt );
		setRadius( getRadius()+m_radiusSpeed*dt );
		m_Time += dt;
		
	}
	else
	{
		m_Time = 0.0;
		setAngle( 0.0f );
		setRadius( 0.0f );
	}

	m_lasttime = currtime;


	//设置Shader的参数。
	GLProgramState* programstate = getGLProgramState();

	programstate->setUniformFloat("radius",m_radius);
	programstate->setUniformFloat("angle",m_angle);
	programstate->setUniformTexture("u_texture", m_pTarget->getSprite()->getTexture());

	Color4F color(getDisplayedColor());
	color.a = getDisplayedOpacity() / 255.0f;
	//将要渲染的信息送入渲染队列。
	GLuint textureID = m_pTarget->getSprite()->getTexture()->getName();
	CC_SAFE_DELETE(m_pMeshCommand);
    m_pMeshCommand = new MeshCommand();
    m_pMeshCommand->genMaterialID(0, m_ProgramState, m_RenderMesh, m_BlendFunc);
	m_pMeshCommand->init(_globalZOrder, textureID, programstate, m_BlendFunc, m_RenderMesh->getVertexBuffer(),m_RenderMesh->getIndexBuffer(),(GLenum)m_RenderMesh->getPrimitiveType(), (GLenum)m_RenderMesh->getIndexFormat(), m_RenderMesh->getIndexCount(), transform);

	m_pMeshCommand->setCullFaceEnabled(false);
	m_pMeshCommand->setDepthTestEnabled(false);
	m_pMeshCommand->setDisplayColor(Vec4(color.r, color.g, color.b, color.a));
	Director::getInstance()->getRenderer()->addCommand(m_pMeshCommand);

	Node::draw(renderer,transform,flags);
}

void VortexLayer::menuCloseCallback(Ref* sender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
