#ifndef COBJECT_H
#define COBJECT_H

#include <glad\glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "CPerspectiveCamera.h"
#include "IObject.h"
#include "IModel.h"

class CGame;

class CObject : public IObject
{
    public:
        CObject(CGame&, IModel&, glm::vec3, glm::vec3 = glm::vec3(1.0f), bool = false, bool = false);
        virtual ~CObject();

        virtual int             Draw(Shader&, glm::vec3);

        virtual glm::vec3       GetPos() { return _pos; }
        virtual void            SetPos(glm::vec3 pos) { _pos = pos; }
        void                    ApplyRotation(float angle, glm::vec3 axe) { _axe = axe; _angle = angle; }

    protected:

    private:
        IModel&         _model;
        bool            _turning;

        glm::vec3       _pos;
        glm::vec3       _scale;

        float           _angle;
        glm::vec3       _axe;

        bool            _isLight;

        CGame&          _game;
};

#endif // COBJECT_H
