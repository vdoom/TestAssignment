#ifndef SHADERS_H
#define SHADERS_H
static const char *vertexModelShaderSource =
    "attribute lowp vec4 _POSITION;\n"
    "uniform lowp mat4 _TRANSFORMMATRIX;\n"
    "uniform lowp mat4 _VIEWPROJECTIONMATRIX;\n"
    "void main() {\n"
    "   gl_Position = (_VIEWPROJECTIONMATRIX * _TRANSFORMMATRIX) * _POSITION;\n"
    "}\n";

//static const char *vertexShaderSource =
//    "attribute lowp vec4 _POSITION;\n"
//    "uniform lowp mat4 matrix;\n"
//    "void main() {\n"
//    "   col = colAttr;\n"
//    "   gl_Position = matrix * _POSITION;\n"
//    "}\n";

static const char *fragmentShaderSource =
    "uniform lowp vec3 _COLOR;\n"
    "uniform lowp vec4 _MATERIAL;\n"
    "void main() {\n"
        "   gl_FragColor = vec4(_MATERIAL.x, _MATERIAL.y, _MATERIAL.z, _MATERIAL.w);\n"//vec4(1.0, 1.0, 1.0, 1.0);\n"
    "}\n";

static const char *lineVertexShaderSource =
    "attribute lowp vec4 _POSITION;\n"
    "uniform lowp mat4 _VIEWPROJECTIONMATRIX;\n"
    "uniform lowp mat4 _TRANSFORMMATRIX;\n"
    "void main() {\n"
    "   gl_Position = (_VIEWPROJECTIONMATRIX * _TRANSFORMMATRIX) * _POSITION;\n"
    "}\n";

static const char *lineFragmentShaderSource =
    "uniform lowp vec4 _MATERIAL;\n"
    "void main() {\n"
        "   gl_FragColor = vec4(_MATERIAL.x, _MATERIAL.y, _MATERIAL.z, 1.0 * _MATERIAL.w);\n"//vec4(1.0, 1.0, 1.0, 1.0);\n"
    "}\n";

////doNotUse
////TODO: need remove
//static const char * texturedVertexShaderSource =
//        "attribute lowp vec4 _POSITION;\n"
//        "attribute lowp vec2 _TEXCOORD;\n"
//        "varying lowp vec2 textureCoord;\n"
//        //"varying lowp float cof;\n"
//        "uniform lowp mat4 modelViewProjectionMatrix;\n"
//        "void main() {\n"
//        "       gl_Position = modelViewProjectionMatrix * _POSITION;\n"/*vec4(position, 1.0);\n"*/
//        "       textureCoord = texcoord;\n"
//       // "       cof = entrcof;\n"
//        "}\n";

static const char * texturedModelVertexShaderSource =
        "attribute lowp vec4 _POSITION;\n"
        "attribute lowp vec2 _TEXCOORD;\n"
        "varying lowp vec2 textureCoord;\n"
        "uniform lowp mat4 _TRANSFORMMATRIX;\n"
        "uniform lowp mat4 _VIEWPROJECTIONMATRIX;\n"
        "void main() {\n"
        "       gl_Position = (_VIEWPROJECTIONMATRIX * _TRANSFORMMATRIX) * _POSITION;\n"
        "       textureCoord = _TEXCOORD;\n"
        "}\n";

static const char * texturedFragmentShaderSource =
        "uniform lowp sampler2D _TEX1;\n"
        "uniform lowp vec4 _MATERIAL;\n"
        "varying lowp vec2 textureCoord;\n"
        "void main()\n"
        "{\n"
        "        lowp vec4 tmp = texture2D(_TEX1, textureCoord );\n"
        "        tmp = vec4(tmp.b * _MATERIAL.x, tmp.g * _MATERIAL.y, tmp.r * _MATERIAL.z, tmp.a * _MATERIAL.w);\n"
        //"        tmp = vec4(tmp.z * cof, tmp.y * cof, tmp.x * cof, tmp.w * cof);\n"
        "        gl_FragColor = tmp;\n"
        //"gl_FragColor.a *= 0.5;\n"
        "}\n";

static const char * texturedFragmentGausBlurShaderSource =
"uniform sampler2D _TEX1;\n"
"vec3 texel_radius;\n"

"varying lowp vec2 textureCoord;\n"

"#define M_PI 3.1415926535897932384626433832795\n"

"float gauss(float x, float sigma)\n"
"{\n"
" float x_sqr = x * x;\n"
" float sigma_sqr = sigma * sigma;\n"
" float sqrt_value = 1.0 / sqrt(2.0 * M_PI * sigma_sqr);\n"
" float exp_value = exp( -x_sqr / (2.0 * sigma_sqr) );\n"
" return sqrt_value * exp_value;\n"
"}\n"

"void main()\n"
"{\n"
"texel_radius = vec3(0.001,0.001,10);\n"
" float r = texel_radius.z;\n"
" float sigma = r / 3.0;\n"

" float sum = 0.0;\n"
" vec4 value = vec4(0.0);\n"

" float x = 1.0;\n"
" while (x <= r)\n"
" {\n"
"  float currentScale = gauss(x, sigma);\n"
"  sum += 2.0 * currentScale;\n"

"  vec2 dudv = texel_radius.xy * x;\n"
"  value += currentScale * (texture(_TEX1, textureCoord - dudv) +\n"
"                     texture(_TEX1, textureCoord + dudv) );\n"
"  x += 1.0;\n"
" }\n"

