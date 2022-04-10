#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif


const int COUNT_LIGHTS = 5; // максимально допустимое количество источников света

struct MaterialProperty
{
    vec3  DiffuseColor;
    vec3  AmbienceColor;
    vec3  SpecularColor;
    float Shines;
};


struct LightProperty
{
    vec3 AmbienceColor;
    vec3 DiffuseColor;
    vec3 SpecularColor;
    vec4 ReflectionColor;
    vec4 Position;
    vec4 Direction;
    float Cutoff;
    float Power;
    int Type; // Direct - 0, Point - 1, Spot - 2
};

uniform MaterialProperty u_MaterialProperty;

uniform bool u_IsUseDiffuseMap;
uniform bool u_IsUseNormalMap;


uniform highp float u_MainLightPower;// сила освещения

uniform LightProperty u_LightProperty[COUNT_LIGHTS];
uniform int u_CountLights;          // реальное количество источников освещения (<= COUNT_LIGHTS)

uniform highp vec3 u_eyePosition; // World position of the camera


uniform sampler2D u_DiffuseMapTexture;
uniform sampler2D u_NormalMapTexture;


varying highp vec4 v_position;
varying highp vec2 v_textcoord;
varying highp vec4 v_color;
varying highp vec3 v_normal;
varying highp mat3 v_tbnMatrix;



//! [0]
void main()
{

    int countLights = u_CountLights;
    if(countLights > COUNT_LIGHTS) countLights = COUNT_LIGHTS;


    vec3 eyeVec = normalize(v_position.xyz - u_eyePosition.xyz); // направление взгляда
    vec3 usingNormal = v_normal; // используемая нормаль
    vec4 diffMatColor = texture2D(u_DiffuseMapTexture, v_textcoord); // диффузный цвет


    vec4 SumaColorDiffuse = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    vec4 SumaColorSpecular = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    vec4 SumaColorAmbient = vec4(0.0f, 0.0f, 0.0f, 0.0f);


    if(u_IsUseNormalMap) usingNormal = normalize(texture2D(u_NormalMapTexture, v_textcoord).rgb * 2.0f - 1.0f);
    if(u_IsUseNormalMap) eyeVec = normalize(v_tbnMatrix * eyeVec);



    for(int i = 0; i < countLights; i++)
    {
        vec3 lightVec = vec3(0.0f, 0.0f, 0.0f); // вектор освещения
        vec4 resultLightColor = vec4(0.0f, 0.0f, 0.0f, 0.0f); // результирующий цвет освещения

        if(u_LightProperty[i].Type == 0) // Directional
        {
            lightVec = normalize(u_LightProperty[i].Direction.xyz);
        }
        else // Point, Spot
        {
            lightVec = normalize(v_position - u_LightProperty[i].Position).xyz;
            if(u_LightProperty[i].Type == 2) // Spot
            {
                float angle = acos(dot(u_LightProperty[i].Direction.xyz, lightVec));
                if(angle > u_LightProperty[i].Cutoff) lightVec = vec3(0.0f, 0.0f, 0.0f);
            }
        }

        if(u_IsUseNormalMap) lightVec = normalize(v_tbnMatrix * lightVec);

        vec3 reflectLight = normalize(reflect(lightVec, usingNormal)); // отражённый свет
        float len = length(v_position.xyz - u_eyePosition.xyz); // расстояние от наблюдателя до точки
        float specularFactor = u_MaterialProperty.Shines; // размер пятна блика
        float ambientFactor = 0.25f; // светимость материала

        if(u_IsUseDiffuseMap == false) diffMatColor = vec4(u_MaterialProperty.DiffuseColor, 1.0f);


        //----------------------------------- Ambient -------------------------------------//


        vec4 ambientColor = ambientFactor * diffMatColor;
        SumaColorAmbient += ambientColor * vec4(u_MaterialProperty.AmbienceColor, 1.0f) *
                                           vec4(u_LightProperty[i].AmbienceColor, 1.0f);

        //--------------------------------- Reflection -------------------------------------//

        vec4 specularColor =  u_LightProperty[i].ReflectionColor *
                              u_LightProperty[i].Power *
                              pow(max(0.0f, dot(reflectLight, -eyeVec)), specularFactor);

        SumaColorSpecular += specularColor *
                             vec4(u_MaterialProperty.SpecularColor, 1.0f) *
                             vec4(u_LightProperty[i].SpecularColor, 1.0f);

        //----------------------------------- Diffuse ---------------------------------------//

        vec4 diffColor = diffMatColor * u_LightProperty[i].Power *
                         max(0.0f, dot(usingNormal, -lightVec));

         SumaColorDiffuse += diffColor *
                             vec4(u_LightProperty[i].DiffuseColor, 1.0f);

    }


    // Set fragment color from texture
    gl_FragColor = SumaColorAmbient + SumaColorDiffuse + SumaColorSpecular;
}
//! [0]
