#pragma once
namespace yougine
{
	struct Vector3
	{
	public:
		float x, y, z;

		Vector3(float x, float y, float z)
			: x(x),
			y(y),
			z(z)
		{
		}

		/**
		 * \brief ������Z�q
		 * \param v �������l��
		 * \return ������ꂽ�Q��
		 */
		Vector3& operator=(const Vector3& v)
		{
			this->x = v.x;
			this->y = v.y;
			this->z = v.z;
		}
	};

}