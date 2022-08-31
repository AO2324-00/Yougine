#include "TransformComponent.h"

TransformComponent::TransformComponent(float x, float y, float z)
{
	this->position = new Vector3(x, y, z);
}

/**
 * \brief position�̎��̂�Ԃ��i�Q�Ƃł͂Ȃ��̂ŕύX���Ă��Atransform.position���̂��̂�ύX�ł����ł͂Ȃ��j
 * \return position�̎���
 */
Vector3 TransformComponent::GetPosition()
{
	return *this->position;
}

/**
 * \brief position�̃Z�b�^�[
 * \param position �Z�b�g����l
 */
void TransformComponent::SetPosition(Vector3 position)
{
	this->position = &position;
}

