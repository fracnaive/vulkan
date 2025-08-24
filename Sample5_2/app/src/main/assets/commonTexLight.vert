#version 400
#extension GL_ARB_separate_shader_objects : enable//开启separate_shader_objects
#extension GL_ARB_shading_language_420pack : enable	//开启shading_language_420pack
layout (std140, set = 0, binding = 0) uniform bufferVals {
    vec4 lightAmbient;
    vec4 lightPosition;
    vec4 lightDiffuse;
} myBufferVals;
layout (push_constant) uniform constantVals {
	mat4 mvp;//最终变换矩阵
    mat4 mm;
} myConstantVals;
layout (location = 0) in vec3 pos;//传入的顶点位置
layout (location = 1) in vec3 inNormal;//传入的顶点法向量
layout (location = 0) out vec4 outLightQD; // 输出的光照强度
layout (location = 1) out vec3 vposition; //传输到片元着色器的顶点位置
out gl_PerVertex {//输出接口块
	vec4 gl_Position;//顶点最终位置
};
vec4 pointLight (
    in mat4 uMMatrix,
    in vec3 lightLocation,
    in vec4 lightDiffuse,
    in vec3 normal,
    in vec3 aPosition) {
    vec4 diffuse;
    vec3 normalTarget = aPosition + normal;
    vec3 newNormal = (uMMatrix * vec4(normalTarget, 1.)).xyz - (uMMatrix * vec4(aPosition, 1.0)).xyz;
    newNormal = normalize(newNormal);
    vec3 vp = normalize(lightLocation - (uMMatrix * vec4(aPosition, 1.0f)).xyz);
    vp = normalize(vp);
    float nDotViewPosition = max(0.0, dot(newNormal, vp));
    diffuse = lightDiffuse * nDotViewPosition;
    return diffuse;
}

void main() {//主函数
    gl_Position = myConstantVals.mvp * vec4(pos, 1.0);//计算顶点最终位置
    outLightQD = myBufferVals.lightAmbient;
    outLightQD += pointLight(
        myConstantVals.mm,
        myBufferVals.lightPosition.xyz,
        myBufferVals.lightDiffuse,
        inNormal,
        pos);
    vposition = pos;//把顶点位置传给片元着色器
}
