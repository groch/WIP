#ifndef CCUBE_H
#define CCUBE_H

#include <AModel.h>

class CCube : public AModel
{
    public:
        CCube();
        virtual ~CCube();

        virtual unsigned int    GetShaderId() { return _shaderProgram; }
        virtual void            Draw();

        unsigned int            GetVao() const { return _vao; }
        unsigned int            GetVbo() const { return _vbo; }
        unsigned int            GetEbo() const { return _ebo; }
        unsigned int*           GetTexId() { return _texId; }


    protected:

    private:
        unsigned int            _vao;
        unsigned int            _vbo;
        unsigned int            _ebo;
        unsigned int            _texId[2];
};

#endif // CCUBE_H
