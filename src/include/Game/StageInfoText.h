// Copyright (C) 2019 Aleksey Khachaturov
// https://github.com/triadium/MDHD
// License: GNU GPL 3 or later, see file LICENSE

#pragma once

namespace Game {

	class StageInfoText : public Object
	{
	public:
		typedef boost::intrusive_ptr<StageInfoText> Ptr;
		typedef std::vector<Ptr> Container;

		// ������ ������ ��������������� ���������
		struct Model {
			// �����, ������� ���� ��������
			std::string text;
			// ��������� ����� "������"
			FPoint startPoint;
			// �������� ����� "������"
			FPoint endPoint;
			// ����� "������"
			float time;
			// ����� ������
			float showTime;
		};

		StageInfoText(const std::string& name, const std::string& fontName);

		void Draw() override;
		void Update(float dt) override;

		const Model & GetModel() const;
		void SetModel(const Model & value);

	private:
		void InternalUpdateData();

	private:		
		std::string _fontName;
		Model _model;
		FPoint _direction;
		FPoint _speed;
		
		bool _isRunning;
	};

}
