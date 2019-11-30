#include "stdafx.h"
#include "Game/Constants.h"
#include "Game/Scene.h"
#include "Game/SceneLayerMessageFunction.h"


namespace Game {

	// FIXME: �������������� ������ ��������� ����� ����� ��������� ���������� ���� � ��������� ��� ��������
	// ���������� ����� ����� ���������� � ����������, ������� ����� �������������� ���������� �����,
	// � ��� ��������� ����� ������ �������� ��������� - ��� �������, ��� � ������� �������.
	// ���������� ������ ������� ������� � ���, ��� ��� ������ ����������������� ������ ������ �������, 
	// ������� ������ ����� ��� �������� �� ����� ������ ������� ��������, ���� ����� �� ������������ 
	// ����������������� ��������� ���� ������� ��������, ���� ���������� - "�� ��� -  �� �����������". 
	// ��� ������� ���������� ������� �������� ����� ������� ���������� ������ ��� ��������, 
	// ������� ����������� �� ��������� ���������, �� ��� ����� ����� �������� ������ �������� - 
	// ���������� �� �����, �������� �� �����. ����� ����� �������� �� � ��������������� �������.

	void SceneLayerMessageFunction::AcceptMessage(const Message & message)
	{
		const std::string& publisher = message.getPublisher();
		const std::string& data = message.getData();
		auto layer = message.GetTargetLayer() == "" ? Core::mainScreen.GetTopLayer() : Core::guiManager.getLayer(message.GetTargetLayer());		
		layer->BroadcastMessage(message);
	}

}
