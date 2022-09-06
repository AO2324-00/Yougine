#include "Scene.h"

namespace yougine
{
    void Scene::CreateGameObject(std::string name, GameObject* parent)
    {
        GameObject* gameobject = new GameObject(name, parent);

        if (parent == nullptr)
        {
            gameobject_list.push_back(gameobject);
        }
        else
        {
            parent->AddChild(gameobject);
        }
    }

    //処理変える
    void Scene::RemoveGameObject(GameObject* gameobject)
    {
        std::list<GameObject*> new_list;

        for (GameObject* obj : gameobject_list)
        {
            if (gameobject == obj)
            {
                new_list.push_back(obj);
            }
        }

        gameobject_list = new_list;
    }

    std::list<GameObject*> Scene::GetGameObjects()
    {
        return gameobject_list;
    }

    GameObject* Scene::GetGameObjectByName(std::string name)
    {
        if (name.empty()) return nullptr;

        GameObject* r_obj = nullptr;
        for (GameObject* game_object : gameobject_list)
        {
            r_obj = RecursiveGameObjects({ game_object }, name);
        }

        return r_obj;
    }

    GameObject* Scene::RecursiveGameObjects(std::vector<GameObject*> game_objects, std::string name)
    {
        for (GameObject* obj : game_objects)
        {
            if (obj->GetName() == name)
            {
                return obj;
            }

            RecursiveGameObjects(obj->GetChildObjects(), name);
        }

        return nullptr;
    }

}