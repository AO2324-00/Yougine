#include "GameObject.h"

std::vector<components::Component> componentList;

namespace yougine
{
	GameObject::GameObject()
	{
		InitializeComponentList();
	}

	void GameObject::InitializeComponentList()
	{
		for(Components::Component component : Components::Component)
		{
			componentList.push_back(component);
		}

		//parentObject������
	}

	std::vector<components::Component> GameObject::GetComponents()
	{
		return component_list;
	}
}