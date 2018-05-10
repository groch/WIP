#ifndef CLIGHT_H
#define CLIGHT_H

#include <IModel.h>

class CLight : public IModel
{
    public:
        CLight();
        virtual ~CLight() throw();

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
        unsigned int            _texId[2];
};

#endif // CLIGHT_H
