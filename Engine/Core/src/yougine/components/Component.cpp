#include "Component.h"
#include <iostream>

namespace yougine::components
{
    Component::Component(managers::ComponentName componentname)
    {
        this->parent_gameobject = nullptr;
        this->component_name = componentname;
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
    bool Component::RegisterThisComponentToComponentList(Scene* scene)
    {
        if (this->component_name == managers::ComponentName::kNone)
        {
            //componentname��kNone�Ȃ̂�ComponentList�ɒǉ����Ȃ�
            return false;
        }
        scene->GetComponentList()->AddObjectToDictionary(this->component_name, this);
        this->register_component_list = scene->GetComponentList();
        return true;
    }

    void Component::UnregisterThisComponentFromComponentList()
    {
        this->register_component_list->RemoveComponentFromDictionary(this->component_name, this);
        this->register_component_list = nullptr;
    }

    //�o�^����Ă��邩�ǂ���
    bool Component::isAlradyRegisterComponentList()
    {
        return !(register_component_list == nullptr);
    }
}
