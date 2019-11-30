// Copyright (C) 2019 Aleksey Khachaturov
// https://github.com/triadium/MDHD
// License: GNU GPL 3 or later, see file LICENSE

#pragma once

namespace Game {

	// ����� �� ������ ������ �������� ����������� ��� ������� ��������
	// � ������������� ��������� ������ ���� ����� �� ����, ��� ��� 
	// ������� ������� � ������ ������ �� ���������������.
	// � ����� ����� �������� ������ ���� ������� �������� � ����������
	// ���������� ������ ������� �������� - ��� ����� ���� ���������� ��
	// ������������ ���������� �������� ������ ������, ������������� ��� 
	// ����������� � ����������� �������������� (���-����� � ������� �������)
	// ��� �� ����� ����� ���������� ������ ��������� ������� �������� 
	// � �� ������� �� ���������������� ����, � ����� ������������ ��� ��������.
	// ��� ����� ����������� ����������, ������� �������� �� ��������� ���������
	// ����� - �������� �������� � ������� ���������, ����������� ������������,
	// ���� �� ����� ���� �� ������� ���� ��� ������� � ������������� ����������� � �.�.

	class Scene : public GUI::Widget
	{
	public:
		
		Scene(const std::string& name, rapidxml::xml_node<>* elem);

		void Update(float dt) override;
		void Draw() override;

		bool MouseDown(const IPoint& mouse_pos) override;
		//void MouseMove(const IPoint& mouse_pos) override;
		void MouseUp(const IPoint& mouse_pos) override;

		void AddObject(Object * object);
		void AddObject(const Object::Ptr& object) { AddObject(object.get()); }
		// void AddObject(Object * object, size_t index);
		
		void RemoveObject(Object * object);
		void RemoveObject(const Object::Ptr& object) { RemoveObject(object.get()); }

		const Object::Container& GetObjects() const;

		void Clear();

		void AddObjectLater(Object * object);
		void AddObjectLater(const Object::Ptr& object);

		void RemoveObjectLater(Object * object);
		void RemoveObjectLater(const Object::Ptr& object);

		ParticleEffectPtr AddEffect(const std::string& name);
		ParticleEffectPtr AddEffect(const std::string& name, const FPoint& position);
		void UpdateEffects(float dt);

	protected:
		void AddProcessor(Processor::Unique&& processor);
		void AddPendingObjects();
		void RemovePendingObjects();
		void RemoveObject(const Object::Container::iterator& it);

	protected:
		Object::Container _objects;
		Processor::Container _processors;
		Object::Container _objectsToBeAdded;
		Object::Container _objectsToBeRemoved;
		EffectsContainer _effects;
	};

}