" value += texture(_TEX1, textureCoord) * (1.0 - sum);\n"

" gl_FragColor = value;\n"
"}\n";

//static const char * texturedFragmentShaderSource =
//"uniform lowp sampler2D _TEX1;\n"
//"vec3 texel_radius;\n"
//"uniform lowp vec4 _MATERIAL;\n"
//"varying lowp vec2 textureCoord;\n"

//"void main()\n"
//"{\n"
//"texel_radius = vec3(0.005,0.005,30);\n"
//"float r = texel_radius.z;\n"

//"float totalScale = 1.0 + r;\n"
//"vec4 value = texture(_TEX1, textureCoord) * totalScale;\n"

//"float x = 1.0;\n"
//"while (x <= r)\n"
//"{\n"
// "vec2 dudv = texel_radius.xy * x;\n"
// "float scale = 1.0 + r - x;\n"
// "value += scale * (texture(_TEX1, textureCoord - dudv) +\n"
// "                  texture(_TEX1, textureCoord + dudv));\n"
// "x += 1.0;\n"
//"}\n"

//"gl_FragColor = value / totalScale / totalScale;\n"//gl_FragColor.a = 1;\n"
//"}\n";

//--------------------DUBLICAT----------------------
//static const char * lineVertexShaderSource =
//        "uniform mat4 uMVPMatrix;\n"
//        "attribute vec4 position;\n"
//        "void main()\n"
//        "{\n"
//        "  gl_Position = uMVPMatrix * position;\n"
//        "}\n";
//static const char * lineFragmentShaderSource =
//        //"precision lowp float;\n"
//        "uniform vec3 col;\n"
//        "void main()\n"
//        "{\n"
//        "  gl_FragColor = vColor;\n"
//        "}\n";
//---------------------------------------------------

static const char * materialVertexShaderSource =
        "attribute lowp vec3 position;\n"
        "attribute lowp vec2 texcoord;\n"
        "attribute lowp vec3 normal;"
        "uniform struct Transform\n"
        "{"
        "        mat4 model;\n"
        "        mat4 viewProjection;\n"
        "        mat3 normal;\n"
        "        vec3 viewPosition;\n"
        "} transform;\n"
        "uniform struct PointLight\n"
        "{"
        "        vec4 position;\n"
        "        vec4 ambient;\n"
        "        vec4 diffuse;\n"
        "        vec4 specular;\n"
        "        vec3 attenuation;\n"
        "} light;\n"
        "varying Vertex {\n"
        "        vec2  texcoord;\n"
        "        vec3  normal;\n"
        "        vec3  lightDir;\n"
        "        vec3  viewDir;\n"
        "        float distance;\n"
        "} Vert;\n"
        "void main(void)\n"
        "{\n"
        "        vec4 vertex   = transform.model * vec4(position, 1.0);\n"
        "        vec4 lightDir = light.position - vertex;\n"
        "        Vert.texcoord = texcoord;\n"
        "        Vert.normal   = transform.normal * normal;\n"
        "        Vert.lightDir = vec3(lightDir);\n"
        "        Vert.viewDir  = transform.viewPosition - vec3(vertex);\n"
        "        Vert.distance = length(lightDir);\n"
        "        gl_Position = transform.viewProjection * vertex;\n"
        "}\n";
static const char * materialFragmentShaderSource =
        "uniform struct PointLight\n"
        "{\n"
        "        vec4 position;\n"
        "        vec4 ambient;\n"
        "        vec4 diffuse;\n"
        "        vec4 specular;\n"
        "        vec3 attenuation;\n"
        "} light;\n"
        "uniform struct Material\n"
        "{\n"
        "        sampler2D texture;\n"
        "        vec4  ambient;\n"
        "        vec4  diffuse;\n"
        "        vec4  specular;\n"
        "        vec4  emission;\n"
        "        float shininess;\n"
        "} material;\n"
        "struct Vertex\n"
        "{\n"
        "        vec2  texcoord;\n"
        "        vec3  normal;\n"
        "        vec3  lightDir;\n"
        "        vec3  viewDir;\n"
        "        float distance;\n"
        "};\n"
        "varying Vertex Vert;\n"
        "varying lowp vec4 color;\n"
        "void main(void)\n"
        "{\n"
        "        vec3 normal   = normalize(Vert.normal);\n"
        "        vec3 lightDir = normalize(Vert.lightDir);\n"
        "        vec3 viewDir  = normalize(Vert.viewDir);\n"
        "        float attenuation = 1.0 / (light.attenuation[0] +\n"
        "                light.attenuation[1] * Vert.distance +\n"
        "                light.attenuation[2] * Vert.distance * Vert.distance);\n"
        "        color = material.emission;\n"
        "        color += material.ambient * light.ambient * attenuation;\n"
        "        float NdotL = max(dot(normal, lightDir), 0.0);\n"
        "        color += material.diffuse * light.diffuse * NdotL * attenuation;\n"
        "        float RdotVpow = max(pow(dot(reflect(-lightDir, normal), viewDir), material.shininess), 0.0);\n"
        "        color += material.specular * light.specular * RdotVpow * attenuation;\n"
        "        color *= texture(material.texture, Vert.texcoord);\n"
        "}\n";
#endif // SHADERS_H
