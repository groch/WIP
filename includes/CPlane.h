#ifndef CPLANE_H
#define CPLANE_H

#include <IModel.h>

class CPlane : public IModel
{
    public:
        CPlane(unsigned int = 0);
        virtual ~CPlane();

        virtual void            Draw(Shader&);

        unsigned int            GetVao() const { return _vao; }
        unsigned int            GetVbo() const { return _vbo; }
        unsigned int            GetEbo() const { return _ebo; }
        unsigned int*           GetTexId() { return _texId; }


    protected:

    private:
        unsigned int            _vao;
        unsigned int            _vbo;
        unsigned int            _ebo;
        unsigned int            _texId[3];
};

#endif // CPLANE_H
