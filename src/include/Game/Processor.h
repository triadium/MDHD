// Copyright (C) 2019 Aleksey Khachaturov
// https://github.com/triadium/MDHD
// License: GNU GPL 3 or later, see file LICENSE

#pragma once

namespace Game {

	class Scene;
	
	// ��������� ����� ��������� �� ��������� ����� ���������
	// ���� ���������� �������� �����. ��������, ����� �����
	// ���������� ���������� � ����� dynamic_cast ��� ������������
	// �����, � ��� �� ��� ����������/�������� ��������� ������
	// ��������, ������� ����� ����������� � ��������� ������ ������
	// ����������� (��������, ����� ����� �� ����� ������������ 
	// �������� ����������� ������������) �� �������� ������������ RTTI. 
	// ��� ����� ����� �������� ������� ���������� ��� �������,
	// ������� ����� �������� ����� ��� ���������� �������
	class Processor
	{
	public:
		typedef std::unique_ptr<Processor> Unique;
		typedef std::vector<Unique> Container;

		Processor(Scene * scene);

		virtual void Update(float dt) = 0;
		// ���������� �������� ��� ������ �� �������, ������� ������ ���� ������� �� �����
		virtual void RemovePendingObjects(const Object::Container & objectsToBeRemoved) = 0;
		virtual void Clear() = 0;

	protected:
		Scene * _scene;
	};
}
