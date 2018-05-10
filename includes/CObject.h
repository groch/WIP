#ifndef COBJECT_H
#define COBJECT_H

#include <glad\glad.h>

#include "CPerspectiveCamera.h"
#include "IObject.h"
#include "IModel.h"

class CGame;

class CObject : public IObject
{
    public:
        CObject(CGame&, IModel&, glm::vec3, glm::vec3 = glm::vec3(1.0f), bool = false, bool = false);
        virtual ~CObject();

        virtual int             Draw(Shader&);

        virtual glm::vec3       GetPos() { return _pos; }
        virtual void            SetPos(glm::vec3 pos) { _pos = pos; }

    protected:

    private:
        IModel&         _model;
        bool            _turning;

        glm::vec3       _pos;
        glm::vec3       _scale;
        bool            _isLight;

        CGame&          _game;
};

#endif // COBJECT_H
