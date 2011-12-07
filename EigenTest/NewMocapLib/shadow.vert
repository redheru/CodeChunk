// shadow.vert

varying vec4 shadow;

void main(void)
{
  vec3 position = vec3(gl_ModelViewMatrix * gl_Vertex);
  vec3 normal = normalize(gl_NormalMatrix * gl_Normal);
  vec3 light = normalize(gl_LightSource[0].position.xyz - position);
  float diffuse = dot(light, normal);

  // 環境光強度を求めておく  
  gl_FrontColor = gl_LightSource[0].ambient * gl_Color;
  shadow = gl_FrontColor;
  if (diffuse > 0.0) {
    vec3 view = normalize(position);
    vec3 halfway = normalize(light - view);
    float specular = pow(max(dot(normal, halfway), 0.0), gl_FrontMaterial.shininess);
    // 拡散反射光強度を求める 
	vec4 temp = gl_LightSource[0].diffuse * gl_Color * diffuse;
    // 影の部分の拡散反射光強度は日向の0.2倍にする
	shadow += temp * 0.2;
	// 日向の部分の陰影を求める
    gl_FrontColor += temp + gl_FrontLightProduct[0].specular * specular;
  }

  // 頂点のワールド座標値にテクスチャ変換行列を掛ける
  gl_TexCoord[0] = gl_TextureMatrix[0] * gl_ModelViewMatrix * gl_Vertex;
  gl_Position = ftransform();
}