// shadow.vert

varying vec4 shadow;

void main(void)
{
  vec3 position = vec3(gl_ModelViewMatrix * gl_Vertex);
  vec3 normal = normalize(gl_NormalMatrix * gl_Normal);
  vec3 light = normalize(gl_LightSource[0].position.xyz - position);
  float diffuse = dot(light, normal);

  // �������x�����߂Ă���  
  gl_FrontColor = gl_LightSource[0].ambient * gl_Color;
  shadow = gl_FrontColor;
  if (diffuse > 0.0) {
    vec3 view = normalize(position);
    vec3 halfway = normalize(light - view);
    float specular = pow(max(dot(normal, halfway), 0.0), gl_FrontMaterial.shininess);
    // �g�U���ˌ����x�����߂� 
	vec4 temp = gl_LightSource[0].diffuse * gl_Color * diffuse;
    // �e�̕����̊g�U���ˌ����x�͓�����0.2�{�ɂ���
	shadow += temp * 0.2;
	// �����̕����̉A�e�����߂�
    gl_FrontColor += temp + gl_FrontLightProduct[0].specular * specular;
  }

  // ���_�̃��[���h���W�l�Ƀe�N�X�`���ϊ��s����|����
  gl_TexCoord[0] = gl_TextureMatrix[0] * gl_ModelViewMatrix * gl_Vertex;
  gl_Position = ftransform();
}