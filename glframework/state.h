#pragma once

#include "core.h"
#include "../wrapper/checkError.h"

#include <vector>

class State
{
public:
	static State* createState();
	static void destroyAllInstances();

	State() = default;
	State(const State&) = default;
	State& operator=(const State&) = default;
	~State();

public:
    // Depth settings
    void setDepthTest(bool depthTest, GLenum depthFunc = GL_LESS);
    void setDepthWrite(bool depthWrite);
    void applyDepthState() const;

    // Polygon offset settings
    void setPolygonOffset(bool enabled, GLenum type = GL_POLYGON_OFFSET_FILL,
		                  float factor = 0.0f, float units = 0.0f);
	void applyPolygonOffsetState() const;

    // Stencil settings
	void setStencilTest(bool enabled, GLenum func, GLint ref, GLuint stencilFuncMask);
	void setStencilOps(GLenum sfail, GLenum zfail, GLenum zpass);
	void setStencilMask(GLuint stencilMask);
	void applyStencilState() const;

	const std::vector<State*>& getInstances() const { return bookmark; }

protected:
	//深度检测相关
	bool	mDepthTest{ true };
	bool	mDepthWrite{ true };
	GLenum	mDepthFunc{ GL_LESS };

	//polygonOffset相关
	bool			mPolygonOffset{ false };
	GLenum      	mPolygonOffsetType{ GL_POLYGON_OFFSET_FILL };
	float			mFactor{ 0.0f };
	float			mUnit{ 0.0f };

	//stencil相关
	//glEnable(GL_STENCIL_TEST);
	//glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	//glStencilMask(0xFF);//保证了模板缓冲可以被清理
	//glStencilFunc(GL_ALWAYS, 1, 0xFF);//如何测试
	bool			mStencilTest{ false };

	GLenum	mSFail{ GL_KEEP };	//模板测试失败了怎么办
	GLenum	mZFail{ GL_KEEP };	//模板测试通过但是深度检测没通过怎么办
	GLenum	mZPass{ GL_KEEP };	//模板+深度测试都通过了怎么办

	GLuint	mStencilMask{ 0xFF };//用于控制模板写入

	GLenum    mStencilFunc{ GL_ALWAYS };
	GLint  	  mStencilRef{ 0 };
	GLuint    mStencilFuncMask{ 0xFF };

protected:
	static std::vector<State*> bookmark;
};