#ifndef COBJECT_H
#define COBJECT_H

#include <glad\glad.h>

#include "CPerspectiveCamera.h"
#include "IObject.h"
#include "AModel.h"

class CGame;

class CObject : public IObject
{
    public:
        CObject(CGame&, AModel&, glm::vec3, bool = false, bool = false);
        virtual ~CObject();

        virtual int     Draw();

        virtual glm::vec3       GetPos() { return _pos; }
        virtual void            SetPos(glm::vec3 pos) { _pos = pos; }

    protected:

    private:
        AModel&         _model;
        bool            _turning;

        glm::vec3       _pos;
        bool            _isLight;

        CGame&          _game;
};

#endif // COBJECT_H
