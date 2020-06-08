#version 330 core

out vec4 FragColor;

in vec3 vertexFrag;
in vec3 normalFrag;

in vec3 matAmbFrag;
in vec3 matDiffFrag;
in vec3 matSpecFrag;

in float matShinFrag;

// Valors per als components que necessitem dels focus de llum
uniform vec3 posFocus;  					// en SCA
uniform vec3 colFocus;
vec3 llumAmbient = vec3(0.2, 0.2, 0.2);

vec3 Lambert (vec3 NormSCO, vec3 L) 
{
  // S'assumeix que els vectors que es reben com a paràmetres estan normalitzats

  // Inicialitzem color a component ambient
  vec3 colRes = llumAmbient * matAmbFrag;

  // Afegim component difusa, si n'hi ha
  if (dot (L, NormSCO) > 0)
    colRes = colRes + colFocus * matDiffFrag * dot (L, NormSCO);
  return (colRes);
}

vec3 Phong (vec3 NormSCO, vec3 L, vec4 vertSCO) 
{
  // Els vectors estan normalitzats

  // Inicialitzem color a Lambert
  vec3 colRes = Lambert (NormSCO, L);

  // Calculem R i V
  if ((dot(NormSCO,L) < 0) || (matShinFrag == 0))
    return colRes;  // no hi ha component especular

  vec3 R = reflect(-L, NormSCO); // equival a: 2.0*dot(NormSCO,L)*NormSCO - L;
  vec3 V = normalize(-vertSCO.xyz); // perquè la càmera està a (0,0,0) en SCO

  if (dot(R, V) < 0)
    return colRes;  // no hi ha component especular
  
  float shine = pow(max(0.0, dot(R, V)), matShinFrag);
  return (colRes + matSpecFrag * colFocus * shine); 
}

void main()
{	
	// Deixem la pos del focus en SCA, afegim un 1 per deixar-ho a coords homogenies
  vec4 focusSCA = vec4(posFocus, 1.0);

  // Direccio de la llum
  vec3 L = focusSCA.xyz - vertexFrag.xyz;
  vec3 NORM = normalFrag;

	// Unicament normalitzem L en el frag, res mes
  L = normalize(L);

	vec3 fcolor = Phong(NORM, L, vec4(vertexFrag, 1.0));

	FragColor = vec4(fcolor, 1.0);	
}
