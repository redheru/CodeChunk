// shadow.frag

uniform sampler2DShadow texture;
varying vec4 shadow;
 
void main (void)
{
  gl_FragColor = shadow + (gl_Color - shadow) * shadow2DProj(texture, gl_TexCoord[0]);
}
