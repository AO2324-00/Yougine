#include "Component.h"
#include <iostream>

namespace yougine::components
{
    Component::Component()
    {
        this->parent_gameobject = nullptr;
    }

    Component::~Component()
    {

    }

    //private
    void Component::InitializeProperties()
    {

    }

    //public
    void Component::Excute()
    {
        std::cout << "excute" << std::endl;
    }

    void Component::InitializeOnPlayBack()
    {
        std::cout << "initialize component" << std::endl;
    }

    bool Component::operator==(const Component& rhs) const
    {
        return *this == rhs;
    }

    GameObject* Component::GetGameObject()
    {
        return parent_gameobject;
    }
    void Component::SetParentGameObject(GameObject* parent_gameobject)
    {
        this->parent_gameobject = parent_gameobject;
    }

    /**
     * \brief parentgameobject��null�̏ꍇ�G���[���o���iSetParentGameObject�֐����ɌĂ΂Ȃ��ƃG���[�ɂȂ�j
     * \param scene parentGameobject����������V�[��
     */
    void Component::SetThisComponentToComponentList(Scene* scene)
    {
        if (this->parent_gameobject == nullptr)
        {
            throw "exception,this compoent does not have parentGameobject";
            return;
        }
        scene->GetComponentList()->AddObjectToDictionary(managers::ComponentName::kCustom, this->parent_gameobject);
    }
}