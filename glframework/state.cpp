#include "state.h"

std::vector<State*> State::bookmark;

State* State::createState()
{
    State* state = new State;
    bookmark.push_back(state);
    return state;
}

void State::destroyAllInstances()
{
    for (State* instance : bookmark)
    {
        if (instance) delete instance;
    }
    bookmark.clear();
}

State::~State()
{
    printf("---- ~State ----\n");

    for (auto& state : bookmark)
    {
        if (state == this)
        {
            state = nullptr;
            break;
        }
    }
}


// Depth settings
void State::setDepthTest(bool depthTest, GLenum depthFunc)
{
    mDepthTest = depthTest;
    mDepthFunc = depthFunc;
}

void State::setDepthWrite(bool depthWrite)
{
    mDepthWrite = depthWrite;
}

void State::applyDepthState() const
{
    if (mDepthTest)
    {
        GL_CALL(glEnable(GL_DEPTH_TEST));
        GL_CALL(glDepthFunc(mDepthFunc));
    }
    else
    {
        GL_CALL(glDisable(GL_DEPTH_TEST));
    }

    if (mDepthWrite)
    {
        GL_CALL(glDepthMask(GL_TRUE));
    }
    else
    {
        GL_CALL(glDepthMask(GL_FALSE));
    }
}

void State::setPolygonOffset(bool enabled, GLenum type, float factor, float units)
{
    mPolygonOffset = enabled;
    mPolygonOffsetType = type;
    mFactor = factor;
    mUnit = units;
}

void State::applyPolygonOffsetState() const
{
    if (mPolygonOffset)
    {
        GL_CALL(glEnable(mPolygonOffsetType));
        GL_CALL(glPolygonOffset(mFactor, mUnit));
    }
    else
    {
        GL_CALL(glDisable(GL_POLYGON_OFFSET_FILL));
        GL_CALL(glDisable(GL_POLYGON_OFFSET_LINE));
    }
}

void State::setStencilTest(bool enabled, GLenum func, GLint ref,
                           GLuint stencilFuncMask)
{
    mStencilTest = enabled;
    mStencilFunc = func;
    mStencilRef = ref;
    mStencilFuncMask = stencilFuncMask;
}

void State::setStencilOps(GLenum sfail, GLenum zfail, GLenum zpass)
{
    mSFail = sfail;
    mZFail = zfail;
    mZPass = zpass;
}

void State::setStencilMask(GLuint stencilMask)
{
    mStencilMask = stencilMask;
}

void State::applyStencilState() const
{
    if (mStencilTest)
    {
        GL_CALL(glEnable(GL_STENCIL_TEST));

        GL_CALL(glStencilOp(mSFail, mZFail, mZPass));
        GL_CALL(glStencilMask(mStencilMask));
        GL_CALL(glStencilFunc(mStencilFunc, mStencilRef, mStencilFuncMask));
    }
    else
    {
        GL_CALL(glDisable(GL_STENCIL_TEST));
    }
}