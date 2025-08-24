#version 400
#extension GL_ARB_separate_shader_objects : enable//开启separate_shader_objects
#extension GL_ARB_shading_language_420pack : enable	//开启shading_language_420pack
layout (std140, set = 0, binding = 0) uniform bufferVals {
    vec4 lightAmbient;
} myBufferVals;
layout (push_constant) uniform constantVals {
	mat4 mvp;//最终变换矩阵
    mat4 mm;
} myConstantVals;
layout (location = 0) in vec3 pos;//传入的顶点位置
layout (location = 0) out vec4 outLightQD; // 输出的光照强度
layout (location = 1) out vec3 vposition; //传输到片元着色器的顶点位置
out gl_PerVertex {//输出接口块
	vec4 gl_Position;//顶点最终位置
};
void main(){//主函数
    gl_Position = myConstantVals.mvp * vec4(pos, 1.0);//计算顶点最终位置
    outLightQD = myBufferVals.lightAmbient;
    vposition = pos;//把顶点位置传给片元着色器
}
