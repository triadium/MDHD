#include "stdafx.h"
#include "Game/Material.h"
#include "Render/Material.h"

namespace Game {

	using namespace Render;

	Material::Material(): _blend(BlendMode::Alpha)
	{		
	}

	Material::Material(Texture * tex0, ShaderProgram * shader, BlendMode blend)
		: _shader(shader), _blend(blend)
	{
		_textures[0] = tex0;
	}

	Material::Material(Texture * tex0, Texture * tex1, ShaderProgram * shader, BlendMode blend)
		: _shader(shader), _blend(blend)
	{
		_textures[0] = tex0;
		_textures[1] = tex1;
	}
	
	Material::Material(Texture * tex0, Texture * tex1, Texture * tex2, ShaderProgram * shader, BlendMode blend)
		: _shader(shader), _blend(blend)
	{
		_textures[0] = tex0;
		_textures[1] = tex1;
		_textures[2] = tex2;
	}

	Material::Material(Texture * tex0, Texture * tex1, Texture * tex2, Texture * tex3, ShaderProgram * shader, BlendMode blend)
		: _shader(shader), _blend(blend)
	{
		_textures[0] = tex0;
		_textures[1] = tex1;
		_textures[2] = tex2;
		_textures[3] = tex3;
	}

	size_t Material::GetTextureCount() const
	{
		size_t result = 0;

		for (; result < kMaxStages; ++result) {
			if (!_textures[result]) { break; }
		}

		return result;
	}

	Texture * Material::GetTexture(size_t index) const
	{
		Assert(i < kMaxStages);
		return _textures[index].get();
	}

	ShaderProgram * Material::GetShaderProgram() const
	{
		return _shader.get();
	}

	BlendMode Material::GetBlendMode() const
	{
		return _blend;
	}

	void Material::Bind()
	{
		_prevBlend = device.GetBlendMode();
		device.SetBlendMode(_blend);
				
		// Сколько текстур поддерживает шейдер, столько и сможем привязать,
		// а иначе привяжем только первую текстуру для шейдера по умолчанию
		if (_shader) {
			_shader->Bind();
			for (size_t stage = 0; stage < kMaxStages; ++stage) {
				if (!_textures[stage]) { break; }

				_textures[stage]->Bind(stage);
			}
		}
		else if (_textures[0]) {
			_textures[0]->Bind();
		}
		// else { skip }
	}

	void Material::Unbind()
	{
		device.SetBlendMode(_prevBlend);
		if (_shader) {
			_shader->Unbind();
		}
		// else { skip }
	}
	
	int Material::Width() const
	{
		return _textures[0] ? _textures[0]->Width() : 0;
	}

	int Material::Height() const
	{
		return _textures[0] ? _textures[0]->Height() : 0;
	}
}