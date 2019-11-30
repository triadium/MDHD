// Copyright (C) 2019 Aleksey Khachaturov
// https://github.com/triadium/MDHD
// License: GNU GPL 3 or later, see file LICENSE

#pragma once

namespace Game {

	// ������, ������� �������� ����� � ��������� ���� �����������-��������
	// ����� �������������� ���������. ������ ����� � ���� ��� ���, 
	// ������ �� ������-�������� � ������ � ���� ������������� ���������

	class Signal : public RefCounter
	{
	public:
		typedef boost::intrusive_ptr<Signal> Ptr;
		typedef std::vector<Ptr> Container;
			
		Signal(const std::string& name, Object * sourceObject);

		const std::string& GetName() const;
		const Object::Ptr& GetSourceObject() const;
		VariableSet& GetData() const;

	public: 
		
	private:
		mutable VariableSet _data;
		std::string _name;
		Object::Ptr _sourceObject;
	};

}
