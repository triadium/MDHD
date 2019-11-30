// Copyright (C) 2019 Aleksey Khachaturov
// https://github.com/triadium/MDHD
// License: GNU GPL 3 or later, see file LICENSE

#pragma once

namespace Game {
	
	// �������� ����� ���������� � ���� �� 4 ������� � ������.
	// ��� ��������� �������� ����� ����������.
	// ���� � ��� �� �������� ����� ���� �������� ��������� 
	// �������� �����.
	// ������ ���������� �� ������������ ������� ����������� ��
	// ��������� ������ ������������� � ���������� ����������. 
	// ����� ����������� �������� � ������� � ������ �������
	// ������������� ��������.

	class Material: public RefCounter
	{
	public:
		typedef boost::intrusive_ptr<Material> Ptr;

		Material();
		Material(Render::Texture* tex0, Render::ShaderProgram* shader = nullptr, BlendMode blend = BlendMode::Alpha);
		Material(Render::Texture* tex0, Render::Texture* tex1, Render::ShaderProgram* shader = nullptr, BlendMode blend = BlendMode::Alpha);
		Material(Render::Texture* tex0, Render::Texture* tex1, Render::Texture* tex2,
			Render::ShaderProgram* shader = nullptr, BlendMode blend = BlendMode::Alpha);
		Material(Render::Texture* tex0, Render::Texture* tex1, Render::Texture* tex2, Render::Texture* tex3,
			Render::ShaderProgram* shader = nullptr, BlendMode blend = BlendMode::Alpha);

		size_t GetTextureCount() const;
		Render::Texture * GetTexture(size_t index) const;
		Render::ShaderProgram * GetShaderProgram() const;
		BlendMode GetBlendMode() const;
		
		void Bind();
		void Unbind();

		int Width() const;
		int Height() const;

	private:
		enum { kMaxStages = 4 };

		std::array<Render::TexturePtr, kMaxStages> _textures;
		Render::ShaderProgramPtr _shader;
		BlendMode _blend;
		BlendMode _prevBlend;
	};
}
