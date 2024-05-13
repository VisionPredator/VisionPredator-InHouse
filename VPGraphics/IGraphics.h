#pragma once
#include <windows.h>


namespace Graphics
{
	class Interface
	{
	public:
		Interface();
		virtual ~Interface();

		virtual void Initialize() abstract;
		virtual void Update(double dt) abstract;
		virtual void Finalize() abstract;

		virtual void Render() abstract;

		virtual void OnResize() abstract;

	private:

	};
}