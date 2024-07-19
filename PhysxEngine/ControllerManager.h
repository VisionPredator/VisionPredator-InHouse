#pragma once
class CharacterController;
class ControllerManager
{
public: 
	ControllerManager();
	~ControllerManager();
	/// <summary>
/// ĳ���� ��Ʈ�ѷ� ���� �Լ�
/// </summary>
/// <param name="controllerInfo"> ĳ���� ��Ʈ�ѷ� ���� ������ </param>
/// <param name="movementInfo"> ĳ���� �����Ʈ ���� ������ </param>
/// <returns></returns>
	bool CreatController();

	/// <summary>
/// ĳ���� ��Ʈ�ѷ� ���� �Լ�
/// </summary>
/// <param name="id">  </param>
/// <returns></returns>
	bool RemoveController(const unsigned int& id);
	std::unordered_map<uint32_t, CharacterController*> m_CharectorMap;
};

