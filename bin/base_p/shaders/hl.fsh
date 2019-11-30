
uniform sampler2D sampler;
uniform sampler2D sampler1;

varying highp vec4 v_color;
varying highp vec2 v_texcoord;

const highp vec4 add = vec4(0.9, 0.3, 0.1, 0.0);

void main()
{
	gl_FragColor =  add * texture2D(sampler1, v_texcoord) + v_color * texture2D(sampler, v_texcoord);
}
