#pragma once
class CharacterController;
class ControllerManager
{
public: 
	ControllerManager();
	~ControllerManager();
	/// <summary>
/// 캐릭터 컨트롤러 생성 함수
/// </summary>
/// <param name="controllerInfo"> 캐릭터 컨트롤러 생성 데이터 </param>
/// <param name="movementInfo"> 캐릭터 무브먼트 생성 데이터 </param>
/// <returns></returns>
	bool CreatController();

	/// <summary>
/// 캐릭터 컨트롤러 삭제 함수
/// </summary>
/// <param name="id">  </param>
/// <returns></returns>
	bool RemoveController(const unsigned int& id);
	std::unordered_map<uint32_t, CharacterController*> m_CharectorMap;
};

