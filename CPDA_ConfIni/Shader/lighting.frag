#version 330 core

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light
{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 color;

uniform vec3 viewPos;

uniform Material material;

uniform Light light;
uniform Light light2;

uniform sampler2D texture_diffuse;


void main()
{
    // =====================================================
    // NORMAL
    // =====================================================

    vec3 norm = normalize(Normal);


    // =====================================================
    // PRIMERA LUZ
    // =====================================================

    vec3 lightDir1 =
        normalize(light.position - FragPos);


    // ---------------- Ambient ----------------

    vec3 ambient1 =
        light.ambient * material.diffuse;


    // ---------------- Diffuse ----------------

    float diff1 =
        max(dot(norm, lightDir1), 0.0);

    vec3 diffuse1 =
        light.diffuse *
        diff1 *
        material.diffuse;


    // ---------------- Specular ----------------

    vec3 viewDir =
        normalize(viewPos - FragPos);

    vec3 reflectDir1 =
        reflect(-lightDir1, norm);

    float spec1 =
        pow(
            max(dot(viewDir, reflectDir1), 0.0),
            material.shininess
        );

    vec3 specular1 =
        light.specular *
        (spec1 * material.specular);


    // =====================================================
    // SEGUNDA LUZ
    // =====================================================

    vec3 lightDir2 =
        normalize(light2.position - FragPos);


    // ---------------- Ambient ----------------

    vec3 ambient2 =
        light2.ambient * material.diffuse;


    // ---------------- Diffuse ----------------

    float diff2 =
        max(dot(norm, lightDir2), 0.0);

    vec3 diffuse2 =
        light2.diffuse *
        diff2 *
        material.diffuse;


    // ---------------- Specular ----------------

    vec3 reflectDir2 =
        reflect(-lightDir2, norm);

    float spec2 =
        pow(
            max(dot(viewDir, reflectDir2), 0.0),
            material.shininess
        );

    vec3 specular2 =
        light2.specular *
        (spec2 * material.specular);


    // =====================================================
    // SUMAR LAS DOS LUCES
    // =====================================================

    vec3 ambient =
        ambient1 + ambient2;

    vec3 diffuse =
        diffuse1 + diffuse2;

    vec3 specular =
        specular1 + specular2;


    // =====================================================
    // RESULTADO FINAL DE ILUMINACIÓN
    // =====================================================

    vec3 result =
        ambient +
        diffuse +
        specular;


    // =====================================================
    // APLICAR TEXTURA
    // =====================================================

    color =
        vec4(result, 1.0f) *
        texture(texture_diffuse, TexCoords);
}