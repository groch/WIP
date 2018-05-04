#ifndef AMODEL_H
#define AMODEL_H

#include <glad\glad.h>

class AModel
{
    public:
        virtual                 ~AModel() = default;

        virtual unsigned int    GetShaderId() = 0;
        virtual void            Pre_Draw() { glUseProgram(_shaderProgram); }
        virtual void            Draw() = 0;

        unsigned int            makeShader(GLenum, const char **);
        unsigned int            linkShaders(unsigned int *, unsigned int);

    protected:
        unsigned int            _shaderProgram;
};

#endif // IMODEL_H
